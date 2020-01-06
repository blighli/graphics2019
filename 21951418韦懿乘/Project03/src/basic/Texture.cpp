//
// Created by Krisu on 2019-11-22.
//

#include <utility>
#include <iostream>
#include "Texture.hpp"
#include <stb_image.h>

Texture::Texture(std::string textureFilePath, std::string tag) :
        filePath(std::move(textureFilePath)), tag(std::move(tag)) {
    loadTexture();
    bindTexture();
}


void Texture::loadTexture() {
    stbi_set_flip_vertically_on_load(true);
    unsigned char *byte_ptr;
    float *float_ptr;

    if (tag == "hdr") {
        float_ptr = stbi_loadf(filePath.c_str(), &width, &height,
                               &nrComponents, 0);
        data = float_ptr;
    } else {
        byte_ptr = stbi_load(filePath.c_str(),
                             &width, &height, &nrComponents, 0);
        data = byte_ptr;
    }

    if (!data) {
        std::cerr << "Texture failed to load at path: "
                  << filePath << std::endl;
        exit(-1);
    }
}

unsigned Texture::bindTexture() {
    GLenum inFormat, outFormat, texelType;

    if (tag == "hdr") {
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
            std::cerr << "Undefined texture format: nrComponents = "
                      << nrComponents << std::endl;
            exit(-1);
        }
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, inFormat, width, height, 0, outFormat,
                 texelType, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

Texture::~Texture() {
    free();
}

void Texture::free() {
    if (data) {
        stbi_image_free(data);
        data = nullptr;
    }
}
