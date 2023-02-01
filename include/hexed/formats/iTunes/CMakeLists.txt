cmake_minimum_required(VERSION 3.0)

add_subdirectory("detail")

target_sources(hexed PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/itl.hpp
    ${CMAKE_CURRENT_LIST_DIR}/xml.hpp
)