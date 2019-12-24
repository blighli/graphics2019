#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vuv;
layout (location = 2) in vec3 vNormal;

uniform mat4 MVP;
uniform mat4 M;
out vec2 uv;
out vec3 normal;

void main(){
    gl_Position = MVP*vec4(vPosition, 1);
    uv = vuv;
    normal = mat3(M)*vNormal;
}