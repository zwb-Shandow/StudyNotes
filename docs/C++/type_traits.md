# A quick primer on type traits in modern C++

类型特性(Type Traits)是C++提供的一组工具，用于在编译期间检查和操作类型的特性。

它允许我们以编程的方式检查类型的某些属性,并根据属性结果进行相应的处理。这可以提高代码的灵活性和可重用性。

例如，给定一个具有类型特征的泛型类型T，它可以是`int`、`bool`、`std:：vector`或任何你想要的类型，你可以问编译器一些问题：它是整数吗？这是一个函数吗？它是指针吗？或者是类吗？它有析构函数吗？可拷贝吗？会抛出异常吗。。。等等。这在条件编译中非常有用，在条件编译中将指示编译器根据输入中的类型选择正确的路径。我们很快就会看到一个例子。

类型特征也可以对类型进行一些转换。例如，给定T，您可以添加/删除常量说明符、引用或指针，或者将其转换为有符号/无符号类型和许多其他疯狂的操作。在编写使用模板的库时非常方便。

这些技术的美妙之处在于，一切都发生在编译时，没有运行时惩罚：毕竟这是模板元编程。我想您对本文其余部分的C++模板有所了解。如果你没有，这本指南是一个很好的介绍。

## 什么是类型特征？

类型特征是一个简单的模板结构，它包含一个成员常量，该常量反过来保存类型特征所问问题的答案或它所执行的转换。例如，让我们看看`std:：is_floating_point`，它是C++标准库在`＜type_traits＞`标头中定义的众多类型特征之一：

```c++
template <typename T>
struct is_floating_point;
```

这个类型特征告诉类型T是否是浮点型。根据作为模板参数传入的类型，成员常量（被称为询问问题的类型特征的值）将被设置为true或false。

另一方面，例如`std:：remove_reference`是一种类型特征，它会改变输入中的类型T：

```c++
template <typename T>
struct remove_reference;
```

这种类型特征基本上将T&转化为T。成员常量（对于那些修改类型的类型特征称为类型）包含转换的结果。

## 如何使用类型特征？

只需使用所需的类型实例化模板结构，然后检查其成员常量并采取相应行动。例如，假设您只想打印出类型是否为浮点：

```c++
#include <iostream>
#include <type_traits>

class Class {};

int main(int argc, char* argv[]) {
  std::cout << std::is_floating_point<Class>::value << std::endl;
  std::cout << std::is_floating_point<float>::value << std::endl;
  std::cout << std::is_floating_point<int>::value << std::endl;
  return 0;
}

```

在上面的代码段中，您将向模板结构`std:：is_floating_point`传递三种不同的类型：一种自定义Class类型、一个float和一个int。编译器和任何常规模板一样，将在后台为您生成三种不同结构：

```c++
struct is_floating_point_Class {
    static const bool value = false;
}
struct is_floating_point_float {
    static const bool value = true;
}
struct is_floating_point_int {
    static const bool value = false;
}
```

在这一点上，只需要读取编译器创建的结构中的`value`成员。由于是静态的，您必须使用`::`语法访问成员常量。请记住，这是模板元编程，所以一切都发生在编译时。

## 类型特征的使用

### 条件汇编

既然我们已经掌握了类型特征背后的思想，让我们试着在一些现实世界的场景中使用它们。假设同一算法有两个函数：一个用于有符号整数，另一个用于无符号整数。您希望编译器在传递int时选择有符号的，在传递无符号的int时选择无符号的，以利用编译器优化。这是我之前提到的条件编译。

对于这项任务，我将使用三种工具：

- C++17 `if constexpr`语法：一个在编译时工作的if语句；

- C++11 `static_assert`函数，顾名思义，如果不满足条件，它会在编译时触发断言；

- 两个自解释的类型特征:`std::is_signed`和`std::is_unsigned`.

代码如下:

```c++
#include <assert.h>
#include <iostream>
#include <type_traits>

void algorithm_signed(int i) {}
void algorithm_unsigned(unsigned u) {}

template <typename T>
void algorithm(T t) {
  if constexpr (std::is_signed<T>::value)
    algorithm_signed(t);
  else if constexpr (std::is_unsigned<T>::value)
    algorithm_unsigned(t);
  else
    static_assert(std::is_signed<T>::value || std::is_unsigned<T>::value,
                  "Must be signed or unsigned!");
}

int main(int argc, char* argv[]) {
  algorithm(3);
  unsigned x = 3;
  algorithm(x);
  algorithm("hello");
  return 0;
}

```

换句话说，模板函数算法充当了一个调度器：当实例化时，编译器会根据传入的类型T抓取正确的函数。如果是`signed`，则会包含`algorithm_signed`；如果是`unsigned`，则会包含`algorithm_unsigned`。最后，如果类型不符合标准，则抛出一个静态断言（即生成错误）。

### 改变类型

类型特征也用于将转换应用于类型。这个魔术的典型用法来自C++标准库和`std:：move`实用函数，它将类型`T`转换为右值引用`T&&`。这是一个重要的操作，为移动语义铺平了道路。

在内部，`std:：move`使用`std:：remove_reference`类型特性从输入中的类型中去掉`&`（如果有的话），并返回`T&&`。一种可能的实施方式：

```c++
template <typename T>
typename remove_reference<T>::type&& move(T&& arg) {
    return static_cast<typename remove_reference<T>::type&&>(arg);
}
```

像这样的转换在整个标准库中广泛存在，通常用于优化函数参数在嵌套模板函数调用中的流动方式。总的来说，这些类型特征中的一些对于普通C++项目来说很少有用，除非你正在编写库或执行一些巧妙的元编程技巧。

在代码中到处读取`::value`和`::type`是令人困惑的。幸运的是，C++14及更高版本引入了简化的语法，这要归功于一些分别以`_v`和`_t`结尾的助手别名。例如：

```c++
std::is_signed<T>::value;        /* ---> */     std::is_signed_v<T>;
std::remove_const<T>::type;      /* ---> */     std::remove_const_v<T>;
```

这些帮助程序适用于查询类型或对其应用转换的所有类型特征。

## 更多类型特征琐事和进一步阅读

类型特征是许多C++特性的基础，和往常一样，在本文中我几乎没有触及它们的表面。以下是未来值得更多关注的其他话题列表。

### 类型特征知识的来源

类型特征是如何了解一个类型的？例如，它如何推断`std:：is_signed_v<T>`对于`int`是真的？大多数基本类型特征都是来自C++黑暗角落的模板元编程技巧、`SFINAE`、标记调度和其他技术的结果。

有些类型特征需要额外的帮助。例如，`std:：is_abstract`类型trait（它告诉一个类型是否为抽象类）不能单独使用模板元编程生成。因此，开发标准库的开发人员会使用编译器提供的特殊内置函数**intrinsics:**，由于编译器对其输入的程序有深入的了解，可以对所讨论的类型有更多的了解。更多信息请点击此处。

### 类型特征和概念

概念是C ++ 20中的重要补充：一种优雅而表达的方式来对模板功能或类可以介入的类型进行约束。例如，在上面的条件汇编示例中，我可以使用概念而不是触发最后的静态断言。毫不奇怪，概念基于标准库中定义的众多类型特征。有关概念的更多信息。

### 类型特征提供内省

内省是程序检查对象的类型或属性的能力。例如，通过内省，您可以询问对象是否有特定的成员函数来调用它。

C++不能在运行时进行内省，但正如我们在本文中看到的，由于类型特征，它在编译时做得很好。在前面的例子中，当我们检查T是否被签名时，我们确实使用了编译时自省。

另一方面，反射是指程序观察和改变其自身结构或行为的能力。目前C++中还没有这样的东西，但一些编程艺术家正在利用类型特征与模板元编程相结合的力量，开发诸如magic_get之类的疯狂库。还有一些建议将反思包括在现代C++中，在这里和这里起草。时间会证明。。。

## 参考链接

[1] [A quick primer on type traits in modern C++](https://www.internalpointers.com/post/quick-primer-type-traits-modern-cpp)