include(FetchContent)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/cache/_deps" CACHE PATH "Custom FetchContent directory" FORCE)

option(LUAU_BUILD_CLI "Build CLI" OFF)
option(LUAU_BUILD_TESTS "Build tests" OFF)
option(LUAU_BUILD_WEB "Build Web module" OFF)
option(LUAU_WERROR "Warnings as errors" OFF)
option(LUAU_STATIC_CRT "Link with the static CRT (/MT)" OFF)
option(LUAU_EXTERN_C "Use extern C for all APIs" OFF)
option(LUAU_BUILD_SHARED "Build as a shared library" OFF)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype
    GIT_TAG VER-2-14-3
    FIND_PACKAGE_ARGS
)
FetchContent_Declare(
    glfw3
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 3.4
    FIND_PACKAGE_ARGS
)
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.5
    FIND_PACKAGE_ARGS
)
FetchContent_Declare(
    cglm
    GIT_REPOSITORY https://github.com/recp/cglm.git
    GIT_TAG v0.9.6
    FIND_PACKAGE_ARGS
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    FIND_PACKAGE_ARGS
)
FetchContent_Declare(
    luau
    GIT_REPOSITORY https://github.com/luau-lang/luau.git
    GIT_TAG 0.735
    FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(freetype)
FetchContent_MakeAvailable(glfw3)
FetchContent_MakeAvailable(cglm)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(luau)

FetchContent_MakeAvailable(googlebenchmark)