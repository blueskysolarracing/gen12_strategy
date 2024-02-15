cmake_minimum_required(VERSION 3.12)
project(pugixml)
message("Pugixml src files: " ${CMAKE_CURRENT_SOURCE_DIR}/src/pugixml.cpp)

add_library(pugixml ${CMAKE_CURRENT_SOURCE_DIR}/src/pugixml.cpp)
target_include_directories(pugixml PUBLIC include)
