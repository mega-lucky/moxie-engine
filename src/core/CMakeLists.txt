add_library(core OBJECT
    engine.cpp
    script.cpp
    time.cpp
    window.cpp
    world.cpp
    input.cpp
)
target_include_directories(core
    PRIVATE ${PROJECT_SOURCE_DIR}/src/internal
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(core PUBLIC
    glad
    glfw
    lua_bindings
    utils
    compiler_flags
)