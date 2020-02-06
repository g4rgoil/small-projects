cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)

project(CellularAutomaton)

set(EXECUTABLE_NAME "cellular-automaton")
add_executable(${EXECUTABLE_NAME} src/application.hpp src/application.cpp src/abstract_game.hpp
    src/conway/universe.hpp src/conway/universe.cpp)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML REQUIRED COMPONENTS graphics window system)
set(SFML_STATIC_LIBRARIES TRUE)

include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
