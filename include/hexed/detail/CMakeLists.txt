cmake_minimum_required(VERSION 3.0)

target_sources(hexed PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Compression.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Encryption.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MemoryMappedFile.hpp
)
