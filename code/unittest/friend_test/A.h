#ifndef FRIEND_TEST_A_H_
#define FRIEND_TEST_A_H_

#include <stdio.h>
#include <gtest/gtest_prod.h>

namespace trunk {
namespace localization {

class A {
 public:
  int a_c;
  int foo_c(int i) { return i + 1; }

 private:
  int a_e;
  int foo_e(int i) { return i + 1; }

  FRIEND_TEST(ATest1, notfixture);
  FRIEND_TEST(ATest2, isfixture);
};

}  // namespace localization
}  // namespace trunk
#endif  // FRIEND_TEST_A_H_
