cmake_minimum_required(VERSION 3.0.0)

project(cursed VERSION 1.0.0)

add_subdirectory("submodules")
add_subdirectory("include")
add_subdirectory("src")

#temporary bodge 

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOUCE_DIR)
endif()
