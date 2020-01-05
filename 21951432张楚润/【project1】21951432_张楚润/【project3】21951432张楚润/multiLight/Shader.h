//Shader.h
#pragma once

#ifndef TEXTURE_SHADER_H_
#define TEXTURE_SHADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <gl/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    ~Shader();

public:
    void useShaderPrograme();

    GLuint getPrograme() {
        return this->m_nProgram;
    }

private:
    GLuint  m_nProgram;
};

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderF;
    std::ifstream fragementShaderF;

    vertexShaderF.exceptions(std::ifstream::badbit);
    fragementShaderF.exceptions(std::ifstream::badbit);

    try
    {
        vertexShaderF.open(vertexPath);     // 打开文件
        fragementShaderF.open(fragmentPath);

        std::stringstream vertexShaderStream, fragementShaderStream;
        vertexShaderStream << vertexShaderF.rdbuf();    // 读取文件至stringstream中
        fragementShaderStream << fragementShaderF.rdbuf();

        vertexShaderF.close();
        fragementShaderF.close();

        vertexCode = vertexShaderStream.str();      // 转换成string类型
        fragmentCode = fragementShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ:" << std::endl;
    }

    const GLchar* pVertexCode = vertexCode.c_str(); // string 转 char*
    const GLchar* pFragementCode = fragmentCode.c_str();

    GLuint nVertexShader, nFragementShader;
    GLint nRes = 0;
    GLchar chLogInfo[512] = { '\0' };

    // 创建顶点着色器
    nVertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 将顶点着色程序的源代码字符数组绑定到顶点着色器对象
    glShaderSource(nVertexShader, 1, &pVertexCode, nullptr);
    glCompileShader(nVertexShader); // compile shader 编译着色器

    // 获取编译结果
    glGetShaderiv(nVertexShader, GL_COMPILE_STATUS, &nRes);
    if (!nRes)
    {
        glGetShaderInfoLog(nVertexShader, 512, nullptr, chLogInfo);
        std::cout << "ERROR::SHADEF::VERTEX::COMPILATION_FAILED:" << chLogInfo << std::endl;
    }

    // 创建片断着色器
    nFragementShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 将片段着色程序的源代码字符数组绑定到片段着色器对象
    glShaderSource(nFragementShader, 1, &pFragementCode, nullptr);
    glCompileShader(nFragementShader);
    glGetShaderiv(nFragementShader, GL_COMPILE_STATUS, &nRes);
    if (!nRes)
    {
        glGetShaderInfoLog(nFragementShader, 512, nullptr, chLogInfo);
        std::cout << "ERROR::SHADEF::FRAGEMENT::COMPILATION_FAILED:" << chLogInfo << std::endl;
    }

    this->m_nProgram = glCreateProgram();   // 创建GLSL程序
    glAttachShader(this->m_nProgram, nVertexShader);    // 绑定shader到program
    glAttachShader(this->m_nProgram, nFragementShader);

    // glLinkProgram操作产生最后的可执行程序，它包含最后可以在硬件上执行的硬件指令
    glLinkProgram(this->m_nProgram);        // 链接
    glGetProgramiv(this->m_nProgram, GL_LINK_STATUS, &nRes);
    if (!nRes)
    {
        glGetProgramInfoLog(this->m_nProgram, 512, nullptr, chLogInfo);
        std::cout << "ERROR::SHADEF::FRAGEMENT::LINK_FAILED:" << chLogInfo << std::endl;
    }

    glDeleteShader(nVertexShader);
    glDeleteShader(nFragementShader);
}

Shader::~Shader()
{
}

void Shader::useShaderPrograme()
{
    glUseProgram(this->m_nProgram); // 使用porgram
}


#endif // SHADER_H
