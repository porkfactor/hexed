cmake_minimum_required(VERSION 3.25)

list(APPEND SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/buffer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/file.cpp
    ${CMAKE_CURRENT_LIST_DIR}/file.hpp
    ${CMAKE_CURRENT_LIST_DIR}/segment.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ITLFormat.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ITLFormat.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ITLFormatReader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ITLFormatReader.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Log.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MusicDbFormat.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MusicDbFormat.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MusicDbFormatReader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MusicDbFormatReader.hpp
)

set(SOURCES ${SOURCES} PARENT_SCOPE)
