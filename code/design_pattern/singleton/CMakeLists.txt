cmake_minimum_required(VERSION 3.15)

project(Singleton)

set(CMAKE_CXX_STANDARD 14)

set("-wall -g")

find_package(Threads)

add_executable(${PROJECT_NAME}_simple simple.cc)
add_executable(${PROJECT_NAME}_threadsafe threadsafe.cc)

target_link_libraries(${PROJECT_NAME}_simple
    ${CMAKE_THREAD_LIBS_INIT}
    )

target_link_libraries(${PROJECT_NAME}_threadsafe
    ${CMAKE_THREAD_LIBS_INIT}
    )
