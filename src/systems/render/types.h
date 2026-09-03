#ifndef __RENDER_TYPES__
#define __RENDER_TYPES__

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#include <stdbool.h>
#endif
#include <glad/glad.h>
#include <cglm/cglm.h>

enum texture_type {
    ALBEDO_TEXTURE
};

typedef struct uniform_data {
    char *name;
    GLuint location;
} uniform_data;

typedef struct shader_data {
    GLuint id;
    uniform_data *uniforms;
    size_t n_uniforms;
} shader_data;

typedef struct texture_data {
    GLuint id;
    enum texture_type type;
} texture_data;

typedef struct material_data {
    shader_data *shader;
    texture_data **textures;
    size_t n_textures;
} material_data;

enum attrib_type {
    ATTRIB_F32,
    ATTRIB_U8,
};

typedef struct vert_attrib_data {
    int size;
    enum attrib_type type;
    bool normalised;
    size_t offset;
} vert_attrib_data;

typedef struct attrib_layout {
    vert_attrib_data *attribs;
    int n_attribs;
    size_t stride;
} attrib_layout;

typedef struct mesh_shape {
    void *vertex_data;
    size_t n_vertices;

    unsigned int *index_data;
    size_t n_indices;

    GLuint VAO, VBO, EBO;

    attrib_layout layout;
} mesh_shape;

typedef struct mesh_vertex {
    vec3 position;
    vec3 normal;
    vec2 uv[2];
    unsigned char colour[4];
} mesh_vertex;

typedef struct glyph_metrics {
    int width, height;
    int bearingx, bearingy;
    int advance;

    int texture_x;
    int texture_y;
} glyph_metrics;

typedef struct font_face {
    glyph_metrics glyphs[96];
    GLuint texture_id;
    
    int atlas_width;
    int atlas_height;
    int line_height;
} font_face;

enum drawcall_type {
    mesh_drawcall,
    text_drawcall
};

typedef struct draw_call {
    enum drawcall_type type;
    union {
        struct {
            mat4 model;
            mesh_shape *mesh;
            material_data *material;
        } mesh;
        struct {
            const char *content;
            size_t content_len;
            font_face *font;
            float x, y;
            float scale;
            float colour[4];
        } text;
    };
} draw_call;

#endif