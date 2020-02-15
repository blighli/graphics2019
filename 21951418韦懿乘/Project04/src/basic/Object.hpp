//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_OBJECT_HPP
#define RENDER_ENGINE_OBJECT_HPP

#include "basic/Shader.hpp"
#include "basic/Transform.hpp"
#include "basic/Material.hpp"
#include "basic/Mesh.hpp"

// Represents an entity in scene
// TODO: make Object an entity that holds Mesh, Shaders, Transform,
//       Textures.
class Object {
public:
    Object(Mesh *mesh, Material *material) : mesh(mesh), material(material) {}
    virtual void prepare() = 0;
    virtual void render() = 0;
    virtual ~Object() = default;

public:
    // The world transformation of the current object is the parent's
    // world transformation multiplying the object's local transformation
    Transform localTransfrom;

    Mesh *mesh;
    Material *material;
};


#endif //RENDER_ENGINE_OBJECT_HPP
