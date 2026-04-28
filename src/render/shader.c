#include "render.h"
#include "glad/glad.h"
#include "memory.h"

#include <string.h>
#include <stdio.h>

static void get_shader_uniforms(ShaderProgram *shader) {
    int n_uniforms = 0;
    glGetProgramiv(shader->ID, GL_ACTIVE_UNIFORMS, &n_uniforms); GL_CHECK()
    shader->UniformsCount = n_uniforms;

    int max_length;
    glGetProgramiv(shader->ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length); GL_CHECK()

    size_t block_size = n_uniforms * (sizeof(Uniform) + max_length);

    MemoryArena *arena = CreateMemoryArena(block_size);
    shader->Uniforms = PushToMemoryArena(arena, n_uniforms * sizeof(Uniform));
    
    for (int i = 0; i < n_uniforms; i++) {
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(
            shader->ID, i,
            max_length, &length,
            &size, &type,
            arena->data + arena->size
        ); GL_CHECK()

        char *name = PushToMemoryArena(arena, length+1);
        shader->Uniforms[i].Name = name;
        shader->Uniforms[i].Location = glGetUniformLocation(shader->ID, name); GL_CHECK()
    }
}

ShaderProgram *CreateShader(const char *vert_src, const char *frag_src) {
    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER); GL_CHECK()
    glShaderSource(vert_shader, 1, &vert_src, NULL); GL_CHECK()
    glCompileShader(vert_shader); GL_CHECK()
    int vert_success;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &vert_success); GL_CHECK()
    if (!vert_success) {
        GLchar info[512];
        glGetShaderInfoLog(vert_shader, 512, NULL, info); GL_CHECK()
        fprintf(stderr, "VERTEX SHADER COMPILATION ERROR:\n%s\n", info);
    }
    
    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER); GL_CHECK()
    glShaderSource(frag_shader, 1, &frag_src, NULL); GL_CHECK()
    glCompileShader(frag_shader); GL_CHECK()
    int frag_success;
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &frag_success); GL_CHECK()
    if (!frag_success) {
        GLchar info[512];
        glGetShaderInfoLog(frag_shader, 512, NULL, info); GL_CHECK()
        fprintf(stderr, "FRAGMENT SHADER COMPILATION ERROR:\n%s\n", info);
    }
    if (!frag_success || !vert_success) {
        return NULL;
    }

    GLuint shader_id = glCreateProgram(); GL_CHECK()
    glAttachShader(shader_id, vert_shader); GL_CHECK()
    glAttachShader(shader_id, frag_shader); GL_CHECK()
    glLinkProgram(shader_id); GL_CHECK()

    int shader_success;
    glGetProgramiv(shader_id, GL_LINK_STATUS, &shader_success); GL_CHECK()
    if (!shader_success) {
        GLchar info[512];
        glGetProgramInfoLog(shader_id, 512, NULL, info); GL_CHECK()
        fprintf(stderr, "SHADER LINKING ERROR:\n%s\n", info);

        return NULL; 
    }

    glDeleteShader(vert_shader); GL_CHECK()
    glDeleteShader(frag_shader); GL_CHECK()

    ShaderProgram *shader = AllocMem(sizeof(ShaderProgram));
    shader->ID = shader_id;
    get_shader_uniforms(shader);

    return shader;
}
GLint GetUniformLocation(ShaderProgram *shader, const char *name) {
    if (shader == NULL) {
        return -1;
    }
    for (int i = 0; i < shader->UniformsCount; i ++) {
        if (strcmp(shader->Uniforms[i].Name, name) == 0) {
            return shader->Uniforms[i].Location;
        }
    }
    return -1;
}