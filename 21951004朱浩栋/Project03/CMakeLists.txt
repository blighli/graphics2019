cmake_minimum_required(VERSION 3.15)
project(Project01)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D FREEGLUT_STATIC")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/assimp-3.3.1//include)
link_directories(${PROJECT_SOURCE_DIR}/assimp-3.3.1/cmake-build-debug/code)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)


add_executable(Project01 main.cpp src/glad.c include/Vertex.h)

target_link_libraries(Project01 freeglut_static opengl32 winmm gdi32 glu32.lib glfw3.dll glew32.dll libassimpd.dll libassimpd.dll.a)