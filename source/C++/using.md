# Using关键字
对C++中 using 关键字的用法总结如下:
 - 类型别名
 - 使用 using 指示引入完整的命名空间
 - 使用 using 声明引入另一个命名空间的成员

## 类型别名

使用 using 代替 typedef 优势，详见[T.43 core guideline](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t43-prefer-using-over-typedef-for-defining-aliases):

- 可读性更好
- 通用性(using可用于模板别名)

```c++
using Horsepower = int;
using Iterator = std::vector<std::string>::iterator;
template <typename T>
using MapT = std::map<int, T>;
```

## 使用 using 命令

>  [不要在头文件的全局命令空间中使用 using namespace](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#sf7-dont-write-using-namespace-at-global-scope-in-a-header-file)

```c++
// bad.h
#include <iostream>
using namespace std;  // bad
```

## 使用 using 声明

```c++
#include <string>
using std::string;
```

## 参考链接

[1] [using usage](https://www.sandordargo.com/blog/2022/04/27/the-4-use-of-using-in-cpp)
