cmake_minimum_required(VERSION 3.5.1)

project(BM)
set(CMAKE_CXX_STANDARD 14)

find_package(benchmark REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(absl REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} benchmark.cc)
target_link_libraries(
  ${PROJECT_NAME}
  benchmark::benchmark
  absl::strings
  ${Boost_LIBRARIES}
)
