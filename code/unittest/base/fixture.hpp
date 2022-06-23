#include <gtest/gtest.h>

#include <vector>

/**
 * @brief The fixture for testing class Demo.
 */
class DemoTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  DemoTest() {
    // You can do set-up work for each test here.
  }

  ~DemoTest() override {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up and
  // cleaning up each test, you can define the following methods:

  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test.)
  }

  void TearDown() override {
    // Code here will be called immediately agter each test (right
    // before the destructor.)
  }

  // Class members declared here can be used by all tests in the test suite
  // for Demo.
};

template <typename T>
class Queue {
 public:
  Queue();
  void Enqueue(const T& element);
  T* Dequeue();
  size_t size() const;
};

/**
 * @brief queue fixture
 */
class QueueTest : public ::testing::Test {
 protected:
  void SetUp() override {
    q0_.Enqueue(1);
    q1_.Enqueue(2);
    q2_.Enqueue(3);
  }

  Queue<int> q0_;
  Queue<int> q1_;
  Queue<int> q2_;
};

/**
 * @brief vector fixture
 */
class VectorTest : public ::testing::Test {
 protected:
  virtual void SetUp() override {
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
  }

  virtual void TearDown() override { vec.clear(); }

  std::vector<int> vec;
};
