#include <iomanip>
#include <iostream>
#include <vector>

class Line {
 public:
  explicit Line(double x, double y, double z) : x_(x), y_(y), z_(z) {
    std::cout << "constructor" << std::endl;
  };

  ~Line() { std::cout << "desc" << std::endl; }

  Line(const Line& a) { std::cout << "copy constructor" << std::endl; }

  Line(Line&& a) { std::cout << "move constructor" << std::endl; }

 private:
  double x_;
  double y_;
  double z_;
};

int main(int argc, char* argv[]) {
  Line line(1, 2, 3);
  std::vector<Line> vec;
  vec.emplace_back(std::move(line));

  return 0;
}
