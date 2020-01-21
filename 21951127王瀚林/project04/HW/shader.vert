#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

//uniform mat4 transform;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vertexColor;
out vec2 TexCoord;

void main()
{
	gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0f);
	vertexColor = color;
	
	// TexCoord = texCoord;
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}