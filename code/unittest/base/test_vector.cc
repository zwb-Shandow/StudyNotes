#include <gtest/gtest.h>

#include "fixture.hpp"

TEST_F(VectorTest, PushBack) {
  // 虽然这里修改了vec, 但对其他测试函数是不可见的
  vec.push_back(4);
  EXPECT_EQ(vec.size(), 4);
  EXPECT_EQ(vec.back(), 4);
}

TEST_F(VectorTest, Size) { EXPECT_EQ(vec.size(), 3); }
