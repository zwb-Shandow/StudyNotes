#include <iostream>
#include <utility>

struct W {
  W(int&, int&) { std::cout << "(左值引用，左值引用)" << std::endl; }
};

struct X {
  X(const int&, int&) { std::cout << "(常量左值引用，左值引用)" << std::endl; }
};

struct Y {
  Y(int&, const int&) { std::cout << "(左值引用，常量左值引用)" << std::endl; }
};

struct Z {
  Z(const int&, const int&) {
    std::cout << "(常量左值引用，常量左值引用)" << std::endl;
  }
};

// 泛型函数
template <typename T, typename A1, typename A2>
T* factory(A1&& a1, A2&& a2) {
  return new T(std::forward<A1>(a1), std::forward<A2>(a2));
}

int main() {
  int a = 4, b = 5;
  W* pw = factory<W>(a, b);
  X* px = factory<X>(2, b);
  Y* py = factory<Y>(a, 2);
  Z* pz = factory<Z>(2, 2);

  delete pw;
  delete px;
  delete py;
  delete pz;

  return 0;
}
