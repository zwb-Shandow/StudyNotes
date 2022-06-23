#include <iostream>
#include <string>
#include <thread>
#include <utility>

class Data {
 public:
  std::string name = "zhangsan";
  int age = 25;
};

/**
 * 创建GetInstance的模板,可将某个对象变为单例,
 * 常用于参数配置
 * 懒汉模式
 */
class Singleton {
 public:
  template <typename T, typename... Args>
  static T& GetInstance(Args... args) {
    static T value(std::forward<Args>(args)...);
    return value;
  }

  template <typename U>
  static U& GetInstance() {
    static U value;
    return value;
  }

  // 删除拷贝构造
  Singleton(Singleton const&) = delete;
  Singleton& operator=(Singleton const&) = delete;

  // 删除移动构造
  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&&) = delete;

 private:
  Singleton();
  ~Singleton();
};

void test1() {
  auto& data = Singleton::GetInstance<Data>();
  std::cout << "name1: " << data.name << "\n"
            << "age1: " << data.age << std::endl;
}

void test2() {
  auto& data = Singleton::GetInstance<Data>();
  std::cout << "name2: " << data.name << "\n"
            << "age2: " << data.age << std::endl;
}

int main() {
  std::thread thread1(test1);
  std::thread thread2(test2);
  thread1.join();
  thread2.join();

  return 0;
}
