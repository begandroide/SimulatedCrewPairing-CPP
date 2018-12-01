cmake_minimum_required(VERSION 3.12)
project(fly CXX)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")

add_executable(out/fly src/main.cpp lib/individual.cpp lib/individual.hpp)