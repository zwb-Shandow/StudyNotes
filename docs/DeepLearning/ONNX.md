# ONNX(Open Nerual Network Exchange)
ONNX是一种开源的神经网络交换格式,用于在不同的机器学习框架之间交换模型。

ONNX文件不仅仅存储了神经网络模型的权重，同时也存储了模型的结构信息以及网络中每一层的输入输出和一些其它的辅助信息，ONNX 是使用 Protobuf 数据结构存储信息的。

## 特征

- 跨框架:ONNX定义了一种标准的模型表示方法,使得不同的框架(如TensorFlow、PyTorch、MXNet等)训练出的模型可以转换为ONNX格式,然后加载到其他框架中运行。
- 跨平台:ONNX模型可以在不同的硬件平台上运行,如PC、移动设备、嵌入式设备等。
- 跨语言:各种语言都可以加载并运行ONNX模型,如Python、C++、Java等。
- 优化工具:ONNX提供了模型优化、转换工具,可以优化模型结构、做量化、模型压缩等。
- 丰富的操作集:ONNX支持大量常见的神经网络操作,使得许多模型可以表示为ONNX。
- 开源社区:ONNX由Facebook和Microsoft合作发起,拥有广泛的用户和开发者社区。

使用 ONNX 的优点包括:

- 模型交换:可以轻松地在不同框架之间交换模型。
- 优化和部署:可以使用ONNX提供的工具对模型进行优化,并部署到不同的平台。
- 起点和终点:ONNX可以作为模型训练到部署的中间表示。
- 标准化:使用标准的ONNX格式,可以减少不同框架和平台之间的兼容性问题。

## ONNX 格式分析

ONNX 是一个开放规范，由以下组件组成:

- 一个可扩展计算图模型的定义
- 标准数据类型定义
- 内置运算定义

以下仅展示 onnx.proto 中定义了几个核心对象：

- `ModelProto`
  - ModelProto 是一种顶级文件/容器格式，用于捆绑 ML 模型并将其计算图与元数据相关联。
- `GraphProto`
  - GraphProto 定义了模型的计算逻辑，由参数化的节点列表组成，这些节点根据其输入和输出形成有向无环图。这相当于许多深度学习框架中的“网络”或“图”。
- `NodeProto`
- `ValueInfoProto`
- `TensorProto`
- `AttributeProto`

## 可视化

```
snap install netron
netron model.onnx
```

## 参考链接

[1] [ONNX](https://zhuanlan.zhihu.com/p/346511883)
