//
// Created by Krisu on 2019-12-29.
//

#include "Material.hpp"

void Material::bind_texture(const std::string &str, Texture *texture) {
    glActiveTexture(GL_TEXTURE0+current_used_texture_units);
    glBindTexture(texture->type(), texture->bind());
    shader->set(str, current_used_texture_units);
    current_used_texture_units++;
}

void Material::updateShader() {
    current_used_texture_units = 1;
    shader->use();
    // ------------------------------------------------------------------------
    if (use_map.albedo) {
        bind_texture("albedo_map", albedo.map);
    } else {
        shader->set("albedo_val", albedo.value);
    }
    // ------------------------------------------------------------------------
    if (use_map.metallic) {
        bind_texture("metallic_map", metallic.map);
    } else {
        shader->set("metallic_val", metallic.value);
    }
    // ------------------------------------------------------------------------
    if (use_map.roughness) {
        bind_texture("roughness_map", roughness.map);
    } else {
        shader->set("roughness_val", roughness.value);
    }
    // ------------------------------------------------------------------------
    if (use_map.emissive) {
        bind_texture("emissive_map", emissive.map);
    } else {
        shader->set("emissive_val", emissive.value);
    }
    // ------------------------------------------------------------------------
    if (normal) {
        bind_texture("normal_map", normal);
    }
    // ------------------------------------------------------------------------
    if (ao) {
        bind_texture("ao_map", ao);
    }
    // ------------------------------------------------------------------------
    if (specular) {
        bind_texture("specular_map", specular);
    }
    // ------------------------------------------------------------------------
    if (height) {
        bind_texture("height_map", height);
    }
    // Binding extra textures
    for (const auto &t : extra_textures) {
        bind_texture(t.name, t.texture);
    }
}


