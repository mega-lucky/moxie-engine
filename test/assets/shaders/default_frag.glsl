#version 330 core

in vec3 normal;
in vec3 fragpos;
in vec4 fragclr;
in vec2 texcoords;

uniform vec3 u_lightpos;
uniform mat4 u_model;
uniform vec4 u_colour;
uniform sampler2D u_texture;
uniform bool u_istextured;

out vec4 FragColor;

float ambient_str = 0.5f;
void main() {
    vec4 light_colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec4 ambient = vec4((ambient_str * light_colour).rgb, 1.0f);
    
    vec3 lightdir = normalize(u_lightpos - u_model[3].xyz);
    float diff = max(dot(normal, lightdir), 0.0f);
    vec4 diffuse = diff * light_colour;
    vec4 lighting = (ambient + diff);
    
    if (u_istextured) {
        FragColor = texture(u_texture, texcoords) * lighting;
    } else {
        FragColor = u_colour * lighting;
    }
}