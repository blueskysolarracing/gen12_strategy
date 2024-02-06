# Minimum version of CMake required
cmake_minimum_required(VERSION 3.12)

Include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(spdlog)

# Project name
project(RaceSim VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include src files - GLOB bad practice but good enough for this project
file(GLOB_RECURSE SOURCES "src/*.cpp")
message(STATUS "RaceSim src files: ${SOURCES}")

# Add the main executable
add_executable(RaceSim ${SOURCES})
target_link_libraries(RaceSim PRIVATE pugixml date spdlog::spdlog)

# Add external libraries
add_subdirectory(libs/date)
add_subdirectory(libs/pugixml)

# Include directories
target_include_directories(RaceSim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/config
    ${CMAKE_CURRENT_SOURCE_DIR}/include/model
    ${CMAKE_CURRENT_SOURCE_DIR}/include/opt
    ${CMAKE_CURRENT_SOURCE_DIR}/include/route
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sim
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/date
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/pugixml/include
)
