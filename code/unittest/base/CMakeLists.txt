cmake_minimum_required(VERSION 3.15)

project(Test)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(
  ${PROJECT_NAME}
  main.cc
  test_vector.cc
  test_queue.cc
)

target_link_libraries(
  ${PROJECT_NAME}
  ${GTEST_BOTH_LIBRARIES}
)
