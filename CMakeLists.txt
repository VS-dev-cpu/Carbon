cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(glfw3 REQUIRED)

include_directories("include")

link_libraries(glfw)

project(Carbon VERSION 0.1.0)

add_library(Carbon
    src/Carbon.cpp

    src/types/vector.cpp
)

# Hello World Example
add_subdirectory(hello_carbon)
