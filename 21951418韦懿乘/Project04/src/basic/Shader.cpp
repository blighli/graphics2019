//
// Created by Krisu on 2019-11-18.
//

#include "Shader.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char *vertexPath,
               const char *fragmentPath,
               const char *geometryPath) {
    // 1. retrive the vertex/fragment src from filepath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files.
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (geometryPath != nullptr) {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch(std::ifstream::failure &e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
        exit(-1);
    }
    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();

    // 2.compile shaders
    unsigned int vertex, fragment;
    int success;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    checkCompileErrors(vertex, "VERTEX", vertexPath);

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    checkCompileErrors(fragment, "FRAGMENT", fragmentPath);

    // if geometry exists
    unsigned int geometry = 0;
    if (geometryPath != nullptr) {
        const char * gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, nullptr);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY", geometryPath);
    }

    // shader program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    if (geometryPath != nullptr)
        glAttachShader(id, geometry);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM", vertexPath);
    // delete shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr)
        glDeleteShader(geometry);
}

unsigned int Shader::getID() const {
    return this->id;
}

void Shader::use() const {
    glUseProgram(this->id);
}

void Shader::set(const std::string &name, bool value) const {
    this->use();
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::set(const std::string &name, int value) const {
    this->use();
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set(const std::string &name, unsigned value) const {
    this->use();
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set(const std::string &name, float value) const {
    this->use();
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set(const std::string &name, const glm::vec2 &value) const {
    this->use();
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::set(const std::string &name, float x, float y) const {
    this->use();
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::set(const std::string &name, const glm::vec3 &value) const {
    this->use();
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::set(const std::string &name, float x, float y, float z) const {
    this->use();
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::set(const std::string &name, const glm::vec4 &value) const {
    this->use();
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::set(const std::string &name, float x, float y, float z,
                      float w) const {
    this->use();
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::set(const std::string &name, const glm::mat2 &mat) const {
    this->use();
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set(const std::string &name, const glm::mat3 &mat) const {
    this->use();
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set(const std::string &name, const glm::mat4 &mat) const {
    this->use();
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}



void
Shader::checkCompileErrors(GLuint shader, const std::string& type, std::string path) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << path << std::endl;
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- ------------------------------------ -- \n";
            exit(-1);
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                path = path.substr(0, path.find('.'));
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << path << std::endl;
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n --  ------------------------------------ -- " << std::endl;
                exit(-1);
            }
        }
    }
}

