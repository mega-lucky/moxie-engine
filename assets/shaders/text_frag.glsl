#version 330 core

in vec2 TexCoord;
in vec4 TextColour;

uniform sampler2D utexture;

out vec4 color;

void main() {
    vec4 sampler = vec4(1.0, 1.0, 1.0, texture(utexture, TexCoord).r);
    color = TextColour * sampler;
};