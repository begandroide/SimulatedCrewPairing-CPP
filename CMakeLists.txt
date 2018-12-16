cmake_minimum_required(VERSION 3.12)
project(clion)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)

add_executable(clion
        instances/instancia.csv
        "instances/instancia2.csv"
        "instances/instancia3.csv"
        lib/agency.cpp
        lib/agency.hpp
        lib/flight.cpp
        lib/flight.hpp
        lib/individual.cpp
        lib/individual.hpp
        lib/natureV.cpp
        lib/nature.hpp
        lib/population.cpp
        lib/operators.cpp
        lib/operators.hpp
        src/main.cpp)
