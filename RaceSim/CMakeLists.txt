# Minimum version of CMake required
cmake_minimum_required(VERSION 3.12)

Include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

# Project name
project(RaceSim VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD_REQURED 17)

# Add external libraries
add_subdirectory(libs/date)

# Include directories for the target executable
include_directories(    
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/config
    ${CMAKE_CURRENT_SOURCE_DIR}/include/model
    ${CMAKE_CURRENT_SOURCE_DIR}/include/opt
    ${CMAKE_CURRENT_SOURCE_DIR}/include/route
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sim
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/date
)
set(EXTERNAL_DEPS date spdlog::spdlog yaml-cpp)

# Include src files - GLOB bad practice but good enough for this project
file(GLOB RACESIM_SOURCES 
    src/config/*.cpp
    src/model/*.cpp
    src/optimizer/*.cpp
    src/route/*.cpp
    src/simulation/*.cpp
    src/utils/*.cpp
)

# Add the main executable
add_executable(${PROJECT_NAME} ./src/main.cpp ${RACESIM_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${EXTERNAL_DEPS})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR})
