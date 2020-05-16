cmake_minimum_required(VERSION 3.9)
project(ktest_example)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS --coverage)

include_directories(../include)
add_executable(ktest_example main.cpp)
target_link_libraries(ktest_example ktest)

