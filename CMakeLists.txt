cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories("include")

project(Carbon VERSION 0.1 LANGUAGES CXX)

add_library(Carbon
    src/Carbon.cpp

    src/types/vector.cpp

    src/collisions.cpp

    src/debug/renderer.cpp
)

target_link_libraries(Carbon glfw GL)

# Hello World Example
add_subdirectory(hello_carbon)
