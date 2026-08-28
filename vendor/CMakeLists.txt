add_library(glad OBJECT
    glad/src/glad.c
)
target_include_directories(glad
    PUBLIC glad/include
)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb)