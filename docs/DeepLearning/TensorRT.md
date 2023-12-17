# TensorRT

## 1. 简介
NVIDIA TensorRT是一款用于高性能深度学习推理的SDK，包括深度学习推理优化器和运行时，可为推理应用提供低延迟和高吞吐量。

### 优势
 - 推理速度提高36倍
基于NVIDIA TensorRT的应用程序在推理过程中的执行速度比仅使用CPU的平台快36倍，使您能够优化在所有主要框架上训练的神经网络模型，通过高精度校准降低精度，并部署到超大规模数据中心、嵌入式平台或汽车产品平台。
 - 优化推理性能
TensorRT基于NVIDIA CUDA®并行编程模型构建，可让您在NVIDIA GPU上使用量化、层和张量融合、内核调优等技术优化推理。
 - 加速每个工作负载 





本指南介绍了TensorRT中可用的基本安装、转换和运行时选项，以及它们的最佳应用时间。

## 2. TenroRT 生态

TensorRT是一个庞大而灵活的项目。它可以处理各种转换 和部署工作流，而哪种工作流最适合您将取决于您的 具体使用情形和问题设置。

TensorRT 提供了多种部署选项，但所有工作流程都涉及将模型转换为优化表示，TensorRT 将其称为引擎。为您的模型构建 TensorRT 工作流程涉及选择正确的部署选项以及用于创建引擎的正确参数组合。

### 2.1 TensorRT 基本工作流

您必须遵循五个基本步骤来转换和部署模型：

- Export The Model
- Select A Batch Size
- Select A Precision
- Convert The Model
- Deploy The Model

在完整的端到端工作流程中理解这些步骤是最容易的：在使用 ONNX 的示例部署中，我们将介绍一个与框架无关的简单部署工作流程，用于使用以下命令将经过训练的 ResNet-50 模型转换并部署到 TensorRT：ONNX 转换和 TensorRT 的独立运行时。

### 2.2 转换和部署选项

TensorRT 生态分为两部分：

1. 用户可以遵循各种路径将其模型转换为优化的 TensorRT 引擎。
2. 在部署优化的TensorRT引擎时，不同的运行时用户可以使用TensorRT作为目标。

### 2.2.1 转换

使用TensorRT转换模型有三个主要选项：

- 使用 TF-TRT
- 从 .onnx 文件自动转换 ONNX
- 使用TensorRT API手动构建网络（使用C++或Python）

为了转换 TensorFlow 模型，TensorFlow 集成 (TF-TRT) 提供模型转换和高级运行时 API，并且能够在 TensorRT 不支持特定运算符的情况下回退到 TensorFlow 实现。有关支持的运算符的更多信息，请参阅 ONNX 运算符支持。

自动模型转换和部署的一个性能更高的选项是使用 ONNX 进行转换。ONNX 是一个与框架无关的选项，可与 TensorFlow、PyTorch 等中的模型配合使用。TensorRT 支持使用 TensorRT API 或 trtexec 从 ONNX 文件自动转换 - 后者是我们将在本指南中使用的。ONNX 转换是全有或全无的，这意味着模型中的所有操作都必须受 TensorRT 支持（或者您必须为不支持的操作提供自定义插件）。ONNX 转换的结果是一个单一的 TensorRT 引擎，它比使用 TF-TRT 的开销更少。

为了获得尽可能高的性能和可定制性，您还可以使用 TensorRT 网络定义 API 手动构建 TensorRT 引擎。这本质上涉及在 TensorRT 操作中仅使用 TensorRT 操作构建与目标模型相同的网络。创建 TensorRT 网络后，您只需从框架中导出模型的权重并将其加载到 TensorRT 网络中。对于这种方法，有关使用 TensorRT 的网络定义 API 构建模型的更多信息，可以在此处找到：

- 使用C++ API从头开始创建网络定义
- 使用Python API从头开始创建网络定义

### 2.2.1 部署

使用 TensorRT 部署模型有以下三种选项：

- 在 TensorFlow 中部署
- 使用独立的 TensorRT 运行时 API
- 使用 NVIDIA Triton 推理服务器

您对部署的选择将决定转换模型所需的步骤。

使用TF-TRT时，最常见的部署选项是简单地在TensorFlow中进行部署。TF-TRT转换会生成一个插入了TensorRT操作的TensorFlow图。这意味着你可以像使用Python运行任何其他TensorFlow模型一样运行TF-TRT模型。

TensorRT 运行时 API 允许最低的开销和最细粒度的控制，但 TensorRT 本身不支持的运算符必须以插件的形式实现（此处提供了预先编写的插件库）。使用运行时 API 进行部署的最常见路径是使用框架中的 ONNX 导出，本指南的下一节将对此进行介绍。

最后，NVIDIA Triton推理服务器是一款开源推理服务软件，使团队能够从任何框架（TensorFlow、TensorRT、PyTorch、ONNX Runtime或自定义框架）、本地存储或谷歌云平台或AWS S3在任何基于GPU或CPU的基础设施（云、数据中心或边缘）上部署经过训练的人工智能模型。这是一个灵活的项目，具有几个独特的功能，例如异构模型和同一模型的多个副本的并发模型执行（多个模型副本可以进一步减少延迟），以及负载平衡和模型分析。如果您必须通过HTTP为模型提供服务，例如在云推理解决方案中，这是一个很好的选择。您可以在此处找到NVIDIA Triton推理服务器主页，并在此处找到文档。

### 2.3 选择正确的工作流

选择如何转换和部署模型的两个最重要的因素是：

1. 框架选择
2. 首选的 TensorRT 运行时目标

以下流程图介绍了本指南中涵盖的不同工作流程。此流程图将帮助您根据这两个因素选择路径。

<img src="/home/zwb/Documents/personal/StudyNotes/docs/Pic/pic19.png" style="zoom: 50%;" />

## 3 使用 ONNX 部署示例

ONNX 转换通常是将 ONNX 模型自动转换为 TensorRT 引擎的最高效的方法。在本节中，我们将在部署预训练 ONNX 模型的背景下逐步完成 TensorRT 转换的五个基本步骤。

在此示例中，我们将使用 ONNX 格式从 ONNX 模型库转换预训练的 ResNet-50 模型；一种与框架无关的模型格式，可以从大多数主要框架（包括 TensorFlow 和 PyTorch）导出。有关 ONNX 格式的更多信息可以在此处找到。

### 3.1 导出模型

TensorRT 转换的两个主要自动路径需要不同的模型格式才能成功转换模型：

- TF-TRT 使用 TensorFlow SavedModels
- ONNX 路径要求模型保存在 ONNX 中。

在本示例中，我们使用 ONNX，因此我们需要一个 ONNX 模型。我们将使用 ResNet-50；可用于多种目的的基本骨干视觉模型。我们将使用 ONNX 模型库中包含的预训练 ResNet-50 ONNX 模型执行分类。

### 3.2 选择批量大小

批量大小会对 TensorRT 在我们的模型上执行的优化产生很大影响。一般来说，在推理时，当我们想要优先考虑延迟时，我们会选择较小的批量大小；当我们想要优先考虑吞吐量时，我们会选择较大的批量大小。较大的批次需要更长的处理时间，但会减少每个样品花费的平均时间。

如果您直到运行时才知道需要什么批量大小，TensorRT 能够动态处理批量大小。也就是说，固定的批量大小允许 TensorRT 进行额外的优化。对于此示例工作流程，我们使用固定批量大小 64。有关处理动态输入大小的更多信息，请参阅动态形状。

### 3.3 选择精度

推理通常需要比训练更少的数字精度。需要注意的是，较低的精度可以在不牺牲任何有意义的精度的情况下加快计算速度并降低内存消耗。TensorRT支持TF32、FP32、FP16和INT8精度。有关精度的详细信息，请参阅降低精度。

### 3.4 模型转换

ONNX 转换方式是自动 TensorRT 转换最通用、性能最高的方式之一。它适用于 TensorFlow、PyTorch 和许多其他框架。

有几个工具可以帮助您将模型从ONNX转换为TensorRT引擎。一种常见的方法是使用trtexec，这是TensorRT中包含的一种命令行工具，可以将ONNX模型转换为TensorRT引擎并对其进行评测。

### 3.5 模型部署

在我们成功创建了TensorRT引擎之后，我们必须决定如何使用TensorRT运行它。

有两种类型的TensorRT运行时：一种是具有C++和Python绑定的独立运行时，另一种是与TensorFlow的本地集成。在本节中，我们将使用一个简化的包装器（ONNXClassifierWrapper），它调用独立运行时。我们将生成一批随机化的“伪”数据，并使用ONNXClassifierWrapper对该批数据进行推理。有关TensorRT运行时的更多信息，请参阅Understanding TensorRT runtimes Jupyter笔记本。

## 4 TF-TRT 框架集成

TF-TRT 集成提供了一种简单灵活的方式来开始使用 TensorRT。TF-TRT 是 TensorRT 的高级 Python 接口，可直接与 TensorFlow 模型配合使用。它允许您将 TensorFlow SavedModel 转换为 TensorRT 优化模型，并使用高级 API 在 Python 中运行它们。

TF-TRT提供了转换路径和Python运行时，允许您像运行任何其他TensorFlow模型一样运行优化模型。这有很多优点，特别是TF-TRT能够通过分析模型并在可能的情况下将子图传递给TensorRT以独立转换为引擎，从而转换包含支持层和不支持层的混合层的模型，而不必创建自定义插件。

## 5 ONNX 转换和部署

ONNX 交换格式提供了一种从许多框架（包括 PyTorch、TensorFlow 和 TensorFlow 2）导出模型的方法，以便与 TensorRT 运行时一起使用。使用 ONNX 导入模型需要 ONNX 支持模型中的运算符，并且您需要提供 TensorRT 不支持的任何运算符的插件实现。（可以在此处找到 TensorRT 插件库）。

### 5.1 使用 ONNX 导出

可以使用 ONNX 项目的 tf2onnx 工具轻松从 TensorFlow 模型生成 ONNX 模型。

## 6 TensorRT 运行时 API 使用



jetpack版本查询
`jtop`

CUDA版本查询
`cat /usr/local/cuda/version.json`

TensorRT版本查询
`dpkg -l |grep TensorRT`



## 参考链接

[1] [NVIDIA Deep Learning TensorRT Documentation](https://docs.nvidia.com/deeplearning/tensorrt/quick-start-guide/index.html#introduction)
