cmake_minimum_required(VERSION 3.25.0)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Library.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LibraryFormat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LibraryFormatManager.cpp
)

add_subdirectory(formats)

add_executable(tombed
    ${SOURCES}
)

target_include_directories(tombed PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(tombed PROPERTIES
    CXX_STANDARD 11
)

target_link_libraries(tombed PUBLIC
    blessed
    hexed

    $<LINK_LIBRARY:FRAMEWORK,CoreFoundation>
)
