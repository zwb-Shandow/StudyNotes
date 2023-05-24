# ROS 2.0 rosbags

关于如何在ROS 2.0生态系统中高效实现rosbags的研究和想法

## 动机

ROS 1最重要的组成部分之一是其持久化的数据记录机制，称为rosbags。它已被证明是ROS 1的核心和重要组成部分，因为记录和回放所有类型的系统数据的能力对于数据分析和调试目的至关重要。在这些数据记录的各种用例中，rosbags可以包含非常简单的数据（如机器人末端执行器的轨迹），也可以包含高度复杂的数据（例如具有多个冗余高分辨率传感器的自动驾驶车辆）。因此，它们的文件大小可以从只有几KB（只有几条消息）到数TB（有数百万条消息）不等。为了在日志记录时保持高性能，rosbags避免对记录的消息进行序列化和反序列化。

在设计ROS 2时，还必须具备高性能数据记录的功能。由于ROS 1和ROS 2之间存在根本性差异，因此应在本文中研究技术挑战。

### ROS 1中的rosbags

ROS 1中rosbags的实现仅次于rqt_bag等高级工具，分为两个基本包。rosbag_storage包抽象了ROS客户端库中ROS消息的写入和读取，因此将存储与ROS接口分离，并实现了这里描述的自己的存储格式。该格式的当前版本2.0具有压缩和二级索引等功能，用于收集有关包文件内容的信息。

### ROS 2中的用例

在ROS 2中，Rosbags的实施带来了新的用例。新的ROS 2 API体系结构引入了一个通用的RMW接口，该接口允许来自不同中间件的各种数据格式，例如来自DDS的CDR（公共数据表示）。

尽管DDS是ROS 2的默认中间件，但ROS 2中rosbags的实现也足够灵活，以支持记录和回放CDR以外的其他数据格式，如protobuf或ZeroMQ。

### ROS 2中rosbags的技术要求

#### 确定性

bag文件的回放必须是确定性的。这意味着当按n次顺序播放bag文件时，接收方应接收n次确定的序列。这可以通过在录制时正确标记每个传入消息的时间来实现。时间戳可以是两个：第一个时间戳是关于实际消息离开发布者的时间，第二个时间戳则是消息被接收并存储在bag文件中的时间。

#### 适应性

当考虑将rosbag文件作为长期存储时，在录制和回放时间段内可能会发生消息定义变更的情况。为了应对这种情况，回放功能应提供一种将旧消息转换为新定义的机制。这可能在功能上受限于新消息定义的字段添加。这意味着所有重叠字段都将被转移到默认的新添加字段的位置。显然，执行此转换步骤需要对ROS 2消息进行反序列化，并且在有效地重放序列化数据时无法执行。

#### 可扩展性

如今，机器人系统可以同时包含大量传感器发布数据。随着时间的推移，这很容易导致大量数据。因此，所选格式能够扩展到巨大的文件大小（> 1 TB）。

#### 随机存取

必须能够授予对文件的随机读取权限，并提取特定的单个消息。随机访问还意味着可以提取一个主题的第n条消息，而不必滚动浏览同一块中的所有消息。

#### 范围存取

此外，还可以仅重放/读取记录的某一部分，该部分由时间范围指定。应当可以访问从tx到tx+n的时间戳范围内的消息。

#### 可变块大小

块大小必须配置以适合各种大型消息类型，以确保各种大型消息类型的最佳性能。应该进一步配置何时将这样的块永久写入磁盘上的条件（例如，在给定的时间间隔或何时达到块大小）。

#### 兼容性

一般要求是与现有ROS1 bag向后兼容。这种兼容性可以通过转换脚本实现，该脚本将ROS1 bag永久转换为ROS2 bag，也可以通过桥接API实现，该桥接API允许手动打开现有的ROS1 bag并将其发布到ROS2系统中。

## ROS 2中rosbags实现

下一节介绍了ROS2中rosbags实施的总体建议。类似于ROS 1中的实现方式，其想法是将体系结构分为多个独立的软件包。

自上而下，我们定义了三层抽象层: rosbag storage API、rosbag API和rosbag CLI。

## rosbag storage API

rosbag存储API与ROS2无关，因为它只读取和写入抽象的二进制消息，这些消息在给定一些元信息的情况下定义良好。

我们将数据存储定义为负责以二进制格式存储传入ROS消息的底层包。数据存储负责持久保存ROS消息，其中包含足够的元信息，以在加载时恢复其完整上下文。虽然其主要目的是存储ROS消息，但抽象层应该是ROS2不可知的，并且实际上只处理二进制数据和通用元信息。如果有合适的元信息，这允许手动插入自定义数据，例如PCAP记录。

### 数据编码

我们将编码的数据定义为以二进制形式表示ROS消息的格式。编码必须唯一描述，并链接到数据存储中的二进制表示。也就是说，能够解释其二进制表示，从而恢复ROS消息。在这一点上，此二进制表示的内容对于存储API并不重要。但是，我们指定，每个主题都必须有一个编码，并且不能为一个主题存储具有不同编码的消息。

### 数据存储

我们引入了一个单独的API层，用于与底层数据存储对接。为了这种表示包括实际消息的二进制blob和元数据的键值对。写消息，必须将消息的抽象表示作为输入。在研究“最佳数据存储”时，我们得出的结论是，没有一种最佳数据存储能够满足所有需求。因此，我们希望能够支持多种数据存储解决方案（例如SQLite或rosbag format2）。因此，重点是创建一个足够强大的API，a）支持多个数据存储，b）能够将数据从一个存储转移到另一个存储。这方面的一个典型用例是在记录高性能关键数据（例如在汽车上）时运行非常高效的写入存储格式。然后，在第二步骤中，对记录的数据进行后处理，以存储在更适合于内省数据的另一存储格式中。因此，用于访问底层存储的API必须足够通用，以独立地描述消息，并相应地进行解释，以正确地存储每个解决方案的消息。类似地，应建立查询格式，允许从数据存储中提取/过滤消息到二进制blob及其元数据中。

#### SQLite

我们选择了SQLite作为此实现的默认解决方案。与其他关系数据库系统相反，它不需要任何外部SQL Server，而是独立的。它也是开源的，经过广泛的测试，并且有一个大型社区。Gazebo团队在SQLite和现有的rosbag格式之间创建了一个很好的比较。

除了上述原因之外，我们选择SQLite的原因还有：

- 这是一个非常小的依赖，在所有主要目标平台上都得到了很好的测试
- 它易于使用，只需一个文件即可正常工作（允许轻松开发初始API和存储）
- 基准测试表明，写入速度超过1GB/s是可能的（大消息，这达到了磁盘写入的极限）
- 写高频的小消息会产生不错的结果（> 100,000条消息/s）

##### 优点

- 表维度在启动时不必是已知的，并且可以灵活扩展。
- 能够使用经典的关系SQL语法和工具查询表。

##### 缺点

- 与简单地将传入数据转储到文件（即ROS 1 rosbag）的格式相比，可能存在写入性能缺陷。

#### 插件式数据存储器

在研究了现有的解决方案后，我们决定为这个API层实现一个可扩展的插件架构，它能够连接各种数据存储解决方案，从而让用户有机会根据他们的用例优化他们的数据存储。

如上所述，我们选择了SQLite作为默认值。然而，当考虑与现有ROS 1 rosbags的集成或兼容性时，这种插件架构已经成为必要。由于现有的ROS 1 rosbag中存储了大量数据，因此ROS 2的rosbag工具必须能够轻松处理遗留的rosbags。这可以通过拥有ROS 1数据存储插件来实现，该插件可以从ROS 1包中读取现有的ROS 1消息，并将其桥接为ROS 2消息格式，例如使用静态桥接。请注意，这个插件很可能会以只读方式实现，这意味着只能读取现有的ROS 1消息并将其填充到ROS 2生态系统中，但目前不支持其他方式。然而，一个可能的插件可以是rosbag格式3.0的扩展，其设计类似于现有的格式2.0，但允许存储ROS 2消息和连接信息。

当能够使用多个数据存储时，用户必须指定要使用哪一个。为此，我们设想了ros2bag-cli的一个选项，其中使用可以指定非默认数据存储。

### API

我们设想使用以下接口来使用此存储API。完整的API可能是一个纯C接口，在C++中实现。这样做的背景是使这个API能够很容易地与其他语言（如Python或Java）进行封装。（下面显示的所有代码都是伪代码，可能会发生更改。）

#### open

每个进程调用一次**open**。其想法是一次性完成所有细节，以便为读写准备数据格式。

- `BagHandle open(string file_path, void* impl_data)`

#### close

退出时每个进程调用一次**close**。

- `bool close(BagHandle* handle)`

#### create_topic

对于要录制的每个新主题，都会调用**create_topic**一次。它接收连接信息、指示相应二进制数据的编码的唯一标识符、数据的块大小，并且可以采用用于实现特定数据的可选指针。这里隐含的约定是每个主题都有一个确切的编码。不支持按消息和主题混合编码。

- `TopicHandle create_topic(BagHandle* handle, ConnectionHeader connection_header, string encoding_id, int chunk_size, void* impl_data)`

#### write

**write**将数据持久化为底层存储格式。它接收一个主题句柄，用于标识二进制数据应放置的位置、接收消息的时间戳以及用于实现特定数据的可选指针。

- `void write(BagHandle* handle, TopicHandle* topic, struct time_received, SerializedData* serialized_data, void* impl_data)`

#### read

**read**从底层存储格式加载数据。我们设想了多种访问数据的方式。第一种是迭代器的风格，它允许按顺序迭代所有消息。可以选择指定主题筛选器。

- `SerializedData read_next(IteratorHandle* iter, TopicHandle* topic, void* impl_data)`

访问数据的第二种方式可以通过对数据的特定部分进行索引来完成。查询参数可用于指定特定主题的消息或指定时间范围。此外，可以通过参数指定这些消息的返回顺序。

- `SerializedData[] read(struct query, enum order_by, void* impl_data)`

#### info

**info**返回有关存储数据的一般信息和统计信息。这里可能的数据可以是主题列表、数据的总持续时间等，然后用于进一步指定读取命令。

- `BagInfo info(BagHandle* handle)`

#### index

index创建bag的索引。该功能应在录制完成后调用。该文件的目的是对记录的数据创建索引，并作为有关bag的一般信息。这些元信息应存储在单独文件（yaml或类似文件）中实际数据旁边。

- `void index(BagHandle* handle)`

我们假设对以下数据类型有类似的定义：

```c
struct ConnectionHeader
{
  string topic_name,
  string topic_type,
  string message_definition,
  string message_md5_sum,
  struct quality_of_service_params
}
```

实际有效载荷的结构如下：

```c
struct SerializedData
{
  byte[] payload,
  int size,
}
```

BagInfo与ROS1 bag信息类似。大多数字段都设置为必填字段，并添加了一个通用键值对，该键值对可以根据实现细节进行指定。我们设想BagInfo结构如下：

```c
struct BagInfo
{
  string path,
  int file_size,
  time duration,
  time start,
  time end,
  int message_count,
  int topic_count,
  string[] topics,
  string[] types,
  map[string, string] key_value_data
}
```

我们每个主题只需要一个编码。TopicHandle可以封装编码信息:

```c
struct TopicHandle
{
  string topic_name,
  string topic_type,
  string encoding_identifier
}
```

## rosbag API

rosbag API描述了记录和重放以及读取和写入ros消息的必要接口。它负责在数据存储器中存储具有正确数据格式的传入消息。同样，API必须从数据存储中的二进制表示中恢复具有给定数据格式的ros消息。

rosbag API对ROS 2的rosidl 类型支持具有很强的依赖性。此软件包的主要目的是向存储API发出查询，接收二进制数据和相应的元信息，然后将消息转换为其适当的ROS 2消息类型。

ROS 2通过设计独立于其基础中间件。这意味着，输入的二进制数据可以根据中间件的不同而不同。对于DDS（即ROS 2默认值），编码为CDR。但是，其他所有中间件都可以使用协议缓冲区或类似于交换数据。由于ROS 2独立于其中间件，因此ROS 2 ROSBAG中的存储数据也应独立且可以互换。为了实现这一目标，我们使用了一个插件系统，该系统允许将ROS 2消息转换为特定编码。因此，该ROSBAG API应提供一个通用函数，该功能可以将任何ROS 2消息序列化为可用编码或从可用编码序列化为ROS 2消息。默认情况下，我们为CDR提供了这样的插件。这样，每个中间件都可以使用此功能将CDR验证为ROS 2，然后在必要时进一步序列到其自己的编码。我们看到这一点 - 引入可插入的序列化格式 - 是提供通用Rosbags的唯一方法。我们还清楚地看到，这引入了重要的开销。因此，我们使此概括可选，以确保使用相同的中间件进行录制和重播时的最佳读/写性能。当没有执行任何编码时，中间件特定的二进制数据将被存储，而无需进行任何进一步的处理。如果没有这种常见的序列化，则可以在多个中间的多个中间的中很容易交换bag，但是可以使用相同的功能将bag离线转换。在这种情况下，非通用包可以离线处理并序列化为任何编码，而不会受到任何性能约束。

如果一个选项，例如 - 编码为ROS2袋记录，然后将每个传入的ROS 2消息序列化到指定的编码格式，然后将其存储在袋中。相反，当没有指定任何指定时，Rosbag API以二进制格式直接接收传入的消息，因此可以将其转发到存储API而无需任何进一步的序列化。显然，当指定的编码恰好与基础中间件相同时，未进行进一步的转换，并且直接存储传入的消息。

读取包文件的数据流基本上与上述相同。读时，必须适用于存储在bag中的每个编码的插件。如果不存在主题编码的插件，则该主题将被忽略，不重播或只能以二进制形式访问。

我们试图将实际的读写API与传输层分离，以使依赖关系尽可能小。在第一组API中，我们看到了读取和写入ROS2消息并将其转换为指定编码的函数。其次，在另一层，我们引入了ROS 2消息的传输，用于发布和订阅。

### API 1 (Read and Write of ROS 2 message)

我们设想一下API:

#### open

打开一个bag。与存储API类似，rosbag API有一个打开bag的入口点。

- `BagHandle* open(string file_path)`

#### write

关闭已经打开的bag

- `close(BagHandle *)`

#### info

返回bag的一组元数据

- `BagInfo info(BagHandle *)`

#### create_topic

为附加消息准备一个新主题。此调用类似于存储API

- `TopicHandle create_topic(BagHandle * handle, ConnectionHeader connection_header, string encoding_id, int chunk_size)`

#### open_topic

在存储API中打开现有主题

- `TopicHandle open_topic(BagHandle * handle, string topic_name)`

#### read

允许读取一组ROS 2消息。应可选择获取一组固定的ROS 2消息

- `ROS 2[] read(struct query, enum order_by)`

类似地，应该有一个迭代器模式来获得ROS 2消息的序列

- `ROS 2 read_next(IteratorHandle * iter, TopicHandle * topic, void * impl_data)`

另一个选项是一种异步方法，它接受从存储中成功加载数据时要触发的回调

- `void read_async(struct query, enum order_by, void (cb *)(ROS 2[] data))`

我们可以进一步直接展示存储的二进制数据：

- `SerializedData[] read_serialized(struct query, enum order_by)`
- `SerializedData read_next(IteratorHandle * iter, TopicHandle * topic, void * impl_data)`
- `void read_async(struct query, enum order_by, void (cb *)(SerializedData[] data))`

#### write

允许将新的ROS 2消息写入包中。与读取功能集类似，应具有写入功能。

- `void write(BagHandle * handle, TopicHandle * topic, struct time_received, ROS 2 msg)`
- `void write_serialized(BagHandle * handle, TopicHandle * topic, struct time_received, SerializedData serialized_msg)`

### API 2 (Record and Replay)

上面提到的读写功能被设计为手动读写带有自定义消息或特定用例的bag文件，其中当前系统的普通记录/回放是不够的。对于后者，有一组功能，允许记录和回放正在运行的系统。这些函数订阅一组主题，记录传入消息，并将这些消息写入存储API。回放的情况正好相反，在回放中，给定一个查询，生成的主题就会被发布。

- `void record(BagHandle * handle, string[] topics)`
- `void record_all(BagHandle * handle)`
- `void replay(BagHandle * handle, string[] topics)`
- `void replay_all(BagHandle * handle)`

### Conversions

### Limitations

## rosbag CLI

rosbag CLI应作为现有ros2cli的新动词的一部分集成，并为简单的录制和回放设置了入口点。与ROS 1一样，此命令行界面可轻松控制录制和回放以及bag文件的反射。在写期间，我们为此接口指定了三个主要命令行动词：

#### record

记录动词应采用一组要记录的主题或指定所有主题。

- `ros2 bag record -a`
- `ros2 bag record <topicA> <topicB> ... <topicN>`

#### play

播放动词应重播现有的bag文件

- `ros2 bag play <bag_file>`

#### info

信息动词应在终端上打印所有有关bag文件的必要信息

- `ros2 bag info <bag_file>`

此CLI软件包仅作为一个简单的接口，没有任何主要的API。大部分逻辑都发生在rosbag API中。由于rosbag API是一个仅限C语言的接口，因此可以很容易地通过该命令行接口进行调用和封装。这背后的原因是，rosbag API应在没有任何命令行接口依赖的情况下完全发挥作用。

### 高级命令行选项

上述功能描述了使用ROS 2的rosbags的常规方法。将应用默认的ROS 2中间件，并使用默认的数据存储（即SQLite）。在这种情况下，应使用非默认的RMW实现，这可以通过ROS 2环境变量进行设置：

```shell
RMW_IMPLEMENTATION=rmw_<middleware> ros2 bag record
```

如上所述，这种记录数据的方式将以RMW特定的二进制格式存储数据。这同样意味着没有其他中间件可能能够重播此数据。为了编写通用rosbag，我们向record动词（称为encoding）介绍了一个新选项。这样，所有传入数据将转换为此编码。

```shell
RMW_IMPLEMENTATION=rmw_<middleware> ros2 bag record --encoding <encoding_identifier>
```

在使用非默认存储格式的情况下，我们提供了一个新选项来选择要使用的存储格式：

```shell
ros2 bag record --storage-format=<format_identifier>
```

在读取bag文件时，我们还可以指定使用哪种格式，以防bag没有用ROS 2 CLI记录，因此没有元信息文件。

```shell
ros2 bag play --storage-format=<format_identifier>
```

#### convert

应该有第四个动词将消息从一个bag转换为另一个bag。这不会在第一次运行中实现，但是我们可以看到这样的API：

- `ros2 bag convert <bag_file_a> <bag_file_b> --encoding <encoding_b> --format <format_b>`

## rosbag2 源码解析

### rosbag2_storage API层

rosbag_storage API与ROS2无关，因为它只读取和写入抽象的二进制消息，这些消息在给定一些元信息的情况下定义良好。

#### 目录

rosbag2_storage的目录结构如下：

```
|- storage_interface               // 接口定义
	|- base_info_interface.hpp
	|- base_io_interface.hpp
	|- base_read_interface.hpp
	|- base_write_interface.hpp
	|- read_only_interface.hpp     // 插件开发需继承的接口，具有只读权限
	|- read_write_interface.hpp    // 插件开发需继承的接口，具有读写权限
|- bag_metadata.hpp                // bag元信息
|- logging.hpp
|- metadata_io.hpp                 // bag元数据的io接口
|- ros_helper.hpp
|- serialized_bag_message.hpp      // 序列化后的数据
|- storage_factory_interface.hpp   // 存储插件工厂类接口
|- storage_factory.hpp             // 存储插件工厂类
|- storage_filter.hpp              // 回放时话题过滤列表
|- storage_options.hpp             // 数据包存储时参数，包含包大小限制、时长限制、缓存限制等
|- storage_traits.hpp
|- topic_metadata.hpp              // 话题元信息，包含名称、类型、序列化格式及qos规则等
|- visbility_control.hpp
```



接口继承关系如下：

![](/home/trunk/Documents/personal/StudyNotes/docs/Pic/rosbag2.png)



### rosbag2 API层

#### 目录

```
|- cache
	|- cache_consumer.hpp
	|- message_cache_buffer.hpp
	|- message_cache.hpp
|- converter_interfaces
	|- serialization_format_converter.hpp
	|- serialization_format_deserializer.hpp
	|- serialization_format_serializer.hpp
|- reader_interfaces
	|- base_reader_interface.hpp        // 只读抽象类接口
|- readers
	|- sequential_reader.hpp
|- types
	|- introspection_message.hpp
|- writer_interfaces
	|- base_writer_interface.hpp
|- writers
	|- sequential_write.hpp
|- converter_options.hpp
|- converter.hpp
|- info.hpp
|- logging.hpp
|- reader.hpp
|- writer.hpp
|- serialization_format_converter_factory_interface.hpp
|- serialization_format_converter_factory.hpp
|- storage_options.hpp
|- types.hpp
|- typesupport_helpers.hpp
|- visbility_control.hpp
```

