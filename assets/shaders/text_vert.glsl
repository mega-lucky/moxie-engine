#version 330 core

layout (location = 0) in vec2 vertex_pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec4 colour;

uniform mat4 projection;

out vec2 TexCoord;
out vec4 TextColour;

void main() {
    TexCoord = uv;
    TextColour = colour;
    
    gl_Position = projection * vec4(vertex_pos, 0.0, 1.0);
}