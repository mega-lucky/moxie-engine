add_library(core OBJECT
    engine.cpp
    script.cpp
    time.cpp
    window.cpp
    world.cpp
)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(core PUBLIC
    glad
    glfw
    lua_bindings
    compiler_flags
)