# Minimum version of CMake required
cmake_minimum_required(VERSION 3.12)

# Project name
project(RaceSim)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include all other source files from src/ directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

message(STATUS "CPP files to link: ${SOURCES}")

# Add the main executable
add_executable(RaceSim ${SOURCES})

target_include_directories(RaceSim PRIVATE include)
