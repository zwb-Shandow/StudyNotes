# Catch2 单元测试框架

## 1. 使用

Catch2 提供两类库，一类为利用框架自带 main 函数运行单元测试，另一类为自定义 main 函数运行单元测试；Catch2 同时支持 find_package 及 pkg-config 找包方式。

注意，若自定义 main 函数，则必须链接 Catch2::Catch2 库，否则必须链接 Catch2::Catch2WithMain，此处是针对 find_package 而言；若使用 pkg-config 找包，则通过 catch2.pc 与 catch2-with-main.pc 区分。

- find_package

```cmake
find_package(Catch2 REQUIRED)
add_executable(test test.cc)
target_link_libraries(test Catch2::Catch2WithMain)
# 若需自定义 main 函数，则链接如下库
# target_link_libraries(test Catch2::Catch2)
```

- pkg-config

```cmake
find_package(PkgConfig REQUIRED)
pkg_check_modules(CATCH REQUIRED catch2-with-main)
# pkg_check_modules(CATCH REQUIRED catch2)
include_directories(${CATCH_INCLUDE_DIRS})
link_directories(${CATCH_LIBRARY_DIRS})

add_executable(test test.cc)
target_link_libraries(test ${CATCH_LIBRARIES})
```

> 依赖 Catch2 编译的单元测试节点自带命令行属性

## 2. 宏

### 2.1 测试宏

#### 2.1.1 测试用例

```c++
TEST_CASE(test name, [,tags]){}  // tag 标签主要用于命令行工具
```

特殊 tags:

- [.] : 跳过当前测试用例
- [!throws] : 
- [!mayfail]
- [!shouldfail]
- [!nonportable]
- [#<filename>]
- [@<alias>]
- [!benchmark]

#### 2.1.2 夹具

```c++
SECTION(section name, [, section description]){}
TEST_CASE_METHOD(FixtureClass, method name, [, tags])
```

- 可通过 TEST_CASE 与 SECTION 组合实现夹具功能

```c++
TEST_CASE("vectors can be sized and resized", "[vector]") {
  // For each section, vector v is anew:

  std::vector<int> v(5);

  REQUIRE(v.size() == 5);
  REQUIRE(v.capacity() >= 5);

  SECTION("resizing bigger changes size and capacity") {
    v.resize(10);

    REQUIRE(v.size() == 10);
    REQUIRE(v.capacity() >= 10);
  }
  SECTION("resizing smaller changes size but not capacity") {
    v.resize(0);

    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() >= 5);
  }
}
```

注意，嵌套层数请不要超过3层

- 类似 googletest 夹具

```c++
class UniqueTestsFixture {
  private:
   static int uniqueID;
  protected:
   DBConnection conn;
  public:
   UniqueTestsFixture() : conn(DBConnection::createConnection("myDB")) {
   }
  protected:
   int getID() {
     return ++uniqueID;
   }
 };

 int UniqueTestsFixture::uniqueID = 0;

 TEST_CASE_METHOD(UniqueTestsFixture, "Create Employee/No Name", "[create]") {
   REQUIRE_THROWS(conn.executeSQL("INSERT INTO employee (id, name) VALUES (?, ?)", getID(), ""));
 }
 TEST_CASE_METHOD(UniqueTestsFixture, "Create Employee/Normal", "[create]") {
   REQUIRE(conn.executeSQL("INSERT INTO employee (id, name) VALUES (?, ?)", getID(), "Joe Bloggs"));
 }
```

### 2.2 断言宏

#### 2.2.1 REQUIRE 家族

> 测试不通过时会立即终止

- REQUIRE(expression)
- REQUIRED_FALSE(expression)
- REQUIRE_NOTHROW(expression)
- REQUIRE_THROWS(expression)
- ...

#### 2.2.2 CHECK 家族

> 测试不通过时会继续执行

- CHECK(expression)
- CHECK_FALSE(expression)
- CHECK_NOTHROW(expression)
- CHECK_THROWS(expression)
- ...

### 2.3 日志宏

> 日志只在对应 case 失败时打印。

```c++
TEST_CASE("Qux") {
    INFO("First info");
    UNSCOPED_INFO("First unscoped info");
    CHECK(false);

    INFO("Second info");
    UNSCOPED_INFO("Second unscoped info");
    CHECK(false);
}
```

- INFO(message expression) : 当前 case 失败，所有 INFO 都会打印
- UNSCOPED_INFO(message expression) : 只记录 case 中最新失败断言的 INFO
- WARN(message expression) : 无论 case 失败与否，必会打印
- FAIL(message expression) : 记录消息并立即终止 case
- CAPTURE(expression1, expression2, ...) : 捕获并记录中间变量

## 3. 数据和参数驱动测试

### 3.1 数据生成器

```c++
TEST_CASE("Generators") {
    auto i = GENERATE(1, 2);
    SECTION("one") {
        auto j = GENERATE(-3, -2);
        REQUIRE(j < i);
    }
    SECTION("two") {
        auto k = GENERATE(4, 5, 6);
        REQUIRE(i != k);
    }
}
```

Catch2 支持自定义 GENERATE。

## 4. 基准测试

### 4.1 简单用法

```c++
#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>

std::uint64_t Fibonacci(std::uint64_t number) {
  return number < 2 ? 1 : Fibonacci(number - 1) + Fibonacci(number - 2);
}

TEST_CASE("Fibonacci") {
  CHECK(Fibonacci(0) == 1);
  CHECK(Fibonacci(5) == 8);

  BENCHMARK("Fibonacci 20") { return Fibonacci(20); };
  BENCHMARK("Fibonacci 25") { return Fibonacci(25); };
  BENCHMARK("Fibonacci 30") { return Fibonacci(30); };
  BENCHMARK("Fibonacci 35") { return Fibonacci(35); };
}
```

### 4.2 高级用法

## 5. 命令行工具使用

### 5.1 测试报告生成

```shell
--reporter JUnit::out=result-junit.xml --reporter XML::out=test.xml
```

目前支持8种报告模板:

- XML
- Automake
- compact
- console
- JUnit
- SonarQube
- TAP
- TeamCity

Catch2 支持自定义 reporter。

## 参考链接

[1] [Catch2](https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md)