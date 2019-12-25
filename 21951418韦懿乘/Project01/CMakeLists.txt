# This will become a big project one day.
# I believe it will happen with all my heart.
cmake_minimum_required(VERSION 3.14)
project(render-engine)

set(CMAKE_CXX_STANDARD 14)

file(GLOB engine_src
        "src/basic/*.cpp"
        "src/engine/*.cpp"
        "src/objects/*.cpp"
)

add_executable(engine ${engine_src})

target_include_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/src)

#------------------------------------------------------------------------------#
# Below is the settings for 3rd party libraries

# glm
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/glm-0.9.9.5/include/")
add_subdirectory(./3rd-party/glm-0.9.9.5/)

# stb_image
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/stb_image/include/")
add_subdirectory(./3rd-party/stb_image/)

# glfw
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/glfw-3.3/include/")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(./3rd-party/glfw-3.3/)

# glad
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/glad/include/")
add_subdirectory(./3rd-party/glad/)

# assimp
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/assimp/include/")
add_subdirectory(./3rd-party/assimp/)

# imgui
include_directories("${PROJECT_SOURCE_DIR}/3rd-party/imgui/")
add_subdirectory(./3rd-party/imgui)

target_link_libraries(imgui glad glfw)
target_link_libraries(engine assimp imgui glad glfw stb_image glm)
