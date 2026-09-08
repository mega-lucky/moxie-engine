add_library(systems INTERFACE)
add_library(render_system STATIC)
target_link_libraries(systems INTERFACE render_system)
target_sources(render_system
    PRIVATE
        render/mesh.cpp
        render/process_calls.c
        render/render_system.cpp
        render/shader.cpp
    PUBLIC
        FILE_SET render_system_headers
        TYPE HEADERS
        BASE_DIRS render
        FILES
            render/mesh.h
            render/process_calls.h
            render/render_system.hpp
            render/shader.h
)
target_link_libraries(render_system PUBLIC cglm glad PRIVATE core compiler_flags)