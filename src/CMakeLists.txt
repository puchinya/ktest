cmake_minimum_required(VERSION 3.9)
project(ktest)

set(CMAKE_CXX_STANDARD 11)

include_directories(../include)
add_library(ktest STATIC ktest.cpp)
