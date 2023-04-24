# googletest 单元测试框架

## 1. 使用

```cmake
find_package(GTest REQUIRED)
add_executable(test main.cc test.cc)
target_link_libraries(test ${GTEST_BOTH_LIBRARIES})
```

必须提供 main 函数，用于执行 RUN_ALL_TESTS。

```c++
#include <gtest/gtest.h>
int main(int argc, char* argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

## 2. 宏

### 2.1 测试宏

#### 2.1.1 测试用例

```c++
TEST(test case name, test name)
```

#### 2.1.2 夹具

```c++
TEST_F(FixtureClass, test name){}
```

### 2.2 断言

#### 2.2.1 EXPECT 家族

> 测试不通过时继续执行

- EXPECT_TRUE(expression)
- EXPECT_FALSE(expression)
- EXPECT_EQ(expression)
- EXPECT_NE(expression)
- ...

#### 2.2.2 ASSERT 家族

> 测试不通过时立即终止

- ASSERT_TRUE(expression)
- ASSERT_FALSE(expression)
- ASSERT_EQ(expression)
- ASSERT_NE(expression)
- ...

## 3. 示例代码

[googletest exampels](https://github.com/google/googletest/tree/main/googletest/samples)