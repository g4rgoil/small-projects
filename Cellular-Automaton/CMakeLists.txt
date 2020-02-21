cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)

project(CellularAutomaton)

set(EXECUTABLE_NAME "cellular-automaton")
add_executable( ${EXECUTABLE_NAME}
    src/application.hpp src/application.cpp
    src/fps_counter.hpp src/fps_counter.cpp
    src/util.hpp src/util.cpp
    src/abstract_game.hpp src/glad.c
    src/conway/conways-game.hpp src/conway/conways-game.cpp)

target_include_directories( ${EXECUTABLE_NAME} PRIVATE include )

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH} )

find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries( ${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} )

find_package( glfw3 3.3 REQUIRED )
target_link_libraries( ${EXECUTABLE_NAME} glfw )

find_package( SFML REQUIRED COMPONENTS graphics window system )
set( SFML_STATIC_LIBRARIES TRUE )

find_package( Freetype REQUIRED )
target_link_libraries( ${EXECUTABLE_NAME} ${FREETYPE_LIBRARIES} )
target_include_directories( ${EXECUTABLE_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})

include_directories( ${SFML_INCLUDE_DIR} )
target_link_libraries( ${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} )
