cmake_minimum_required(VERSION 3.25.0)

include(FetchContent)

FetchContent_Declare(cursed
    GIT_REPOSITORY "git@github.com:porkfactor/cursed.git"
    GIT_TAG "develop"
    GIT_SHALLOW FALSE
    GIT_SUBMODULES ""
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(cursed)
