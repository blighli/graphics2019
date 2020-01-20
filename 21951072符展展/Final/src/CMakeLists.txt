cmake_minimum_required(VERSION 3.15)
project(tt)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

include_directories(
        ${PROJECT_SOURCE_DIR}
        external/
        external/glad-4.6
        external/glm-0.9.9.6
        external/stb_image
)

# glfw
include_directories(external/glfw-3.3/include)
add_subdirectory(external/glfw-3.3)
# assimp
include_directories(external/assimp-3.3.1/include/)
add_subdirectory(external/assimp-3.3.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
aux_source_directory(common COMMON_FILES)
set(SOURCE_FILES
        ${COMMON_FILES}
        main.cpp
        glad.c)
add_executable(tt ${SOURCE_FILES})
target_link_libraries(tt glfw assimp)