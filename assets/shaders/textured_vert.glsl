#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec4 aClr;

uniform mat4 uprojection;
uniform mat4 uview;
uniform mat4 umodel;

out vec2 uv_coord;
out vec4 base_clr;

void main() {
   uv_coord = aUV;
   base_clr = aClr;
   gl_Position = uprojection * uview * umodel * vec4(aPos, 1.0);
}