cmake_minimum_required(VERSION 3.15)
project(Project04)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D FREEGLUT_STATIC")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/box2d-master/include)
link_directories(${PROJECT_SOURCE_DIR}/box2d-master/cmake-build-debug/src)


INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(Project04 main.cpp src/glad.c include/Vertex.h include/Object.cpp include/Object.h)

target_link_libraries(Project04 freeglut_static opengl32 winmm gdi32 glu32.lib glfw3.dll glew32.dll libbox2d.a)