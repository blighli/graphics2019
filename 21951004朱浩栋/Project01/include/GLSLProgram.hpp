#pragma once

#include <iostream>
#include <assert.h>
#include "GL/glew.h"
#include <string>

class   GLSLProgram{

public:
    typedef GLint   attribute;
    typedef GLint   uniform;

public:
    GLint           _vHandle;
    GLint           _fHandle;
    GLint           _program;

public:
    GLSLProgram();
    virtual ~GLSLProgram(){};
    virtual void    initialize(std::string v, std::string f)=0;//! 初始化
    virtual void    begin()=0;//! 使用shader
    virtual void    end()=0;//! 结束

protected:
    bool createProgram(const std::string vs, const std::string fs);//!创建GPU程序
};

GLSLProgram::GLSLProgram() {
    _vHandle    =   -1;
    _fHandle    =   -1;
    _program    =   -1;
}
bool GLSLProgram::createProgram(const std::string vs, const std::string fs){

    bool    result = false;
    do{
        //! 创建shader
        _vHandle = glCreateShader(GL_VERTEX_SHADER);
        _fHandle = glCreateShader(GL_FRAGMENT_SHADER);
        //! 指定shader的源代码
        glShaderSource(_vHandle, 1, (const GLchar**)&vs, 0);
        glShaderSource(_fHandle, 1, (const GLchar**)&fs, 0);
        //! 编译shader
        glCompileShader(_vHandle);
        GLint   status = 0;
        char    compileLog[1024] = { 0 };
        glGetShaderiv(_vHandle, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE){
            glGetShaderInfoLog(_vHandle, sizeof(compileLog), 0, compileLog);
            std::cout<<"VS ERROR!"<<std::endl<<compileLog<<std::endl;
            assert(false && compileLog);
            break;
        }

        glCompileShader(_fHandle);
        glGetShaderiv(_fHandle, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE){
            glGetShaderInfoLog(_fHandle, sizeof(compileLog), 0, compileLog);
            std::cout<<"FS ERROR!"<<std::endl<<compileLog<<std::endl;
            assert(false && compileLog);
            break;
        }
        //! 4创建程序
        _program = glCreateProgram();
        glAttachShader(_program, _vHandle);
        glAttachShader(_program, _fHandle);
        //! 5链接程序
        glLinkProgram(_program);
        glGetProgramiv(_program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE){
            glGetProgramInfoLog(_program, sizeof(compileLog), 0, compileLog);
            break;
        }
        result = true;

    } while (false);

    if (!result){
        if (_vHandle != -1){
            glDeleteShader(_vHandle);
        }
        if (_fHandle != -1){
            glDeleteShader(_fHandle);
        }
        if (_program != -1){
            glDeleteProgram(_program);
        }
        _vHandle = -1;
        _fHandle = -1;
        _program = -1;
    }

    return  result;
}
