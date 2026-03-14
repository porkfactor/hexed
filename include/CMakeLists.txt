cmake_minimum_required(VERSION 3.25.0)

add_library(hexed INTERFACE)
target_include_directories(hexed INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(hexed INTERFACE
    $<BUILD_INTERFACE:blessed>
    z
    spdlog
)

add_subdirectory("hexed")
