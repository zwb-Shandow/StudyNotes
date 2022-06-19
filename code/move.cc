/* g++ move.cc -fno-elide-constructors */

#include <iostream>
#include <string>

class Container {
 public:
  using Resource = std::string;

  Container() {
    resource_ = new Resource;
    std::cout << "defautl constuctor." << std::endl;
  }

  explicit Container(const Resource& resource) {
    resource_ = new Resource(resource);
    std::cout << "explicit constuctor." << std::endl;
  }

  ~Container() {
    delete resource_;
    std::cout << "destructor" << std::endl;
  }

  Container(const Container& rhs) {
    resource_ = new Resource(*(rhs.resource_));
    std::cout << "copy constuctor." << std::endl;
  }

  // 深拷贝构造
  Container& operator=(const Container& rhs) {
    delete resource_;
    resource_ = new Resource(*(rhs.resource_));
    std::cout << "copy assignment." << std::endl;
    return *this;
  }

  // 移动构造
  Container(Container&& rhs) : resource_(rhs.resource_) {
    rhs.resource_ = nullptr;
    std::cout << "move constuctor." << std::endl;
  }

  Container& operator=(Container&& rhs) {
    Resource* tmp = resource_;
    resource_ = rhs.resource_;
    rhs.resource_ = tmp;
    std::cout << "move assignment." << std::endl;
    return *this;
  }

 private:
  Resource* resource_ = nullptr;
};

Container get() {
  Container ret("tag");
  return ret;
}

int main(int argc, char* argv[]) {
  // Container foo;
  Container foo = get();
  return 0;
}
