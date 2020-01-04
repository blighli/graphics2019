#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoords;

out vec3 normal;
out vec3 position;
out vec2 TexCoords;

uniform mat4 M;
uniform mat3 N;
uniform mat4 MVP;

void main()
{
    normal=normalize(N*vertexNormal);
    position=vec3(M*vec4(vertexPosition,1.0));
    TexCoords=vertexTexCoords;
    gl_Position=MVP*vec4(vertexPosition,1.0);
}
