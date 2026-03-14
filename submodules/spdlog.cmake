cmake_minimum_required(VERSION 3.25.0)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY "git@github.com:gabime/spdlog.git"
    GIT_TAG "v1.16.0"
    GIT_SHALLOW TRUE
    GIT_SUBMODULES ""
)

FetchContent_MakeAvailable(
    spdlog
)
