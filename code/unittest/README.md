# 单元测试

## 断言

 - EXPECT_*(EXPECT_TRUE(), EXPECT_FALSE(), EXPECT_EQ(), EXPECT_NE(), EXPECT_LT(), EXPECT_LE(), EXPECT_GT(), EXPECT_GE())
执行失败,不会导致测试函数返回.
 - ASSERT_*(ASSERT_TRUE(), ASSERT_FALSE(), ASSERT_EQ(), ASSERT_NE(), ASSERT_LT(), ASSERT_LE(), ASSERT_GT(), ASSERT_GE())
执行失败,会导致当前的测试函数立即返回.

## 测试流程

- 调用RUN_ALL_TESTS()

 - 保存所有 googletest 标志的状态

 - 为第一个测试创建一个测试夹具对象

 - 通过 SetUp() 初始化

 - 在夹具对象上运行测试函数

 - 通过 TearDown() 清理夹具

 - 删除夹具

 - 恢复所有 googletest 标志的状态

 - 依次重复执行测试函数

  > [注] 当测试函数中发生致命错误, 会跳过后续步骤, 继续进行下一个测试

## 示例

- [基础测试框架](base)
- [友元测试](friend_test)
