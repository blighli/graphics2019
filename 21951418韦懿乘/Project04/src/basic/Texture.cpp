//
// Created by Krisu on 2019-11-22.
//

#include <utility>
#include <iostream>
#include "Texture.hpp"
#include <stb_image.h>

Texture::Texture(const std::string& path, bool data_is_float):
        m_type(TextureType::Texture2D), data_is_float(data_is_float) {

    this->load(path);
}

Texture::Texture(unsigned int id, unsigned int type) :
        id(id), m_type(type), data_is_float(false) {

    if (type != TextureType::Texture2D && type != TextureType::CubeMap) {
        throw std::runtime_error("undefined texture type");
    }
}


void Texture::load(const std::string &path) {
    stbi_set_flip_vertically_on_load(true);

    if (data_is_float) {
        data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
    } else {
        data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    }
    if (!data) {
        throw std::runtime_error("Texture failed to load at path: "
                                 + path + "\n");
    }
}

unsigned Texture::bind() {
    // Load and bind if not
    if (id == 0) {
        GLenum inFormat, outFormat, texelType;

        if (data_is_float) {
            inFormat = GL_RGB16F;
            outFormat = GL_RGB;
            texelType = GL_FLOAT;
        } else {
            texelType = GL_UNSIGNED_BYTE;
            if (nrComponents == 1) {
                inFormat = outFormat = GL_RED;
            } else if (nrComponents == 3) {
                inFormat = GL_SRGB;
                outFormat = GL_RGB;
            } else if (nrComponents == 4) {
                inFormat = GL_SRGB_ALPHA;
                outFormat = GL_RGBA;
            } else {
                throw std::runtime_error(
                        "Undefined texture format: nrComponents = "
                         + std::to_string(nrComponents));
            }
        }

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        // load texture data into opengl
        glTexImage2D(GL_TEXTURE_2D, 0, inFormat, width, height, 0, outFormat,
                     texelType, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        free();
    }
    return id;
}

Texture::~Texture() {
    free();
    // delete texture from opengl and free space
    glDeleteTextures(1, &id);
}

void Texture::free() {
    if (data) {
        stbi_image_free(data);
        data = nullptr;
    }
}