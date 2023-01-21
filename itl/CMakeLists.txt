cmake_minimum_required(VERSION 3.0)

add_executable(clam itl.cpp)

set_target_properties(clam PROPERTIES
    CXX_STANDARD 11
)

target_link_libraries(clam PRIVATE
    hexed
    blessed
    spdlog
    z
)
