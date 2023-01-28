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

## 2. diagnostic

[diagnostic_analysis](http://wiki.ros.org/diagnostic_analysis)

## 3. boost占位符未定义

**问题描述**

- ros16.04 依赖 boost1.58，占位符为 `_1`
- ros18.04 依赖 boost1.60，占位符为 `boost::placeholders::_1`

**解决方案**

```c++
#include <boost/version.hpp>
#if BOOST_VERSION >= 106000
	using boost::placeholders::_1;
	using boost::placeholders::_2;
	using boost::placeholders::_3;
#endif
```

参考链接: [https://github.com/freeorion/freeorion/issues/3172](https://github.com/freeorion/freeorion/issues/3172)

