#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>

enum TextureEnum {
    DIFFUSE,
    SPECULAR,
    NORMAL
};

class Texture {
public:


    Texture(GLenum TextureTarget, const std::string &FileName, TextureEnum TextureType);

    ~Texture();

    void Load();

    void Bind(GLenum TextureUnit);

    std::string getFilename() { return m_fileName; }

    TextureEnum getTextureType() { return m_textureType; }

private:

    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureID;
    TextureEnum m_textureType;

};


GLuint loadDDS(const char *imagepath);

