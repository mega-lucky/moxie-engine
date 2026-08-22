cmake_minimum_required(VERSION 3.23)
project(MoxieEngine)

include(FetchContent)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/cache/_deps" CACHE PATH "Custom FetchContent directory" FORCE)

# add_library(asan_flags INTERFACE)
# target_compile_options(asan_flags INTERFACE -fsanitize=address -fno-omit-frame-pointer)
# target_link_options(asan_flags INTERFACE -fsanitize=address)

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

FetchContent_MakeAvailable(freetype)
FetchContent_MakeAvailable(glfw3)
FetchContent_MakeAvailable(cglm)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(googlebenchmark)

add_library(glad OBJECT
    vendor/glad/src/glad.c
)
target_include_directories(glad
    PUBLIC vendor/glad/include
)
add_library(lua_bindings OBJECT
    src/bindings/world_lua.cpp
    src/bindings/time_lua.cpp
)
target_include_directories(lua_bindings
    PRIVATE
        src/core
    PUBLIC
        src/bindings
        src/bindings/lua_headers
)
target_link_directories(lua_bindings PUBLIC lib)
target_link_libraries(lua_bindings PUBLIC
    luauvm
    luaucompiler
    luaubytecode
    luauast
    luaucommon
    # asan_flags
)
add_library(core OBJECT
    src/core/engine.cpp
    src/core/script.cpp
    src/core/time.cpp
    src/core/window.cpp
    src/core/world.cpp
)
target_include_directories(core
    PUBLIC src/core
)
target_link_libraries(core PUBLIC
    glad
    glfw
    lua_bindings
    # asan_flags
)
add_library(systems INTERFACE)
add_library(render_system OBJECT)
target_link_libraries(systems INTERFACE render_system)
target_sources(render_system # line 89
    PRIVATE
        src/systems/render/mesh.cpp
        src/systems/render/process_calls.c
        src/systems/render/render_system.cpp
        src/systems/render/shader.cpp
    PUBLIC FILE_SET HEADERS
        BASE_DIRS src/systems/render
        FILES
            src/systems/render/mesh.h
            src/systems/render/process_calls.h
            src/systems/render/render_system.hpp
            src/systems/render/shader.h
)
target_link_libraries(render_system PRIVATE glad cglm glm core)
add_executable(moxie-engine test/test.cpp)
target_link_libraries(moxie-engine PRIVATE core lua_bindings glad freetype cglm glm systems render_system)
target_compile_options(moxie-engine PRIVATE -std=c++23 -g)