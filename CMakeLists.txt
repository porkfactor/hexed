cmake_minimum_required(VERSION 3.0.0)

project(cursed VERSION 1.0.0)

add_subdirectory("submodules")
#add_subdirectory("include")
add_subdirectory("itl")

#temporary bodge 
add_library(blessed INTERFACE)
set_target_properties(blessed PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(hexed INTERFACE)
set_target_properties(hexed PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOUCE_DIR)
endif()
