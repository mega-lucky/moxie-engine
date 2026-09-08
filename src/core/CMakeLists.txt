add_library(engine STATIC engine.cpp)
target_sources(engine PUBLIC
    FILE_SET engine_headers
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    TYPE HEADERS
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/engine.hpp
)
target_link_libraries(engine
PUBLIC
    window
    input
    script
    time
    world
PRIVATE
    compiler_flags
)

add_library(window STATIC window.cpp)
target_sources(window PUBLIC
    FILE_SET window_headers
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    TYPE HEADERS
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/window.hpp
)
target_link_libraries(window PRIVATE
    glfw
    glad
)
target_include_directories(window PRIVATE
    ${PROJECT_SOURCE_DIR}/src/internal
)

add_library(input STATIC input.cpp)
target_sources(input PUBLIC
    FILE_SET input_headers
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    TYPE HEADERS
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/input.hpp
)
target_link_libraries(input PRIVATE
    glfw
)
target_include_directories(input PRIVATE
    ${PROJECT_SOURCE_DIR}/src/internal
)

add_library(time STATIC time.cpp)
target_sources(time PUBLIC
    FILE_SET time_headers
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    TYPE HEADERS
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/time.hpp
)

add_library(world STATIC world.cpp)
target_sources(world PUBLIC
    FILE_SET world_headers
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    TYPE HEADERS
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/world.hpp
)
target_link_libraries(world PUBLIC
    utils
)

add_library(script STATIC script.cpp)
target_sources(script PUBLIC
    FILE_SET script_headers
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    TYPE HEADERS
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/script.hpp
)
target_link_libraries(script PRIVATE
    engine
    bindings
    Luau.VM
    Luau.Compiler
    Luau.Bytecode
    Luau.Ast
    Luau.Common
)

add_library(core INTERFACE)
target_link_libraries(core INTERFACE engine)