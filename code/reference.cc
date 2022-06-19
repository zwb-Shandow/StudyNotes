#include <iostream>

void process_value(int& i) {
  std::cout << "lvalue processed: " << i << std::endl;
}

void process_value(const int& i) {
  std::cout << "const lvalue processed: " << i << std::endl;
}

void process_value(int&& i) {
  std::cout << "rvalue processed: " << i << std::endl;
}

int main(int argc, char* argv[]) {
  int a = 1;
  const int b = 2;
  process_value(a);
  process_value(b);
  process_value(3);
  return 0;
}
