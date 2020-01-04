#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 projection_matrix = mat4(1.0);
uniform mat4 view_matrix = mat4(1.0);
uniform mat4 model_matrix = mat4(1.0);

out DATA {
	vec2 texCoord;
}vs_out;

void main() {
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(aPosition, 1.0);
	vs_out.texCoord = aTexCoord;
}