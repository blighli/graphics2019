#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vuv;

uniform mat4 MVP;

out vec2 uv;

void main(){
    gl_Position = MVP*vec4(vPosition, 1);
    uv = vuv;
}