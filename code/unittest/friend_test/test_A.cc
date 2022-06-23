#include <gtest/gtest.h>
#include <gtest/gtest_prod.h>
#include <stdio.h>

#include "A.h"

namespace trunk {
namespace localization {

TEST(ATest1, notfixture) {
  A b;
  int x = 3;
  EXPECT_EQ(4, b.foo_e(x));
}

class ATest2 : public testing::Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  A c;
};

TEST_F(ATest2, isfixture) {
  int x = 3;
  EXPECT_EQ(4, c.foo_e(x));
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace localization
}  // namespace trunk
