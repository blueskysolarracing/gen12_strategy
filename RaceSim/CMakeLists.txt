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
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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

add_library(${PROJECT_NAME} SHARED ${RACESIM_SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTERNAL_DEPS})

# Add the main executable
add_executable(sim ./src/main.cpp)
target_link_libraries(sim PRIVATE ${EXTERNAL_DEPS} ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR})

enable_testing()

add_executable(GeographyTest tests/GeographyTest.cpp)
target_link_libraries(GeographyTest gtest gtest_main ${EXTERNAL_DEPS} ${PROJECT_NAME})
add_test( GeographyTest GeographyTest )

add_executable(TimeTest tests/TimeTest.cpp)
target_link_libraries(TimeTest gtest gtest_main ${EXTERNAL_DEPS} ${PROJECT_NAME})
add_test( TimeTest TimeTest )

install(TARGETS sim DESTINATION ${CMAKE_SOURCE_DIR})