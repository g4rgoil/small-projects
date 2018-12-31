cmake_minimum_required(VERSION 3.13)
project(3D_Puzzle)

set(CMAKE_CXX_STANDARD 17)

add_executable(3d-puzzle include/help.hpp
        include/puzzle.hpp src/puzzle.cpp
        include/print.hpp src/print.cpp
        include/solve.hpp src/solve.cpp)

