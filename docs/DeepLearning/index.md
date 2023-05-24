# 深度学习

深度学习从训练到推理的完整体系如下：

![](../Pic/pic19.png)



<img src="/home/trunk/Documents/personal/StudyNotes/docs/Pic/深度学习.jpg" style="zoom:80%;" />

## 开源深度学习框架

### TensorFlow

由Google开发，是一个功能强大的开源深度学习框架，支持各种类型的机器学习和深度学习任务

### PyTorch

由Facebook开发，以动态计算图为基础，受到研究人员和实践者的喜爱，易于使用且具有灵活性

### Caffe

一个由伯克利视觉与学习中心开发的深度学习框架，主要用于计算机视觉任务



以上框架均可实现模型的构建，模型训练及前向推理部分，可指定只用CPU或GPU，但是推理效率较低.

## 深度学习推理引擎

### TensorRT

TensorRT（Tensor Runtime）是由NVIDIA开发的一个高性能深度学习推理引擎，旨在优化和加速深度学习模型的推理阶段。虽然深度学习框架本身已经能够进行模型训练和推理，但TensorRT的存在有以下几个重要原因：

1. **推理性能优化：** 深度学习框架在训练阶段通常会优化模型以提高准确性，但这可能导致模型在推理阶段变得复杂，影响性能。TensorRT 专门用于优化推理过程，包括对网络结构进行裁剪、融合和量化等，以减少推理所需的计算资源，提高速度和吞吐量。
2. **硬件加速：** TensorRT充分利用NVIDIA GPU的硬件加速功能，如Tensor Core和 FP16 计算，以实现更快的推理速度。它能够在GPU上执行高度优化的计算，从而在保持准确性的前提下提供更高的性能。
3. **低延迟需求：** 在许多应用中，特别是实时性要求较高的场景，低延迟的推理是至关重要的。TensorRT通过其优化技术可以显著减少推理延迟，使其在实时应用中表现出色。
4. **移动和嵌入式平台：** 移动设备和嵌入式系统通常资源有限，因此需要高效的推理引擎。TensorRT在这些平台上的性能优势使其成为推理的理想选择。
5. **灵活性：** TensorRT支持各种深度学习框架和模型格式，这使得将训练好的模型部署到TensorRT上变得相对容易。同时，它也提供了Python和C++的API，使开发者能够轻松集成和部署模型。

总之，虽然深度学习框架在训练和推理方面都能提供很大的灵活性和功能，但TensorRT专注于深度学习模型推理的性能优化和硬件加速，特别适合对速度和延迟有较高要求的场景，如实时视频分析、自动驾驶等

### OpenVINO

由Intel开发的推理引擎，旨在优化和加速视觉相关任务的深度学习模型推理。它支持多种硬件平台，包括Intel CPU、GPU、FPGA 和 VPU。

## 并行计算平台与编程模型

### CUDA

### OpenCL

## 参考链接

[1] [Deep Learning Training vs. Inference: What’s the Difference?](https://www.xilinx.com/applications/ai-inference/difference-between-deep-learning-training-and-inference.html)

[2] [Accelerating Inference in TensorFlow with TensorRT User Guide](https://docs.nvidia.com/deeplearning/frameworks/tf-trt-user-guide/index.html)
