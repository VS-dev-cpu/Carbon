cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Source Files
set(SOURCES
    # Core
    src/Carbon.cpp

    # Utility
    src/utility/collisions.cpp

    # Types
    src/types/colliders.cpp
    src/types/types.cpp

    # Debug
    src/debug/log.c
)

# Libraries
set(LIBRARIES mathutil)

# Physics Engine Project
project(Carbon VERSION 0.1 LANGUAGES C CXX)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(mathutil)

# Hello World Example
add_subdirectory(hello_carbon)
