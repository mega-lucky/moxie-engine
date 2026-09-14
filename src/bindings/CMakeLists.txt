add_library(luaumeta STATIC
    lua_meta/transform_meta.cpp
    lua_meta/meshshape_meta.cpp
    lua_meta/meshrender_meta.cpp
    lua_meta/worldquery_meta.cpp
    lua_meta/camera_meta.cpp
    lua_meta/quaternion_meta.cpp
)
target_sources(luaumeta PUBLIC
    FILE_SET luau_tags_header
    TYPE HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lua_meta
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/lua_meta/lua_meta.h
        ${CMAKE_CURRENT_SOURCE_DIR}/lua_meta/lua_tags.h
)

target_link_libraries(luaumeta PRIVATE
    core
    render_system
    Luau.VM
)
add_library(luaulibs STATIC
    lua_libs/world_lua.cpp
    lua_libs/time_lua.cpp
    lua_libs/inputs_lua.cpp
    lua_libs/quaternion_lua.cpp
)
target_sources(luaulibs PUBLIC
    FILE_SET luau_lib_headers
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lua_libs
    TYPE HEADERS
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/lua_libs/lua_libs.h
)
target_link_libraries(luaulibs PRIVATE
    world
    time
    input
    cglm
    Luau.VM
    luaumeta
)
add_library(bindings INTERFACE)
target_link_libraries(bindings INTERFACE luaumeta luaulibs)