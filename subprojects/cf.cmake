include (FetchContent)

FetchContent_Declare(cf
    GIT_REPOSITORY "https://github.com/apple-oss-distributions/CF.git"
    GIT_TAG "CF-1153.18"
    GIT_SHALLOW TRUE
    GIT_SUBMODULES ""
)

FetchContent_MakeAvailable(cf)