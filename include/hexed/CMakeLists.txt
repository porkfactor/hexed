cmake_minimum_required(VERSION 3.0)

target_sources(hexed PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Album.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Library.hpp
    ${CMAKE_CURRENT_LIST_DIR}/LibraryFormat.hpp
    ${CMAKE_CURRENT_LIST_DIR}/LibraryFormatManager.hpp
    ${CMAKE_CURRENT_LIST_DIR}/LibraryFormatReader.hpp
    ${CMAKE_CURRENT_LIST_DIR}/LibraryFormatWriter.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Playlist.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Track.hpp    
)

add_subdirectory("detail")
add_subdirectory("impl")