#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec4 a_colour;
layout (location = 3) in vec2 a_texcoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 normal;
out vec3 fragpos;
out vec4 fragclr;
out vec2 texcoords;

vec4 get_colour(vec3 normal) {
    vec3 clr = normal.x!=0 ? vec3(1.0f,0.0f,0.0f) : normal.y!=0 ? vec3(0.0f,1.0f,0.0f) : normal.z!=0 ? vec3(0.0f,0.0f,1.0f) : vec3(0.0f,0.0f,0.0f);
    if (normal.x + normal.y + normal.z == -1) {clr *= 0.5f;}
    return vec4(clr, 1.0f);
}

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
    fragpos = vec3(u_model * vec4(a_pos, 1.0f));
    mat3 normalmatrix = transpose(inverse(mat3(u_model)));
    normal = normalize(normalmatrix * a_normal);
    texcoords = a_texcoords;
    fragclr = get_colour(a_normal);
}