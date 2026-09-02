add_library(compiler_flags INTERFACE)
target_compile_options(compiler_flags INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Debug>>:-fsanitize=address -fno-omit-frame-pointer>
)

target_link_options(compiler_flags INTERFACE
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Debug>>:-fsanitize=address>
)

add_subdirectory(core)
add_subdirectory(bindings)
add_subdirectory(systems)
add_subdirectory(utils)

add_executable(moxie-engine main.cpp)
target_link_libraries(moxie-engine PRIVATE
    core
    lua_bindings
    systems
    cglm
    glm
    render_system
    glad
    utils
)
set_target_properties(moxie-engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)