cmake_minimum_required(VERSION 3.25.0)

include(FetchContent)

FetchContent_Declare(blessed
    GIT_REPOSITORY "https://github.com/porkfactor/blessed.git"
    GIT_TAG "main"
    GIT_SHALLOW TRUE
    GIT_SUBMODULES ""
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(blessed)
