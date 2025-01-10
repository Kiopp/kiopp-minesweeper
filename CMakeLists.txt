# Setup
cmake_minimum_required(VERSION 3.31.2)
project(KioppMinesweeper C)
set(CMAKE_BUILD_TYPE Release)

# Setup raylib
include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

# Print raylib variables (for debugging)
FetchContent_GetProperties(raylib)
message(STATUS "raylib_POPULATED: ${raylib_POPULATED}")
message(STATUS "raylib_SOURCE_DIR: ${raylib_SOURCE_DIR}")
message(STATUS "raylib_BINARY_DIR: ${raylib_BINARY_DIR}")

# Include directory
set(INCLUDE_DIR inc)

# Source files
set(SOURCES
    src/main.c
    src/Button.c
)

# Create executable
add_executable(kiopp_mine ${SOURCES})

# Specify include directories
target_include_directories(kiopp_mine PUBLIC ${INCLUDE_DIR})

# Link with raylib
target_link_libraries(kiopp_mine PRIVATE raylib) 