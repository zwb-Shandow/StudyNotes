# TensorRT

> 翻译自 https://docs.nvidia.com/deeplearning/tensorrt/developer-guide/index.html#perform_inference_c

## 摘要

本 NVIDIA TensorRT 开发人员指南演示了如何使用 C++ 和 Python API 来实现最常见的深度学习层。它展示了如何采用使用深度学习框架构建的现有模型并使用提供的解析器构建 TensorRT 引擎。开发人员指南还提供了常见用户任务的分步说明，例如创建 TensorRT 网络定义、调用 TensorRT 构建器、序列化和反序列化，以及如何向引擎提供数据并执行推理；全部同时使用 C++ 或 Python API。

## 1. 介绍

NVIDIA®TensorRT™ 是一个促进高性能机器学习推理的SDK。它被设计为与TensorFlow、PyTorch和MXNet等培训框架以互补的方式工作。它特别专注于在NVIDIA硬件上快速高效地运行已经训练好的网络。

NVIDIA TensorRT快速入门指南适用于想要试用TensorRT SDK的用户；具体来说，您将学习如何构建一个应用程序，以便在TensorRT引擎上快速运行推理。

### 1.1 本指南的结构

第 1 章提供有关 TensorRT 如何打包和支持，以及它如何融入开发者生态系统的信息。

第2章概括介绍了TensorRT的功能。

第三章和第四章分别介绍了C++和Python API。

后续章节提供了有关高级功能的更多详细信息。

附录包含一个图层参考和常见问题解答。

### 1.2 示例

NVIDIA TensorRT 示例支持指南阐释了本指南中讨论的许多主题。可以在此处找到针对嵌入式应用程序的其他示例。

### 1.3 补充GPU的功能

### 1.4 配套软件

NVIDIA Triton™ 推理服务器是一个更高级别的库，可跨 CPU 和 GPU 提供优化的推理。它提供启动和管理多个模型的功能，以及用于服务推理的 REST 和 gRPC 端点

NVIDIA DALI® 提供用于预处理图像、音频和视频数据的高性能原语。TensorRT 推理可以作为自定义运算符集成到 DALI 管道中。可以在此处找到集成为 DALI 一部分的 TensorRT 推理的工作示例。

TensorFlow-TensorRT (TF-TRT) 是将 TensorRT 直接集成到 TensorFlow 中。它选择 TensorFlow 图的子图由 TensorRT 加速，同时让图的其余部分由 TensorFlow 本地执行。结果仍然是一个 TensorFlow 图，您可以照常执行。有关 TF-TRT 示例，请参阅 TensorFlow 中的 TensorRT 示例。

Torch-TensorRT (Torch-TRT) 是一个 PyTorch-TensorRT 编译器，可将 PyTorch 模块转换为 TensorRT 引擎。在内部，PyTorch 模块首先根据所选的中间表示 (IR) 转换为 TorchScript/FX 模块。编译器选择 PyTorch 图的子图由 TensorRT 加速，同时让图的其余部分由 Torch 本机执行。结果仍然是一个可以照常执行的 PyTorch 模块。有关示例，请参阅 Torch-TRT 示例。

TensorFlow-Quantization Toolkit 提供了用于以降低的精度训练和部署基于 Tensorflow 2 的 Keras 模型的实用程序。该工具包用于仅根据运算符名称、类和模式匹配来量化图中的不同层。然后，量化图可以转换为 ONNX，然后转换为 TensorRT 引擎。例如，请参考模型动物园。

PyTorch量化工具包提供了以较低精度训练PyTorch模型的设施，然后可以将其导出以在TensorRT中进行优化。

此外，PyTorch自动稀疏性（ASP）工具为训练具有结构化稀疏性的模型提供了便利，然后可以将其导出，并允许TensorRT在NVIDIA Ampere Architecture GPU上使用更快的稀疏策略。

TensorRT 与 NVIDIA 的分析工具、NVIDIA Nsight™ 系统和 NVIDIA 深度学习分析器 (DLProf) 集成。

TensorRT的一个子集已通过NVIDIA DRIVE®产品认证。某些API标记为仅在NVIDIA DRIVE中使用，不支持通用。

### 1.5 ONNX

TensorRT 从框架导入经过训练的模型的主要方法是通过 ONNX 交换格式。TensorRT 附带 ONNX 解析器库来协助导入模型。在可能的情况下，解析器是向后兼容的，直到操作集7；ONNX模型操作集版本转换器可以帮助解决不兼容问题。

GitHub 版本可能支持比 TensorRT 附带的版本更新的 opset，请参阅 ONNX-TensorRT 运算符支持矩阵，了解有关支持的 opset 和运算符的最新信息。对于 TensorRT 部署，我们建议导出到最新的可用 ONNX opset。

可在[此处](https://github.com/onnx/onnx-tensorrt/blob/main/docs/operators.md)找到 TensorRT 的 ONNX 运算符支持列表。

PyTorch 本身支持 ONNX 导出。对于TensorFlow，推荐的方法是 tf2onnx。

将模型导出到ONNX后，一个好的第一步是使用Polygraphy运行恒定折叠。这通常可以解决ONNX解析器中的TensorRT转换问题，并通常简化工作流程。有关详细信息，请参阅此示例。在某些情况下，可能需要进一步修改ONNX模型，例如，用插件替换子图，或者在其他操作方面重新实现不支持的操作。为了简化此过程，可以使用ONNX GraphSurgeon。

### 1.6 代码分析工具

有关将 valgrind 和 clang sanitizer 工具与 TensorRT 结合使用的指南，请参阅“故障排除”一章。

## 2. TensorRT 的功能

本章概述了 TensorRT 的功能。它旨在对所有 TensorRT 用户有用。

### 2.1 C++ 和 Python API

TensorRT的API具有针对C++和Python的语言绑定，具有几乎相同的功能。Python API促进了与Python数据处理工具包和NumPy和SciPy等库的互操作性。C++API可以更高效，并且可以更好地满足某些法规遵从性要求，例如在汽车应用中。

注：Python API并不适用于所有平台。有关更多信息，请参阅NVIDIA TensorRT支持列表。

### 2.2 编程模型

TensorRT 分两个阶段运行。

在第一阶段（通常离线执行）中，您向 TensorRT 提供模型定义，然后 TensorRT 针对目标 GPU 对其进行优化。

在第二阶段，您使用优化的模型来运行推理。

#### 2.2.1 构建阶段

TensorRT 构建阶段的最高级别接口是 *Builder*（C++、Python）。builder 负责优化模型并生成引擎。

为了构建引擎，你必须：

- 创建网络定义
- 为 builder 指定配置
- 调用 builder 创建引擎

*NetworkDefinition* 接口（C++、Python）用于定义模型。将模型传输到 TensorRT 的最常见途径是将其从 ONNX 格式的框架中导出，并使用 TensorRT 的 ONNX 解析器来填充网络定义。但是，您也可以使用 TensorRT 的 *Layer*（C++、Python）和 *Tensor*（C++、Python）接口逐步构建定义。

无论选择哪种方式，都必须定义哪些张量是网络的输入和输出。未标记为输出的张量被认为是可以由构建器优化掉的瞬态值。**必须命名输入和输出张量，以便在运行时，TensorRT知道如何将输入和输出缓冲区绑定到模型。**

*BuilderConfig* 接口（C++、Python）用于指定 TensorRT 应如何优化模型。在可用的配置选项中，您可以控制 TensorRT 降低计算精度的能力，控制内存和运行时执行速度之间的权衡，并限制 CUDA® 内核的选择。由于构建器可能需要几分钟或更长时间才能运行，因此您还可以控制构建器如何搜索内核以及缓存搜索结果以供后续运行使用。

有了网络定义和构建器配置后，您可以调用构建器来创建引擎。该构建器消除了死计算、折叠常量、重新排序和组合操作，以便在 GPU 上更有效地运行。它可以选择降低浮点计算的精度，方法是简单地以 16 位浮点运行它们，或者通过量化浮点值以便可以使用 8 位整数执行计算。它还使用不同的数据格式对每一层进行多次实现，然后计算执行模型的最佳时间表，从而最大限度地减少内核执行和格式转换的综合成本。

构建器以一种称为 *plan* 的序列化形式创建引擎，该形式可以立即反序列化，也可以保存到磁盘以备将来使用。

注意：

- 默认情况下，TensorRT 创建的引擎特定于创建它们的 TensorRT 版本以及创建它们的 GPU。请参阅版本兼容性和硬件兼容性部分，了解如何配置引擎以实现向前兼容性。
- TensorRT 的网络定义不会深拷贝参数数组（例如卷积的权重）。因此，在构建阶段完成之前，不得释放这些数组的内存。使用 ONNX 解析器导入网络时，解析器拥有权重，因此在构建阶段完成之前不得将其销毁。
- 构建器对算法进行计时以确定最快的。与其他 GPU 工作并行运行构建器可能会扰乱时序，从而导致优化效果不佳。

#### 2.2.2 运行时阶段

TensorRT执行阶段的最高级别接口是 *Runtime*（C++，Python）。

使用运行时，通常会执行以下步骤：

- 反序列化 *plan* 以构建引擎
- 为引擎创建执行上下文

然后，反复执行：

- 填充用于推理的输入缓冲区。
- 在执行上下文上调用 *enqueueV3()* 来运行推理。

*Engine* 接口（C++、Python）代表优化的模型。您可以查询引擎以获取有关网络输入和输出张量的信息 - 预期维度、数据类型、数据格式等。

从引擎创建的 *ExecutionContext* 接口（C++，Python）是调用推理的主要接口。执行上下文包含与特定调用关联的所有状态，因此您可以将多个上下文与单个引擎关联，并并行运行它们。

调用推理时，必须在适当的位置设置输入和输出缓冲区。根据数据的性质，数据可能位于 CPU 或 GPU 内存中。如果根据您的模型不明显，您可以查询引擎以确定在哪个内存空间中提供缓冲区。

在建立缓冲区之后，可以对推理进行排队（enqueueV3）。所需的内核在CUDA流上排队，并尽快将控制权返回给应用程序。有些网络需要在CPU和GPU之间进行多次控制传输，因此控制可能不会立即返回。要等待异步执行完成，请使用 *cudaStreamSynchronize* 对流进行同步。

### 2.3 插件

TensorRT 有一个 *Plugin* 接口，允许应用程序提供 TensorRT 本身不支持的操作的实现。在转换网络时，ONNX 解析器可以找到使用 TensorRT 的 *PluginRegistry* 创建和注册的插件。

TensorRT 附带了一个插件库，可以在[此处](https://github.com/NVIDIA/TensorRT/tree/main/plugin)找到其中许多插件和一些其他插件的源代码。

您还可以编写自己的插件库并使用引擎将其序列化。

有关更多详细信息，请参阅使用自定义层扩展 TensorRT 章节。

### 2.4 类型和精度

TensorRT 支持 FP32, FP16, INT8, INT32, UINT8, BOOL 数据类型。

有关层 I/O 数据类型规范，请参阅[TensorRT操作符文档](https://docs.nvidia.com/deeplearning/tensorrt/operators/docs/)。

- FP32, FP16
  - 未量化的高精度类型
- INT8
  - 隐式量化
    - 解释为量化的整数类型。INT8类型的张量必须具有关联的比例因子（通过校准或setDynamicRange API）。
  - 显式量化
    - 解释为带符号整数类型。INT8类型的转换需要显式Q/DQ层。
- UINT8
  - 数据类型仅可用作网络 I/O 类型。
  - 在将数据用于其他操作之前，必须使用 CastLayer 将 UINT8 中的网络级输入从 UINT8 转换为 FP32 或 FP16。
  - UINT8 中的网络级输出必须由已显式插入网络的 CastLayer 生成（仅支持从 FP32/FP16 到 UINT8 的转换）。
  - 不支持 UINT8 量化。
  - *ConstantLayer* 不支持 UINT8 作为输出类型。
- BOOL
  - 与受支持的层一起使用的布尔类型。

当TensorRT选择CUDA内核在网络中实现浮点运算时，它默认为FP32实现。有两种方法可以配置不同级别的精度：

- 为了控制模型级别的精度，*BuilderFlag* 选项（C++、Python）可以向 TensorRT 指示，在搜索最快的实现时，它可能会选择较低精度的实现（并且因为较低的精度通常会更快，如果允许的话，通常会这样做）。因此，您可以轻松指示 TensorRT 对整个模型使用 FP16 计算。对于输入动态范围约为 1 的正则化模型，这通常会产生显著的加速，而精度变化可以忽略不计。
- 对于更细粒度的控制，由于网络的一部分对数字敏感或需要高动态范围，因此层必须以更高的精度运行，可以为该层指定算术精度。

有关更多详细信息，请参阅降低的精度部分。

### 2.5 量化

TensorRT 支持量化浮点数，其中浮点值线性压缩并舍入为8位整数。这显著增加了算术吞吐量，同时减少了存储要求和内存带宽。在对浮点张量进行量化时，TensorRT 必须知道其动态范围 - 也就是需要表示的值的范围 - 超出此范围的值在量化时会被夹在范围内。

动态范围信息可以由构建器基于代表性输入数据来计算（这称为校准）。或者，您可以在框架中执行量化感知训练，并将包含必要动态范围信息的模型导入 TensorRT。

### 2.6 张量和数据格式

定义网络时，TensorRT 假设张量由多维 C 样式数组表示。每个层对其输入都有特定的解释：例如，2D 卷积将假设其输入的最后三个维度采用 CHW(Channel-Height-Width) 格式 - 无法选择使用类似 WHC 格式。有关每个层如何解释其输入的信息，请参阅 NVIDIA TensorRT Operator's Reference。

> CHW 表示 "Channel-Height-Weight"，是一种表示图像或张量维度的格式，特别在深度学习中常用。

请注意，张量最多限于 2^31-1 个元素。

在优化网络时，TensorRT 在内部执行转换（包括 HWC，但也包括更复杂的格式）以使用最快的 CUDA 内核。一般来说，选择格式是为了优化性能，应用程序无法控制这些选择。然而，底层数据格式在 I/O 边界（网络输入和输出，以及将数据传入和传出插件）公开，以允许应用程序最大限度地减少不必要的格式转换。

有关更多详细信息，请参阅I/O格式部分。

### 2.7 动态形状

默认情况下，TensorRT 根据定义模型的输入形状（批量大小、图像大小等）来优化模型。但是，可以将构建器配置为允许在运行时调整输入尺寸。为了实现这一点，您可以在构建器配置中指定一个或多个 OptimizationProfile（C++、Python）实例，其中包含每个输入的最小和最大形状，以及该范围内的优化点。

TensorRT为每个配置文件创建一个优化的引擎，选择适用于[最小值，最大值]范围内所有形状的CUDA内核，并且对于优化点来说是最快的 - 通常每个配置文件的内核不同。然后，您可以在运行时在这些配置文件中进行选择。

有关更多详细信息，请参阅“使用动态形状”一章。

### 2.8 DLA

TensorRT 支持 NVIDIA 的深度学习加速器 (DLA)，这是许多 NVIDIA SoC 上存在的专用推理处理器，支持 TensorRT 层的子集。TensorRT 允许您在 DLA 上执行网络的一部分，而在 GPU 上执行其余部分；对于可以在任一设备上执行的层，您可以在构建器配置中逐层选择目标设备。

有关更多详细信息，请参阅使用DLA一章。

### 2.9 更新权重

构建引擎时，您可以指定稍后更新其权重。如果您经常更新模型的权重而不更改结构，例如在强化学习中或在保留相同结构的情况下重新训练模型时，这可能会很有用。使用 *Refitter*（C++、Python）接口执行权重更新。

有关更多详细信息，请参阅"Refitting an Engine"一节

### 2.10 trtexec 工具

samples目录中包含一个名为trtexec的命令行包装工具。trtexec 是一个使用 TensorRT 的工具，无需开发自己的应用程序。trtexec工具有三个主要用途：

- 根据随机或用户提供的输入数据对网络进行基准测试。
- 从模型中生成序列化的引擎
- 从构建器生成序列化时序缓存。

有关更多详细信息，请参阅trtexec部分。

### 2.11 Polygraphy

Polygraphy 是一个工具包，旨在帮助在 TensorRT 和其他框架中运行和调试深度学习模型。它包括一个 Python API 和一个使用该 API 构建的命令行界面 (CLI)。

使用Polygraphy，您可以：

- 在多个后端（例如 TensorRT 和 ONNX-Runtime）之间运行推理，并比较结果（例如 API、CLI）。
- 将模型转换为各种格式，例如具有训练后量化功能的 TensorRT 引擎（例如 API、CLI）。
- 查看有关各种类型模型的信息（例如 CLI）
- 允许命令行方式修改 ONNX 模型
  - 提取子图
  - Simplify 和 sanitize
- 隔离TensorRT中的错误策略

有关更多详细信息，请参阅Polygraphy存储库。

## 3. C++ API

本章介绍了 C++ API 的基本用法，假设您从 ONNX 模型开始。SampleOnnxMNIST 更详细地说明了此用例。

当 TensorRT 第一次调用 CUDA 时，如果在此之前不存在 CUDA 上下文，则会自动创建 CUDA 上下文。通常最好在第一次调用 TensorRT 之前自己创建和配置 CUDA 上下文。

为了说明对象的生命周期，本章中的代码没有使用智能指针；但是，建议将它们与 TensorRT 接口一起使用。

### 3.1 构建阶段

要创建构建器，您首先必须实例化 ILogger 接口。此示例捕获所有警告消息，但忽略信息性消息：

```c++
class Logger : public ILogger           
{
    void log(Severity severity, const char* msg) noexcept override
    {
        // suppress info-level messages
        if (severity <= Severity::kWARNING)
            std::cout << msg << std::endl;
    }
} logger;
```

然后可以创建 *IBuilder* 的实例：

```c++
IBuilder* builder = createInferBuilder(logger);
```

#### 3.1.1 创建网络定义

创建构建器后，优化模型的第一步是创建网络定义：

```c++
uint32_t flag = 1U << static_cast<uint32_t>
    (NetworkDefinitionCreationFlag::kEXPLICIT_BATCH); 
// 创建具有使用 flags 参数指定的不可变属性的网络定义对象。 当与 NetworkDefinitionCreationFlag::kEXPLICIT_BATCH 标志一起使用时，CreateNetworkV2 支持动态形状和显式批量尺寸。
INetworkDefinition* network = builder->createNetworkV2(flag);
```

kEXPLICIT_BATCH 将网络标记为显式批处理网络。 动态形状支持需要设置 kEXPLICIT_BATCH 标志。 对于动态形状，任何输入维度都可以在运行时变化，并且网络规范中没有隐式维度。 不同的维度通过使用通配符维度值-1 来指定。

#### 3.1.2 使用 ONNX 解析器导入模型

现在，必须从 ONNX 表示填充网络定义。ONNX解析器API位于文件 NvOnnxParser.h 中，解析器位于NvOnnxParser C++命名空间中。

您可以创建ONNX解析器来填充网络，如下所示：

```c++
IParser* parser = createParser(*network, logger);
```

然后，读取模型文件并处理任何错误:

```c++
parser->parseFromFile(modelFile, 
    static_cast<int32_t>(ILogger::Severity::kWARNING));
for (int32_t i = 0; i < parser.getNbErrors(); ++i)
{
std::cout << parser->getError(i)->desc() << std::endl;
}
```

TensorRT网络定义的一个重要方面是，它包含指向模型权重的指针，这些指针由构建器复制到优化的引擎中。由于网络是使用解析器创建的，解析器拥有权重占用的内存，因此在构建器运行之前不应删除解析器对象。

需要注意，TensorRT 支持的 ONNX 操作有限，对于不在支持列表里的操作，需要用户通过 Plugin 的形式实现。

TensorRT 支持的操作列表为： https://github.com/onnx/onnx-tensorrt/blob/main/docs/operators.md

#### 3.1.3 构建引擎

下一步是创建一个构建器配置，指定TensorRT应该如何优化模型。

```c++
IBuilderConfig* config = builder->createBuilderConfig();
```

此接口有许多属性，您可以设置这些属性来控制 TensorRT 如何优化网络。一个重要的属性是最大工作空间大小。层实现通常需要一个临时工作空间，并且此参数限制了网络中任何层可以使用的最大大小。如果提供的工作空间不足，TensorRT可能无法找到层的实现。默认情况下，工作空间设置为给定设备的总全局内存大小；在必要时限制它，例如，当多个引擎要在一个设备上构建时。

```c++
config->setMemoryPoolLimit(MemoryPoolType::kWORKSPACE, 1U << 20);
```

- kWORKSPACE: TensorRT 使用 kWORKSPACE 来存储操作中的中间缓冲区。 这相当于已弃用的 IBuilderConfig::setMaxWorkspaceSize 并覆盖该值。 默认为最大设备内存。 设置为较小的值以限制集体使用超过阈值的策略。要更有针对性地删除策略，请使用 IAlgorithmSelector。
- kDLA_MANAGED_SRAM: kDLA_MANAGED_SRAM 是 DLA 用于在层内通信的快速软件管理 RAM。 该池的大小必须至少为 4 KiB，并且必须是 2 的幂。默认为 1 MiB。 Orin 每个核心的容量为 1 MiB，Xavier 在其所有加速器核心之间共享 4 MiB。
- kDLA_LOCAL_DRAM: kDLA_LOCAL_DRAM 是 DLA 用于跨操作共享中间张量数据的主机 RAM。 该池的大小必须至少为 4 KiB，并且必须是 2 的幂。默认为 1 GiB。
- kDLA_GLOBAL_DRAM: kDLA_GLOBAL_DRAM 是 DLA 用于存储执行权重和元数据的主机 RAM。 该池的大小必须至少为 4 KiB，并且必须是 2 的幂。默认值为 512 MiB。
- kTACTIC_DRAM: kTACTIC_DRAM 是优化器用来运行策略的设备 DRAM。 在主机和设备内存统一的嵌入式设备上，这包括 TensorRT 构建网络直至每个内存分配点所需的所有主机内存。 当 cudaGetDeviceProperties.embedded 为 true 时，默认为 cudaGetDeviceProperties 报告的 TotalGlobalMem 的 75%，否则为 100%。

一旦指定了配置，就可以构建引擎。

```c++
IHostMemory* serializedModel = builder->buildSerializedNetwork(*network, *config);
```

由于序列化引擎包含权重的必要副本，解析器、网络定义、构建器配置和构建器不再是必要的，可以安全地删除：

```C++
delete parser;
delete network;
delete config;
delete builder;
```

然后可以将引擎保存到磁盘，并且可以删除它序列化到的缓冲区。

```C++
delete serializedModel
```

注意：序列化引擎不可跨平台或TensorRT版本移植。引擎特定于它们所构建的确切GPU模型（除了平台和TensorRT版本）。

由于构建引擎是一个离线过程，因此可能需要相当长的时间。有关如何使生成器运行得更快，请参阅“优化生成器性能”部分。

### 3.2 反序列化 Plan

假设您之前已经序列化了一个优化的模型并希望执行推理，那么您必须创建一个Runtime接口的实例。与生成器一样，运行时需要记录器的一个实例：

```c++
IRuntime* runtime = createInferRuntime(logger);
```

将模型读取到缓冲区后，可以对其进行反序列化以获得引擎：

```c++
ICudaEngine* engine = 
  runtime->deserializeCudaEngine(modelData, modelSize);
```

### 3.3 推理

引擎拥有优化的模型，但为了执行推理，我们必须管理中间激活的附加状态。这是使用 *ExecutionContext* 接口完成的：

```c++
IExecutionContext *context = engine->createExecutionContext();
```

一个引擎可以有多个执行上下文，允许一组权重用于多个重叠的推理任务。（当前的例外情况是使用动态形状时，每个优化配置文件只能有一个执行上下文，除非指定了预览功能kPROFILE_SHARING_0806。）

要执行推理，必须为输入和输出传递 TensorRT 缓冲区，TensorRT 要求您通过调用setTensorAddress 来指定该缓冲区，该缓冲区采用张量的名称和缓冲区的地址。您可以使用为输入和输出张量提供的名称查询引擎，以在数组中找到正确的位置：

```c++
context->setTensorAddress(INPUT_NAME, inputBuffer);
context->setTensorAddress(OUTPUT_NAME, outputBuffer);
```

然后，您可以调用 TensorRT 的 enqueueV3 方法来使用 CUDA 流开始推理：

```c++
context->enqueueV3(stream);
```

网络是否异步执行取决于网络的结构和特性。例如，可能导致同步行为的功能的非详尽列表包括数据相关形状、DLA 使用、循环和同步插件。通常在内核之前和之后使用cudaMemcpyAsync（）将数据传输排队，以从GPU中移动数据（如果数据还没有）。

要确定内核（可能还有 cudaMemcpyAsync()）何时完成，请使用标准 CUDA 同步机制，例如事件或等待流。

## 4. Python API

## 5. TensorRT 工作原理

本章提供有关 TensorRT 如何工作的更多详细信息。

### 5.1 对象生命周期

TensorRT 的 API 是基于类的，某些类充当其他类的工厂。对于用户拥有的对象，工厂对象的生命周期必须跨越它创建的对象的生命周期。例如，*NetworkDefinition* 和 *BuilderConfig* 类是从 Builder 类创建的，并且这些类的对象应在构建器工厂对象之前销毁。

此规则的一个重要例外是从构建器创建引擎。创建引擎后，您可以销毁构建器、网络、解析器和构建配置并继续使用该引擎。

### 5.2 错误处理和日志记录

创建 TensorRT 顶级接口（构建器、运行时或 refitter）时，必须提供 *Logger*（C++、Python）接口的实现。记录器用于诊断和信息性消息；其详细级别是可配置的。由于记录器可用于在 TensorRT 生命周期中的任何时刻传回信息，因此其生命周期必须跨越应用程序中该接口的任何使用。该实现还必须是线程安全的，因为 TensorRT 可能在内部使用工作线程。

对对象的API调用将使用与相应的顶级接口关联的记录器。例如，在对ExecutionContext:：enqueV3（）的调用中，执行上下文是从引擎创建的，而引擎是从运行时创建的，因此TensorRT将使用与该运行时关联的记录器。

错误处理的主要方法是 ErrorRecorder（C++、Python）接口。您可以实现此接口，并将其附加到 API 对象以接收与该对象关联的错误。对象的记录器也将传递给它创建的任何其他记录器 - 例如，如果您将错误记录器附加到引擎，并从该引擎创建执行上下文，它将使用相同的记录器。如果您随后将新的错误记录器附加到执行上下文，它将仅接收来自该上下文的错误。如果生成错误但未找到错误记录器，则会通过关联的记录器发出错误。

### 5.3 内存

TensorRT使用大量的设备内存（即GPU直接访问的内存，而不是连接到CPU的主机内存）。由于设备内存通常是一种受约束的资源，因此了解 TensorRT 如何使用它很重要。

#### 5.3.1 构建阶段

在构建过程中，TensorRT 为计时层实现分配设备内存。某些实现可能会消耗大量临时内存，尤其是对于大张量。您可以通过构建器配置的内存池限制来控制临时内存的最大数量。工作区大小默认为设备全局内存的完整大小，但可以在必要时进行限制。如果构建器发现由于工作空间不足而无法运行的适用内核，它将发出一条日志消息来指示这一点。

然而，即使工作空间相对较小，计时也需要为输入、输出和权重创建缓冲区。TensorRT 对于此类分配返回内存不足的操作系统 (OS) 具有很强的鲁棒性。在某些平台上，操作系统可能会成功提供内存，然后内存不足杀手进程会观察到系统内存不足，并杀死 TensorRT。如果发生这种情况，请在重试之前释放尽可能多的系统内存。

在构建阶段，主机内存中通常至少有两个权重副本：来自原始网络的权重副本以及构建时作为引擎一部分包含的权重副本。此外，当 TensorRT 组合权重（例如卷积与批量归一化）时，将创建额外的临时权重张量。

#### 5.3.2 运行时阶段

在运行时，TensorRT 使用相对较少的主机内存，但可以使用大量的设备内存。

引擎在反序列化时分配设备内存来存储模型权重。由于序列化引擎几乎都是权重，因此其大小非常接近权重所需的设备内存量。

*ExecutionContext*使用两种类型的设备内存：

- 某些层实现所需的持久内存 - 例如，某些卷积实现使用边缘掩码，并且此状态不能像权重一样在上下文之间共享，因为其大小取决于层输入形状，而该形状可能因上下文而异。该内存在创建执行上下文时分配，并在其生命周期内持续有效。
- 暂存存储器，用于在处理网络时保存中间结果。该内存用于中间激活张量。它还用于层实现所需的临时存储，其范围由 *IBuilderConfig::setMemoryPoolLimit()* 控制。

您可以选择使用 *ICudaEngine::createExecutionContextWithoutDeviceMemory()* 创建一个没有临时内存的执行上下文，并在网络执行期间自行提供该内存。这允许您在不同时运行的多个上下文之间共享它，或者在推理未运行时用于其他用途。所需的临时内存量由*ICudaEngine::getDeviceMemorySize()* 返回。

默认情况下，TensorRT 直接从 CUDA 分配设备内存。但是，您可以将 TensorRT 的 *IGpuAllocator*（C++、Python）接口的实现附加到构建器或运行时并自行管理设备内存。如果您的应用程序想要控制所有 GPU 内存并重新分配给 TensorRT，而不是让 TensorRT 直接从 CUDA 分配，这非常有用。

TensorRT 的依赖项（NVIDIA cuDNN 和 NVIDIA cuBLAS）可能会占用大量设备内存。TensorRT 允许您通过使用构建器配置中的 *TacticSources*（C++、Python）属性来控制是否使用这些库进行推理。请注意，某些插件实现需要这些库，因此当排除它们时，网络可能无法成功编译。

CUDA 基础设施和 TensorRT 的设备代码也消耗设备内存。内存量因平台、设备和 TensorRT 版本而异。您可以使用 *cudaGetMemInfo* 来确定正在使用的设备内存总量。

TensorRT 测量构建器和运行时中关键操作前后使用的内存量。这些内存使用统计信息会打印到TensorRT的信息记录器中。例如：

```shell
[MemUsageChange] Init CUDA: CPU +535, GPU +0, now: CPU 547, GPU 1293 (MiB)
```

它指示通过 CUDA 初始化的内存使用变化。CPU +535，GPU +0 是运行 CUDA 初始化后的内存量增加。之后的内容：CUDA 初始化后的 CPU/GPU 内存使用快照。

注意：在多租户的情况下，cudaGetMemInfo 和 TensorRT 报告的内存使用情况容易出现竞争条件，其中新的分配/释放由不同的进程或不同的线程完成。由于 CUDA 无法控制统一内存设备上的内存，因此cudaGetMemInfo 返回的结果在这些平台上可能不准确。

#### 5.3.3 CUDA 延迟加载

CUDA延迟加载是一项CUDA功能，它可以显著降低TensorRT的峰值GPU和主机内存使用率，并加快TensorRT初始化，而对性能的影响可以忽略不计（<1%）。内存使用和初始化时间的节省取决于型号、软件堆栈、GPU平台等。通过设置环境变量CUDA_MODULE_LOADING=LAZY来启用。有关更多信息，请参阅NVIDIA CUDA文档。

#### 5.3.4 L2 级缓存管理



### 5.4 线程

通常，TensorRT对象不是线程安全的；客户端必须序列化来自不同线程的对对象的访问。

预期的运行时并发模型是不同的线程将在不同的执行上下文上操作。上下文包含执行期间的网络状态（激活值等），因此在不同线程中同时使用上下文会导致未定义的行为。

为了支持该模型，以下操作是线程安全的：

- 运行时或引擎上的非修改操作。
- 从 TensorRT 运行时反序列化引擎。
- 从引擎创建执行上下文。
- 注册和注销插件

在不同线程中使用多个构建器不存在线程安全问题；然而，构建器使用计时来确定所提供参数的最快内核，并且使用具有相同 GPU 的多个构建器将扰乱计时和 TensorRT 构建最佳引擎的能力。使用多个线程使用不同的 GPU 进行构建就不存在此类问题。

### 5.5 确定性

TensorRT 构建器使用计时来找到实现给定运算符的最快内核。时序内核会受到噪声的影响-GPU上运行的其他工作、GPU时钟速度的波动等等。时序噪声意味着在生成器的连续运行中，可能无法选择相同的实现。

通常，不同的实现将使用不同顺序的浮点运算，从而导致输出中的微小差异。这些差异对最终结果的影响通常很小。然而，当TensorRT被配置为通过在多个精度上调谐来优化时，FP16和FP32内核之间的差异可能更显著，特别是如果网络没有很好地正则化或对数值漂移敏感。

可能导致不同内核选择的其他配置选项包括不同的输入大小（例如，批量大小）或输入配置文件的不同优化点（请参阅使用动态形状部分）。

*AlgorithmSelector*（C++，Python）接口允许您强制构建器为给定层选择特定的实现。您可以使用它来确保构建器在不同的运行中拾取相同的内核。有关更多信息，请参阅算法选择和可复制构建部分。

在构建了引擎之后，除了 *IFillLayer* 之外，它是确定性的：在相同的运行时环境中提供相同的输入将产生相同的输出。

#### 5.5.1 IFillLayer确定性

当使用 *RANDOM_UNIFORM* 或 *RANDOM_NORMAL* 操作将 *IFillLayer* 添加到网络中时，上述确定性保证不再有效。在每次调用时，这些操作都会根据 RNG 状态生成张量，然后更新 RNG 状态。此状态是在每次执行上下文的基础上存储的。

### 5.6 运行时配置

TensorRT提供了多个运行库来满足各种用例。运行 TensorRT 引擎的 C++ 应用程序应该链接到以下其中一个：

- 默认库（libnvinfer.so）
- 精益运行时库（libnvinfer_lean.so/.dll）比默认库小得多，并且仅包含运行与版本兼容的引擎所需的代码。它有一些限制；主要是，它不能重新装置或序列化引擎。
- libnvinfer_dispatch.so，用于解决兼容性问题

## 6. 高级主题

### 6.1 版本兼容性

### 6.9 显式批处理与隐式批处理

TensorRT支持两种指定网络的模式：显式批处理和隐式批处理。

在隐式批处理模式下，每个张量具有隐式批处理维度，所有其他尺寸必须具有恒定的长度。这种模式是由Tensorrt的早期版本使用的，现在已弃用，但继续支持向后兼容。

在显式批处理模式中，所有维度都是显式的，并且可以是动态的，也就是说，它们的长度可以在执行时更改。许多新功能（如动态形状和循环）仅在此模式下可用。ONNX解析器也需要它。