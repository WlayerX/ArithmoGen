cmake_minimum_required(VERSION 3.10)
project(ArithmoGen VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Threading support
find_package(Threads REQUIRED)

# Include directories
include_directories(include)

# Source files
add_executable(ArithmoGen
    src/main.cpp
    src/Solver.cpp
    src/Expression.cpp
)

# Link libraries
target_link_libraries(ArithmoGen PRIVATE Threads::Threads)

# Compiler options
if(MSVC)
    target_compile_options(ArithmoGen PRIVATE /W4 /O2)
else()
    target_compile_options(ArithmoGen PRIVATE -Wall -Wextra -O3 -march=native)
endif()
