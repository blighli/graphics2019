//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_SKYBOX_HPP
#define RENDER_ENGINE_SKYBOX_HPP

#include "../src/basic/Object.hpp"
#include "../basic/Shader.hpp"
#include "../basic/Texture.hpp"

class Skybox : public Object {
public:
    Skybox() = delete;
    Skybox(const std::string &path, const std::string &texture_type="");

    void prepare() override;
    void render() override;

    inline unsigned int getIrradianceMap()  { return irradianceMap; }
    inline unsigned int getPrefilterMap()   { return prefilterMap; }
    inline unsigned int getBrdfLUTTexture() { return brdfLUTTexture; }

private:
    Shader equirectToCubemapShader;
    Shader irradianceShader;
    Shader prefilterShader;
    Shader brdfLUTShader;

    Texture hdrTexture;

    unsigned int envCubemap;
    unsigned int irradianceMap;
    unsigned int prefilterMap;
    unsigned int brdfLUTTexture;
};


#endif //RENDER_ENGINE_SKYBOX_HPP
