cmake_minimum_required(VERSION 3.0)

target_sources(hexed PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/buffer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/envelope.hpp
    ${CMAKE_CURRENT_LIST_DIR}/header.hpp
    ${CMAKE_CURRENT_LIST_DIR}/hohm.hpp
    ${CMAKE_CURRENT_LIST_DIR}/section.hpp
)