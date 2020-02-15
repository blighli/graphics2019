//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_SKYBOX_HPP
#define RENDER_ENGINE_SKYBOX_HPP

#include "basic/Object.hpp"
#include "basic/Shader.hpp"
#include "basic/Texture.hpp"

// TODO: store precomputed map into actual files.
class Skybox {
public:
    explicit Skybox(const std::string& path_to_image);

    void prepare();
    void render();

    inline unsigned int getIrradianceMap()  { return irradianceMap; }
    inline unsigned int getPrefilterMap()   { return prefilterMap; }
    inline unsigned int getBrdfLUTTexture() { return brdfLUTTexture; }

public:
    Shader shader {"shaders/skybox.vert", "shaders/skybox.frag"};

private:
    Shader equirectToCubemapShader {"shaders/cubemap.vert",
                                    "shaders/equirectangular-to-cubemap.frag"};
    Shader irradianceShader {"shaders/cubemap.vert",
                             "shaders/irradiance-convolution.frag"};
    Shader prefilterShader {"shaders/cubemap.vert",
                            "shaders/prefilter-map.frag"};
    Shader brdfLUTShader {"shaders/brdf.vert", "shaders/brdf.frag"};
    Texture hdrTexture;

    unsigned int envCubemap = 0;
    unsigned int irradianceMap = 0;
    unsigned int prefilterMap = 0;
    unsigned int brdfLUTTexture = 0;
};


#endif //RENDER_ENGINE_SKYBOX_HPP
