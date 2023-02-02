cmake_minimum_required(VERSION 3.0)

add_subdirectory("detail")
add_subdirectory("impl")

target_sources(hexed PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ITLFormat.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ITLFormatReader.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MusicDBFormat.hpp
    ${CMAKE_CURRENT_LIST_DIR}/XMLFormat.hpp
)