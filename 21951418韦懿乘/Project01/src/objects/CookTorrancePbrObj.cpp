//
// Created by Krisu on 2019-12-06.
//

#include "CookTorrancePbrObj.hpp"
#include "../basic/Model.hpp"

CookTorrancePbrObj::CookTorrancePbrObj() :
    Object(Shader("./shaders/cookTorrancePBR.vert",
                  "./shaders/cookTorrancePBR.frag")),
    albedo("./resources/pbr/rustediron2_basecolor.png"),
    normal("./resources/pbr/rustediron2_normal.png"),
    metallic("./resources/pbr/rustediron2_metallic.png"),
    roughness("./resources/pbr/rustediron2_roughness.png"),
    ao("./resources/pbr/ao.png") {}

void CookTorrancePbrObj::prepare() {
    renderShader.use();
    renderShader.setValue("albedoMap", 0);
    renderShader.setValue("normalMap", 1);
    renderShader.setValue("metallicMap", 2);
    renderShader.setValue("roughnessMap", 3);
    renderShader.setValue("aoMap", 4);
    renderShader.setValue("irradianceMap", 5);
    renderShader.setValue("prefilterMap", 6);
    renderShader.setValue("brdfLUT", 7);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo.id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal.id());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallic.id());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughness.id());
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, ao.id());
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
}

void CookTorrancePbrObj::render() {
    renderShader.use();
    if (!use_model) {
        Primitive::renderSphere();
    } else {
        model->render();
    }
}
