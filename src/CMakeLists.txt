cmake_minimum_required(VERSION 3.0)

add_executable(tombed
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

set_target_properties(tombed PROPERTIES
    CXX_STANDARD 11
)

target_link_libraries(tombed PUBLIC
    blessed
    hexed
)