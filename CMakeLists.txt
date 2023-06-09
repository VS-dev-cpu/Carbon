cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories("include")

option(DEBUG_RENDERER "Enable Debug Renderer" ON)

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

# Include Debug Renderer
if(DEBUG_RENDERER)
    list(APPEND SOURCES
        src/debug/renderer.cpp
        src/GL/glad.c
    )

    list(APPEND LIBRARIES
        glfw
        GL
    )

    add_compile_definitions(DEBUG_RENDERER)
endif()

# Physics Engine Project
project(Carbon VERSION 0.1 LANGUAGES C CXX)

add_library(Carbon ${SOURCES})
target_link_libraries(Carbon ${LIBRARIES})

# Hello World Example
add_subdirectory(hello_carbon)
