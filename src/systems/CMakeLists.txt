add_library(systems INTERFACE)
add_library(render_system STATIC)
target_sources(render_system
    PRIVATE
        render/mesh.cpp
        render/process_calls.c
        render/render_system.cpp
        render/shader.cpp
        render/text.cpp
    PUBLIC
        FILE_SET render_system_headers
        TYPE HEADERS
        BASE_DIRS render
        FILES
            render/mesh.h
            render/process_calls.h
            render/render_system.hpp
            render/shader.h
            render/text.h
)

add_library(physics_system STATIC)
target_sources(physics_system
    PRIVATE
        physics/collision.c
    PUBLIC
        FILE_SET physics_system_headers
        TYPE HEADERS
        BASE_DIRS physics
        FILES
            physics/physics_system.h
)

target_link_libraries(render_system PUBLIC cglm glad PRIVATE freetype engine transform compiler_flags)
target_link_libraries(physics_system PUBLIC cglm PRIVATE transform compiler_flags)

target_link_libraries(systems INTERFACE render_system physics_system)