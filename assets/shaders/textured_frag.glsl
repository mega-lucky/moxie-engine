#version 330 core

out vec4 FragColor;
in vec2 uv_coord;
in vec4 base_clr;

uniform sampler2D utexture;

void main() {
   FragColor = texture(utexture, uv_coord) * base_clr;
}