add_library(luaumeta STATIC
    meta/transform_meta.cpp
    meta/meshshape_meta.cpp
    meta/meshrender_meta.cpp
    meta/worldquery_meta.cpp
    meta/camera_meta.cpp
    meta/quaternion_meta.cpp
)
target_sources(luaumeta PUBLIC
    FILE_SET luau_tags_header
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/lua_tags.h"

    FILE_SET luau_meta_header
    TYPE HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/meta"
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/meta/init_meta.h"
)

target_link_libraries(luaumeta PRIVATE
    core
    render_system
    Luau.VM
)
add_library(luaulibs STATIC
    world_lua.cpp
    time_lua.cpp
    inputs_lua.cpp
    quaternion_lua.cpp
)
target_sources(luaulibs PUBLIC
    FILE_SET luau_lib_headers
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    TYPE HEADERS
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/lua_bindings.h
)
target_link_libraries(luaulibs PRIVATE
    world
    time
    input
    cglm
    Luau.VM
)
add_library(bindings INTERFACE)
target_link_libraries(bindings INTERFACE luaumeta luaulibs)