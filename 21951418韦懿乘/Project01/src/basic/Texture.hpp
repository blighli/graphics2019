//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_TEXTURE_HPP
#define RENDER_ENGINE_TEXTURE_HPP

#include <glad/glad.h>
#include <cassert>
#include <string>

class Texture {
public:
    Texture() = default;
    explicit Texture(std::string textureFilePath,
                     std::string tag="");

    inline unsigned id() { assert(textureID != -1); return textureID; };
    inline const std::string& type() { return tag; }
    inline const std::string& path() { return filePath; }
    ~Texture();

protected:
    // Load texture from drive to memory, not bind to any texture object
    void loadTexture();
    // Bind texture and return texture id
    unsigned bindTexture();
    // Free memory
    void free();

private:
    // unsigned char (byte) for normal jpg
    // float for hdr image
    void *data = nullptr;
    std::string filePath;
    std::string tag;
    unsigned int textureID = -1;
    int width = 0, height = 0, nrComponents = 0;
};


#endif //RENDER_ENGINE_TEXTURE_HPP
