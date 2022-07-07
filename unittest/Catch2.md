# Catch2 单元测试框架

## 1. 使用

Catch2 提供两类库，一类为利用框架自带 main 函数运行单元测试，另一类为自定义 main 函数运行单元框架；Catch2 同时支持 find_package 及 pkg-config 找包方式。

注意，若自定义 main 函数，则必须链接 Catch2 库，否则编译出错。

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

## 2. 宏

### 2.1 测试宏

#### 2.1.1 测试用例

```c++
TEST_CASE(test name, [,tags]){}  // tag 标签主要用于命令行工具
```

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

### 2.2 断言

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

## 参考链接

[1] [Catch2](https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md)