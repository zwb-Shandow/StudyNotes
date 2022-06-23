cmake_minimum_required(VERSION 3.15)
project(friend_test)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} test_A.cc)

target_link_libraries(${PROJECT_NAME}
    ${GTEST_BOTH_LIBRARIES})
