#version 330 core

out vec4 FragColor;

uniform vec3 LampColor;

void main() {
	FragColor = vec4(LampColor, 1.0f); // set all the 4 vector values to 1.0
}