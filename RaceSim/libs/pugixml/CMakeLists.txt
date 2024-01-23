cmake_minimum_required(VERSION 3.12)
project(pugixml)
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/pugixml.cpp)
message("Pugixml src files: " ${SOURCES})

add_library(pugixml ${SOURCES})
target_include_directories(pugixml PUBLIC include)
