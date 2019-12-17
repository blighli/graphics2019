cmake_minimum_required(VERSION 3.15)
project(Project01)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall")


# include
include_directories(${PROJECT_SOURCE_DIR}/include)

# glm
include_directories(${PROJECT_SOURCE_DIR}/3rd-party/glm/)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/glm)

# glfw
include_directories(${PROJECT_SOURCE_DIR}/3rd-party/glfw-3.3/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/glfw-3.3/)

# glad
include_directories(${PROJECT_SOURCE_DIR}/3rd-party/glad-4.6/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/glad-4.6/)


aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)

add_executable(Project01 ${SRC})

target_link_libraries(Project01  glfw glad)


