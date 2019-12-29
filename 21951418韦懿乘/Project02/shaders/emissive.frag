#version 330
layout(location = 0) out vec4 FragColor;

uniform sampler2D albedo;

in vec2 TexCoords;

void main() {
    vec3 color = texture(albedo, TexCoords).rgb;
    color = pow(color, vec3(1.0 / 2.2)); // gamma
    FragColor = vec4(color, 1.0);
}
