#include "./types.h"
#include "./shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

void init_shader(shader_data *data, const char *vert_src, const char *frag_src) {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vert_src, NULL);
    glCompileShader(vs);
    GLint success;
    char log[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, NULL, log); std::cerr << "VS: " << log << "\n";
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &frag_src, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, NULL, log); std::cerr << "FS: " << log << "\n";
    }

    GLuint shader = glCreateProgram();
    glAttachShader(shader, vs);
    glAttachShader(shader, fs);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, log); std::cerr << "LINK: " << log << "\n";
    }

    data->id = shader;

    GLint n_uniforms = 0;
    GLint max_name_len;
    glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &n_uniforms);
    glGetProgramiv(shader, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);
}

void init_shader_from_file(shader_data *data, const char *vert_path, const char *frag_path) {
    std::ifstream vert_file(vert_path);
    if (!vert_file.is_open()) {
        throw std::runtime_error("Unable to open file.");
    }

    std::ostringstream vert_stream;
    vert_stream << vert_file.rdbuf();

    std::ifstream frag_file(frag_path);
    if (!frag_file.is_open()) {
        throw std::runtime_error("Unable to open file.");
    }

    std::ostringstream frag_stream;
    frag_stream << frag_file.rdbuf();

    init_shader(data, vert_stream.str().c_str(), frag_stream.str().c_str());
}