cmake_minimum_required(VERSION 3.0)

add_library(blessed INTERFACE)
target_include_directories(blessed INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_library(hexed INTERFACE)
target_include_directories(hexed INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(hexed INTERFACE
    z
    spdlog
)

add_subdirectory("blessed")
add_subdirectory("hexed")