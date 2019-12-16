#pragma once

#include <GLSLProgram.hpp>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

class   Shader :public GLSLProgram{

public:
    uniform     _view;
    uniform     _proj;
    uniform     _trans[10];
    uniform     _isTrans[10];
    uniform     _lightPos[20];
    uniform     _lightColor[20];
    uniform     _isLight[20];
    uniform     _metallic;
    uniform     _roughness;
    uniform     _camera;
    attribute   _position;//!vertex position
    attribute   _normal;//!normal dir
    attribute   _color;

public:
    //!Override
    virtual void initialize(std::string v, std::string f) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        //! 保证ifstream对象可以抛出异常：
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try{
            //! 打开文件
            vShaderFile.open(v);
            fShaderFile.open(f);
            std::stringstream vShaderStream, fShaderStream;
            // 读取文件的缓冲内容到数据流中
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件处理器
            vShaderFile.close();
            fShaderFile.close();
            // 转换数据流到string
            vertexCode   = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        createProgram(vShaderCode, fShaderCode);
        //!关联
        _view = glGetUniformLocation(_program, "_view");
        _proj = glGetUniformLocation(_program, "_proj");
        for(int i=0;i<10;i++){
            char ctmp0[4],ctmp1[4];itoa(i,ctmp0,10);itoa(i,ctmp1,10);
            char ctmp2[20]="_trans.trans[";char ctmp3[20]="_trans.isTrans[";
            _trans[i] = glGetUniformLocation(_program, strcat(ctmp2,strcat(ctmp0,"]")));
            _isTrans[i] = glGetUniformLocation(_program, strcat(ctmp3,strcat(ctmp1,"]")));
        }
        for(int i=0;i<20;i++){
            char ctmp0[4],ctmp1[4],ctmp2[4];itoa(i,ctmp0,10);itoa(i,ctmp1,10);itoa(i,ctmp2,10);
            char ctmp3[20]="_light.lightPos[";char ctmp4[20]="_light.lightColor[";char ctmp5[20]="_light.isLight[";
            _lightPos[i] = glGetUniformLocation(_program, strcat(ctmp3,strcat(ctmp0,"]")));
            _lightColor[i] = glGetUniformLocation(_program, strcat(ctmp4,strcat(ctmp1,"]")));
            _isLight[i] = glGetUniformLocation(_program, strcat(ctmp5,strcat(ctmp2,"]")));
        }
        _metallic = glGetUniformLocation(_program, "_metallic");
        _roughness = glGetUniformLocation(_program, "_roughness");
        _camera = glGetUniformLocation(_program, "_camera");

        _position = glGetAttribLocation(_program, "_position");
        _normal = glGetAttribLocation(_program, "_normal");
        _color = glGetAttribLocation(_program, "_color");
    }
    virtual void begin() {
        glEnableVertexAttribArray(_position);
        glEnableVertexAttribArray(_normal);
        glEnableVertexAttribArray(_color);
        glUseProgram(_program);
    }
    virtual void end() {
        glDisableVertexAttribArray(_position);
        glDisableVertexAttribArray(_normal);
        glDisableVertexAttribArray(_color);
        glUseProgram(0);
    }



};