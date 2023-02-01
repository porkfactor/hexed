cmake_minimum_required(VERSION 3.0)

target_sources(hexed PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Album.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Library.hpp
    ${CMAKE_CURRENT_LIST_DIR}/LibraryFormat.hpp
    ${CMAKE_CURRENT_LIST_DIR}/LibraryFormatManager.hpp
)