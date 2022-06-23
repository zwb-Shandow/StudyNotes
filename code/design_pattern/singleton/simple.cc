#include <string>
#include <chrono>
#include <thread>
#include <iostream>

/**
 * The Singleton class defines the 'GetInstance' method that servers as an
 * alternative to constructor and lets clients access the same instance of
 * this class over and over.
 */
class Singleton {
  /**
   * The Singleton's constructor should always be private to prevent direct
   * construction calls with the 'new' operator.
   */
 protected:
  Singleton(const std::string value) : value_(value) {}

  static Singleton* singleton_;

  std::string value_;

 public:
  /**
   * Singletons should not be cloneable.
   */
  Singleton(Singleton&) = delete;
  /**
   * Singletons should not be assignable.
   */
  void operator=(const Singleton&) = delete;
  /**
   * This is the static method that controls the access to the singletion
   * instance. On the first run, it creates a singleton object and places it
   * into the static field. On subsequent runs, it returns the client existing
   * object stored in the static field.
   */
  static Singleton* GetInstance(const std::string& value);
  /**
   * Finally, any singleton should define some business logic, which can be
   * executed on its instance.
   */
  void SomeBusinessLogic() {}

  std::string value() const { return value_; }
};

Singleton* Singleton::singleton_ = nullptr;

/**
 * Static methods should be defined outside the class.
 */
Singleton* Singleton::GetInstance(const std::string& value) {
  /**
   * This is a safer way to create an instance. instance = new Singleton is
   * dangeruous in case two instance threads wants to access at the same time
   */
  if (singleton_ = nullptr) {
    singleton_ = new Singleton(value);
  }
  return singleton_;
}

void ThreadFoo() {
    // Following code emulates solw initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton* singleton = Singleton::GetInstance("FOO");
    std::cout << singleton->value() << std::endl;
}

void ThreadBar() {
    // Following code emulates solw initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton* singleton = Singleton::GetInstance("BAR");
    std::cout << singleton->value() << std::endl;
}

int main() {
    ThreadFoo();
    /* std::thread t1(ThreadFoo); */
    /* std::thread t2(ThreadBar); */
    /* t1.join(); */
    /* t2.join(); */

    return 0;
}
