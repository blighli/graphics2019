// 箱子顶点着色器
#version 330 core

// 位置顶点 及 位置顶点的向量
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;        // 纹理坐标

out vec3 IN_ObjPos;     // 顶点位置
out vec3 IN_Normal;     // 法向量
out vec2 IN_TexCoords;  // 纹理

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// 顶点变换到世界空间坐标 把顶点位置属性乘以模型矩阵
void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

    IN_ObjPos = vec3(model * vec4(position, 1.0f));
    IN_Normal = mat3(transpose(inverse(model))) * normal;

    IN_TexCoords = texCoords;   // 纹理输出至片断着色器
}
