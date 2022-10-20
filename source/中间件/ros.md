# ros 问题整理

## 1. CPack & Catkin

**问题描述**

同时允许cmake 及 catkin 编译时，CPack 打包引入额外文件

```cmake
# CMakeLists.txt
find_package(catkin)
```

**解决方案**

[Remove Catkin artifacts from CPack build](https://stackoverflow.com/questions/50486421/remove-catkin-artifacts-from-cpack-build)