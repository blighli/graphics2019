#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 albedo = vec3(1, 1, 1);

void main() {
    FragColor = vec4(albedo, 1.0);
}




