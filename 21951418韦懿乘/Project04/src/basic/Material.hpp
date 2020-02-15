//
// Created by Krisu on 2019-12-29.
//

#ifndef RENDER_ENGINE_MATERIAL_HPP
#define RENDER_ENGINE_MATERIAL_HPP

#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "basic/Texture.hpp"
#include "basic/Shader.hpp"

// Hold and manage all the textures, responsible for binding texture to shader
// All suppord
class Material {
public:
    Material() = default;

    // Replace material shader with a user-modified one
    inline void setShader(Shader *new_shader);

    inline void setAlbedo(Texture *a);
    inline void setAlbedo(glm::vec3 a);
    inline void setAlbedo(float r, float g, float b);

    inline void setMetallic(Texture *m);
    inline void setMetallic(float m);

    inline void setRoughness(Texture *r);
    inline void setRoughness(float r);

    inline void setEmissive(Texture *e);
    inline void setEmissive(float e);

    inline void setNormal(Texture *n);
    inline void setAO(Texture *a);
    inline void setSpecular(Texture *s);
    inline void setHeight(Texture *h);

    inline void appendTexture(const std::string &name, Texture *t);

    void updateShader();

private:
    void bind_texture(const std::string &str, Texture *texture);

public:
    // Shader responsible for this texture
    Shader *shader = new Shader {"shaders/cook-torrance.vert",
                                 "shaders/cook-torrance.frag"};

private:
    // All supported general use parameter(tex/val) type:
    struct { Texture *map; glm::vec3 value; } albedo = { nullptr, {0, 0, 0} };
    struct { Texture *map; float value; } metallic = { nullptr, 0 };
    struct { Texture *map; float value; } roughness = { nullptr, 0 };
    struct { Texture *map; float value; } emissive = { nullptr, 0 };
    Texture *normal = nullptr;
    Texture *ao = nullptr;
    Texture *specular = nullptr;
    Texture *height = nullptr;
    // Parameter status:
    struct {
        bool albedo = false;
        bool metallic = false;
        bool specular = false;
        bool roughness = false;
        bool emissive = false;
    } use_map;
    // Append some extra shader specific texture
    struct ExtraTexture {
        std::string name;
        Texture *texture;
    };
    std::vector<ExtraTexture> extra_textures;
    int current_used_texture_units = 0;
};







// ----------------------Inline functions----------------------------
void Material::appendTexture(const std::string &name, Texture *t) {
    extra_textures.push_back({name, t});
}

void Material::setShader(Shader *new_shader) {
    delete shader; shader = new_shader; }

void Material::setAlbedo(Texture *a)  {
    use_map.albedo = true; albedo.map = a; }

void Material::setAlbedo(glm::vec3 a) { use_map.albedo = false; albedo.value = a; }

void Material::setAlbedo(float r, float g, float b) { setAlbedo({r, g, b}); }

void Material::setMetallic(Texture *m) { use_map.metallic = true; metallic.map = m; }

void Material::setMetallic(float m)    { use_map.metallic = false; metallic.value = m; }

void Material::setRoughness(Texture *r) { use_map.roughness = true; roughness.map = r; }

void Material::setRoughness(float r)    { use_map.roughness = false; roughness.value = r;; }

void Material::setEmissive(Texture *e) { use_map.emissive = true; emissive.map = e; }

void Material::setEmissive(float e)    { use_map.emissive = false; emissive.value = e; }

void Material::setNormal(Texture *n) { normal = n; }

void Material::setAO(Texture *a)     { ao = a; }

void Material::setSpecular(Texture *s) { specular = s; }

void Material::setHeight(Texture *h)   { height = h; }


#endif //RENDER_ENGINE_MATERIAL_HPP
