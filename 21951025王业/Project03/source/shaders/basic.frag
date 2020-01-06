#version 330 core

out vec4 color;

in DATA {
	vec2 texCoord;
}fs_in;

uniform sampler2D ourTexture;

void main() {
	color = vec4(texture(ourTexture, fs_in.texCoord));
}