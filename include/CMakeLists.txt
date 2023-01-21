cmake_minimum_required(VERSION 3.0.0)

add_library(spdlog INTERFACE)

set_target_properties(spdlog PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
)
