# iterator

迭代器是一个指向容器中某个元素的对象。像指针一样，迭代器可以用来访问它所指向的元素，并且可以在容器的内容中移动。C++标准库中的每个容器都提供了自己的迭代器，以及一些检索迭代器的方法。使用迭代器很容易：从容器中获取一个实例，将其移动到需要的地方，然后获取指向的元素。

具体地说，迭代器是一个简单的类，它提供了一堆运算符：增量++、解引用*和其他一些运算符，这些运算符使它与指针和可以对其执行的算术运算非常相似。事实上，迭代机是指针的泛化，在编写迭代器本身时，这些运算符经常被用作基础。

迭代器是标准库容器的基石之一,但当您想为自己编写的自定义容器提供遍历元素的能力时,它们也很有用:这就是我在本文中要研究的内容。向您的容器添加迭代器将使它们与[基于范围的for循环](https://en.cppreference.com/w/cpp/language/range-for)和[C++算法库](https://en.cppreference.com/w/cpp/algorithm)兼容:这是一个基于迭代器的用于搜索、排序、计数和操作容器的函数集合。

## 实现迭代器

### 自定义容器

在深入挖掘之前，让我们定义一个简单的容器，我们想用迭代器来增加趣味性：

```c++
class Integers {
    private:
    	int m_data_[200];
};
```

Integers类是一个围绕int原始数组的包装器：我们希望能够通过迭代器访问该私有数组的元素，以及对其进行循环或将其传递给任何标准库算法。让我们从做一些设计决策开始。

### 自定义迭代器

#### 选择迭代器类型

第一步是选择要实现的迭代器的类型。现代C ++定义了六种类型：

| Name               | Description                                                  |
| ------------------ | ------------------------------------------------------------ |
| 输入迭代器         | 只能进行读取操作,一次一个元素,并且不保证多次读取同一个元素。典型例子是`std::istream_iterator` |
| 输出迭代器         | 只能进行写入操作,一次一个元素,并且不保证多次写入同一个元素。典型例子是`std::ostream_iterator` |
| 前向迭代器         | 可以进行读取和写入操作,并且能多次读取/写入同一个元素。它支持`++`运算符,可以按顺序遍历容器中的元素。典型例子是`std::list::iterator` |
| 双向迭代器         | 继承自前向迭代器,同时支持`--`运算符,可以按顺序和逆序遍历容器中的元素。典型例子是`std::list::iterator` |
| 随机访问迭代器     | 继承自双向迭代器,同时支持跳跃式访问,通过[n]运算可以直接访问第n个元素。支持所有指针所支持的运算(>,<,+, '-')。典型例子是`std::vector::iterator` |
| 连续随机访问迭代器 | 继承自随机访问迭代器,要求其所指向的内存是连续的,并且支持指针的算术运算以及每个元素的大小一致。典型例子是原生数组的迭代器 |

这6种迭代器类型形成了一个层次,每种迭代器都继承自更低一级的迭代器,拥有更强大的功能。选择合适的迭代器类型对自定义容器的性能至关重要。

输入和输出迭代器通常用于低级别组件，如输入和输出流（所谓的单程算法），因此具有局限性。我们想对我们的自定义容器做更多的工作，所以我们将跳过这两个，直接跳到可变的Forward Iterator。

**迭代器通常在它所属的类内部声明**，例如：

```c++
class Integers {
    public:
    	struct Iterator {};
};
```

第一件事是为迭代器分配一些属性。在C++ 17之前，这是通过使用标签调度机制进行标记来完成的，而C ++ 20使用概念：在本文中，我将遵循传统方法。

C++ 对迭代器有一定的要求和期望:

- `iterator_category` - 迭代器所属的迭代器类别,比如 `std::input_iterator_tag`, `std::forward_iterator_tag` 等。这用于编译期选择最高效的算法
- `difference_type` - 用于表示两个迭代器之间的距离的有符号整数类型。通常使用 `std::ptrdiff_t`
- `value_type` - 迭代器所遍历的元素类型
- `pointer` - 用于指向 `value_type` 的类型的指针。通常使用 `value_type*`
- `reference` - 用于引用 `value_type` 的类型的引用。通常使用 `value_type&`

代码实现为:

```c++
#include <iterator>  // for std::forward_iterator_tag
#include <cstddef>   // for std::ptrdiff_t

struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = int;
    using pointer           = int*;
    using reference         = int&;
};

// struct Iterator {
//     typedef std::forward_iterator_tag iterator_category;
//     typedef std::ptrdiff_t            difference_type;
//     typedef int                       value_type;
//     typedef int*                      pointer;
//     typedef int&                      reference;
// };
```

`using` 和 `typedef` 都是类型别名，都可以用于给类型起别名。`typedef` 现在主要用于向后兼容老代码，而 `using` 在大多数情况下都优于 `typedef`，所以，在定义迭代器的时候，使用 `using` Defines 会更好。

#### 定义的标签的作用

上面的一些标签一开始可能看起来毫无用处。事实上，您会注意到，在迭代器的定义过程中，它们永远不会被提及。如果容器被传递到`<algorithm>`库中的一个标准库函数，则标记用于选择最有效的算法。错误的标签意味着性能不理想！迭代器类别还用于设置算法要求，例如`std:：fill`想要一个前向迭代器，而`std:：reverse`想要一个双向迭代器。传递错误的迭代器将导致编译错误。这是因为算法对迭代器有一定期望,而这些期望就是通过tag进行描述的。

#### 定义迭代器构造函数

所有迭代器都必须是可构造的、可复制的、可分配的、可破坏的和可交换的。让我们将这些需求转化为迭代器的代码：

```c++
struct Iterator {
    // Iterator tag here ...
  public:
    Iterator(pointer ptr) : m_ptr_(ptr) {}
    
  private:
    pointer m_ptr_;
};
```

很简单，我们只需要一个自定义构造函数来初始化私有成员变量`m_ptr_`，它指向`Integers`容器的一个元素。自定义构造函数满足可构造的要求，而所有其他构造函数都由编译器友好地提供的隐式声明的构造函数和运算符覆盖。

#### 运算符实现

我们正在构建一个可变的`Forward Iterator`，它继承了`Input`和`Output Iterator`。生成的迭代器必须支持以下操作：

- `*iterator` 与 `iterator->`: 可解引用，获取它指向的值；
- `++iterator` 与 `iterator++`: 可递增的，向前移动一步，包括前缀和后缀版本。后者必须返回一些可解引用的东西；
- `iterator_a == iterator_b` 与 `iterator_a != iterator_b`： 与其他迭代器比较；

这是通过在`Iterator`类中实现一些自定义运算符来完成的，例如：

```c++
struct Iterator {
  public:
    // Iterator tag here ...
    
    // Iterator constructors here ...
    
    // * / ->
    reference operator*() { return *m_ptr_; }
    pointer operator->() { return m_ptr_; }
    // 前置自增
    Iterator& operator++() {
        m_ptr_++;
        return *this;
    }
    // 后置自增
    Iterator operator++(int) {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    // 比较运算符
    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
        return lhs.m_ptr_ == rhs.m_ptr_;
    }
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
        return lhs.m_ptr_ != rhs.m_ptr_;
    }
    
  private:
    pointer m_ptr_;
}
```

正如您所看到的，每个操作符都涉及到私有指针`m_ptr_`的使用。另外，请注意这两个比较运算符的友元声明（[设置友元的原因](../operator.md)）：这是将运算符定义为非成员函数的方便方法，同时还可以访问`Iterator`类的私有部分。

### 准备容器

我们的迭代器很好。最后一步是使我们的自定义容器能够创建迭代对象。这是通过添加两个返回迭代类别类的`begin()`和`end()`的公共方法来完成的，分别代表第一个和最后一个元素：

```c++
class Itergers {
  public:
    
    // Iterator definition here ...
    
    Iterator begin() { return Iterator(&m_data_[0]); }
    Iterator end() { return Iterator(&m_data_[200]); }  // 200已经越界
}
```

`end()` 方法返回一个迭代器，该迭代器指向原始数组末尾之后的无效内存地址。 这样的迭代器只是一个占位符，用于确定何时到达边界：永远不应该直接访问它。

### 测试迭代器

自定义容器及其迭代器现已准备就绪。让我们用基于范围的for循环测试它们：

```c++
Integers integers;
for (auto i : integers) {
    std::cout << i << std::endl;
}
```

此代码将神奇地打印容器中每个整数的值。它之所以有效，是因为基于范围的for循环只是编译器为以下内容创建的语法糖：

```c++
for (auto it = integers.begin(), end = integers.end(); it != end(); ++it) {
    const auto i = *it;
    std::cout << i << std::endl;
}
```

换句话说：创建了两个迭代器`it`和`end`。第一个指向容器的开始，另一个指向容器结束。然后，在每个循环中，`it`迭代器都会递增，直到它等于`end`，也就是说，直到到达容器的末尾。实际值是在打印之前通过在局部变量中解引用来获得的。

现在让我们尝试一下`Algorithm`库中的一个函数，例如`std::fill`：

```c++
Integers integers;
std::fill(integers.begin(), integers.end(), 3);
```

该函数为容器中的所有元素分配值3。它之所以有效，是因为`std:：fill`通常是这样实现的:

```c++
template <typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
    for (; first != last; ++ first) {
        *first = value;
    }
}
```

请注意，我们的迭代器并不适用于`Algorithm`库中的所有函数。例如，我们不能将它传递给`std:：reverse`，因为它需要一个双向迭代器。到目前为止，困难的部分已经完成，所以扩展迭代器现在只需要向类中添加更多的运算符，并选择最好的标记来描述它。

## 总结

### 迭代器的行为就像指针：难道我就不能直接使用它们吗？

我们的自定义容器是一个老式数组的包装器，可以使用指针算术进行导航。事实上，我们可以去掉整个`Iterator`类，只需分别从`Integers::begin()`和`Integers::end()`方法返回一个指向第一个和最后一个数组元素的指针。基于范围的循环和算法库中的函数仍然可以正常工作。然而，现实世界中的容器通常基于比普通数组更复杂的数据结构——比如链表或哈希表，其中指针及其操作还不够。迭代器抽象了对象背后的所有复杂性，使其行为像指针一样方便使用，并允许您使用熟悉的操作访问复杂的数据结构。

### 使迭代器不可变

默认情况下，迭代器可以更改它所指向的元素。如果你想让它不可变，常见的技巧是向自定义容器类中添加另一个迭代器类型-让我们称之为`ConstantIterator`。这个新的迭代器类型与原始迭代器几乎相同，只是它的解引用运算符现在返回一个常量引用：

```c++
const reference operator*() const { return *m_ptr_; }
```

这同样适用于->运算符。最后，自定义容器必须能够返回这样的新迭代器类型。这是通过添加两个额外的公共方法`cbegin()`和`cend()`来完成的（其中前导c代表常量），这两个方法返回`ConstantIterator`类的实例：

```c++
ConstantIterator cbegin() const { return ConstantIterator(&m_data_[0]); }
ConstantIterator cend() const { return ConstantIterator(&m_data_[200]); }
```

许多标准库容器同时提供`begin()/end()`和`cbegin()/cend()`对。对每个迭代器类型应用相同的模式。例如，`std:：array`也有`rbegin()/rend()`，其中r代表反向迭代器（是的，您也可以反向循环标准库数组）。

### 如何在C++20中定义迭代器？

C++20引入了`concepts`，这是一种对模板函数或类可以接受的类型进行约束的智能方法。虽然迭代器类别和属性保持不变，但更改的是您如何实施它们：在C++17之前使用`tags`，在C++20之后使用`concepts`。例如，您可以用`std::forward_iterator`概念来标记迭代器，而不是`std:：forward_iterator_tag`标记。这同样适用于所有迭代器属性。例如，前向迭代器必须是`std:：incrementable`。这种新机制有助于获得更好的迭代器定义，并使编译器的错误可读性更强。一旦概念实现变得更加广泛，我就会升级这篇文章。

## 参考链接

[1] [Writing a custom iterator in modern C++](https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp)