
// 灯泡顶点着色器
#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 灯泡只需设置位置
    gl_Position = projection * view * model * vec4(position, 1.0f);

}
