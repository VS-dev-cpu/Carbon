cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories("include")

# Source Files
set(SOURCES
    # Core
    src/Carbon.cpp

    # Utility
    src/utility/collisions.cpp

    # Types
    src/types/vector.cpp
    src/types/matrix.cpp
    src/types/colliders.cpp
    src/types/types.cpp

    # Debug
    src/debug/log.c
)

# Libraries
set(LIBRARIES dl)

# Physics Engine Project
project(Carbon VERSION 0.1 LANGUAGES C CXX)

add_library(Carbon ${SOURCES})
target_link_libraries(Carbon ${LIBRARIES})

# Hello World Example
add_subdirectory(hello_carbon)
