add_library(lua_bindings OBJECT
    world_lua.cpp
    time_lua.cpp
    meta/transform_meta.cpp
)
target_include_directories(lua_bindings
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        lua_headers
        meta
)
target_link_libraries(lua_bindings PUBLIC
    Luau.VM
    Luau.Compiler
    Luau.Bytecode
    Luau.Ast
    Luau.Common
)