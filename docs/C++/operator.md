# operator

## 为什么比较运算符重载要被声明为友元函数？

运算符重载通常被定义为类的成员函数,但是比较运算符(==, !=, <, >, <=, >=)的重载却通常被定义为友元函数。这是因为:

1. 比较运算符需要访问类的私有成员,而友元函数可以访问类的私有成员。如果定义为普通的成员函数,则无法访问私有成员。

2. 比较运算符的第一个参数是该类的左值,如果定义为成员函数的话,该左值会自动转化为this指针,而友元函数不会发生这种转化。

例如:

```c++
class Date {
 public:
  explicit Date(int year, int month, int day)
      : year_(year), month_(month), day_(day) {}

  friend bool operator==(const Date& lhs, const Date& rhs) {
    return lhs.year_ == rhs.year_ && lhs.month_ == rhs.month_ &&
           lhs.day_ == rhs.day_;
  }

 private:
  int year_;
  int month_;
  int day_;
};

```

这里`operator==`被定义为友元函数,可以访问Date的私有成员,并且接收两个Date左值作为参数。

如果定义为成员函数,则必须写成:

```c++
class Date {
public:
    bool operator==(const Date& d) const {
        return year_ == d.year_ && month_ == d.month_ && day_ == d.day_;    
    }
};
```

这样无法访问Date的私有成员,并且第二个参数d会自动转化为this,失去了左值的意义。所以总结来说,比较运算符重载之所以常常定义为友元函数主要有两个原因:

1. 访问私有成员

2. 保留左值的语义

这符合比较运算符最基本的要求,所以理解这一点对运算符重载有很好的指导作用。