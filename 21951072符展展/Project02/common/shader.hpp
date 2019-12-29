#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath = nullptr, const GLchar *tessControlPath = nullptr,
           const GLchar *tessEvalPath = nullptr) {
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::string tessControlCode;
        std::string tessEvalCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        std::ifstream tcShaderFile;
        std::ifstream teShaderFile;
        // 保证ifstream对象可以抛出异常：
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        tcShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        teShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            if (geometryPath != nullptr) {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
            if (tessControlPath != nullptr && tessEvalPath != nullptr) {
                //tc
                tcShaderFile.open(tessControlPath);
                std::stringstream tcShaderStream;
                tcShaderStream << tcShaderFile.rdbuf();
                tcShaderFile.close();
                tessControlCode = tcShaderStream.str();
                //te
                teShaderFile.open(tessEvalPath);
                std::stringstream teShaderStream;
                teShaderStream << teShaderFile.rdbuf();
                teShaderFile.close();
                tessEvalCode = teShaderStream.str();
            }
        }
        catch (std::ifstream::failure &e) {
            std::cout <<"SHADER_ERROR: FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        GLuint vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, vertexPath);
        std::cout << "Compiling shader : " << vertexPath << std::endl;

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, fragmentPath);
        std::cout << "Compiling shader : " << fragmentPath << std::endl;

        GLuint geometry;
        if (geometryPath != nullptr) {
            const char *gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, nullptr);
            glCompileShader(geometry);
            checkCompileErrors(geometry, geometryPath);
            std::cout << "Compiling shader : " << geometryPath << std::endl;
        }

        GLuint tc, te;
        if (tessControlPath != nullptr && tessEvalPath != nullptr) {
            const char *tcShaderCode = tessControlCode.c_str();
            tc = glCreateShader(GL_TESS_CONTROL_SHADER);
            glShaderSource(tc, 1, &tcShaderCode, nullptr);
            glCompileShader(tc);
            checkCompileErrors(tc, tessControlPath);
            std::cout << "Compiling shader : " << tessControlPath << std::endl;

            const char *teShaderCode = tessEvalCode.c_str();
            te = glCreateShader(GL_TESS_EVALUATION_SHADER);
            glShaderSource(te, 1, &teShaderCode, nullptr);
            glCompileShader(te);
            checkCompileErrors(te, tessEvalPath);
            std::cout << "Compiling shader : " << tessEvalPath << std::endl;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr) {
            glAttachShader(ID, geometry);
        }
        if (tessControlPath != nullptr && tessEvalPath != nullptr) {
            glAttachShader(ID, tc);
            glAttachShader(ID, te);
        }
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr) {
            glDeleteShader(geometry);
        }
        if (tessControlPath != nullptr && tessEvalPath != nullptr) {
            glDeleteShader(tc);
            glDeleteShader(te);
        }
    }

    void use() {
        glUseProgram(ID);
    }

    void del(){
        glDeleteProgram(ID);
    }

    void setBool(const std::string &name, bool value) const {
        int l = glGetUniformLocation(ID, name.c_str());
        if (l == -1)
            std::cout << name << std::endl;
        //assert(l != -1);
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    void setInt(const std::string &name, int value) const {
        int l = glGetUniformLocation(ID, name.c_str());
        if (l == -1)
            std::cout << name << std::endl;
        //assert(l != -1);
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        int l = glGetUniformLocation(ID, name.c_str());
        if (l == -1)
            std::cout << name << std::endl;
        //assert(l != -1);
        glUniform1f(l, value);

    }

    void setVec2(const std::string &name, const glm::vec2 &value) const {
        int l = glGetUniformLocation(ID, name.c_str());
        if (l == -1)
            std::cout << name << std::endl;
        //assert(l != -1);
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec2(const std::string &name, float x, float y) const {
        int l = glGetUniformLocation(ID, name.c_str());
        if (l == -1)
            std::cout << name << std::endl;
        //assert(l != -1);
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const {
        int l = glGetUniformLocation(ID, name.c_str());
        if (l == -1)
            std::cout << name << std::endl;
        //assert(l != -1);
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string &name, float x, float y, float z) const {
        int l = glGetUniformLocation(ID, name.c_str());
        if (l == -1)
            std::cout << name << std::endl;
        //assert(l != -1);
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const {
        int l = glGetUniformLocation(ID, name.c_str());
        if (l == -1)
            std::cout << name << std::endl;
        //assert(l != -1);
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string &name, float x, float y, float z, float w) {
        int l = glGetUniformLocation(ID, name.c_str());
        if (l == -1)
            std::cout << name << std::endl;
        //assert(l != -1);
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        int l = glGetUniformLocation(ID, name.c_str());
        if (l == -1)
            std::cout << name << std::endl;
        //assert(l != -1);
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:

    static void checkCompileErrors(GLuint shader, const std::string &type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION: " << type << "\n" << infoLog << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR: " << type << "\n" << infoLog << std::endl;
            }
        }
    }
};