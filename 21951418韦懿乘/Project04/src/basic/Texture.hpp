//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_TEXTURE_HPP
#define RENDER_ENGINE_TEXTURE_HPP

#include <glad/glad.h>
#include <cassert>
#include <string>

enum TextureType {
    Texture2D = GL_TEXTURE_2D,
    CubeMap = GL_TEXTURE_CUBE_MAP
};

class Texture {
public:
    explicit Texture(const std::string& path,
                     bool float_data=false);
    explicit Texture(unsigned int id, unsigned int type);

    // Bind texture(only once) and return texture id
    unsigned int bind();
    inline unsigned int type() const { return m_type; }

    ~Texture();

protected:
    // Load data from image file into memory
    void load(const std::string &path);
    // Free memory
    void free();

private:
    // unsigned char (byte) for normal jpg, float for hdr image
    bool data_is_float;
    void *data = nullptr;
    unsigned int id = 0;
    unsigned int m_type;
    int width = 0, height = 0, nrComponents = 0;
};


#endif //RENDER_ENGINE_TEXTURE_HPP
