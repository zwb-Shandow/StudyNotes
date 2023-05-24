# 编译器

## 常用编译选项

### -fdump-lang-class
转储C++类的内部表示，加上这个参数后，编译器会在编译过程中输出类的内部相关信息，比如继承关系，虚函数表等细节。

```C++
class A {
  public:
    A() = default;
    virtual ~A() = default;
    
    virtual void funcA() {}
    virtual void funcB() {}
    void funcC() {}
  private:
    int a_;
    int b_;
};

int main(int argc, const char **argv) {
    A a;
    return 0;
}
```

生成类的内存布局

- gcc

```bash
g++ -fdump-lang-class test.cc
```

- clang

```bash
# 输出内存布局
clang -Xclang -fdump-record-layouts -stdlib=libc++ -c test.cc
# 输出虚函数表的布局
clang -Xclang -fdump-vtable-layouts -stdlib=libc++ -c test.cc
```

类内存布局如下，clang编译器打印的信息更加友好:

```
*** Dumping AST Record Layout
         0 | class A
         0 |   (A vtable pointer)
         8 |   int a_
        12 |   int b_
           | [sizeof=16, dsize=16, align=8,
           |  nvsize=16, nvalign=8]

Vtable for 'A' (6 entries).
   0 | offset_to_top (0)
   1 | A RTTI
       -- (A, 0) vtable address --
   2 | A::~A() [complete]
   3 | A::~A() [deleting]
   4 | void A::funcA()
   5 | void A::funcB()
```

- offset_to_top (0)：表明这个虚表地址距离对象顶部的偏移量，因为在对象头部所以是0。
- RTTI 指针：虚表第一个元素,指针指向了对象运行时类型信息(type_info)的地址，用于运行时的类型识别，用于 typeid 和 dynamic_cast
- A::~A() [complete]：虚表指针指向的元素，因为对象可以在栈内和堆内创建，本虚构函数对应栈内创建的实例，只需要调用析构函数不用 delete 相应的内存
- A::~A() [deleting]：那么带 deleteing 的虚构函数对应堆内创建的实例，需要调用析构函数和delete相应的内存，释放资源
