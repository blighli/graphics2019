cmake_minimum_required(VERSION 3.15)
project(Project02)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

include_directories(
        ${PROJECT_SOURCE_DIR}
        external/
        external/glad-4.6
        external/glfw-3.3
        external/glm-0.9.9.6
        external/stb_image
)
link_directories(
        lib
)
link_libraries(
        libglfw3.a
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
aux_source_directory(common COMMON_FILES)
set(SOURCE_FILES
        ${COMMON_FILES}
        main.cpp
        glad.c
        )
add_executable(Project02 ${SOURCE_FILES})