//
// Created by Krisu on 2019-11-19.
//

#ifndef RENDER_ENGINE_MESH_HPP
#define RENDER_ENGINE_MESH_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <vector>
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.hpp"
#include "Texture.hpp"

class Vertex {
public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

// Only respond for Triangular Meshes
// Leave textures for others to concern
class Mesh {
public:
    Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,
         unsigned int mesh_type=GL_TRIANGLES);

    void render() const;

public:
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

private:
    // Called in constructor
    void setupMesh();
    unsigned int mesh_type;
};

// Rendering some fixed size primitive
namespace Primitive {
    // Unit == 2 :)
    Mesh sphere();
    Mesh cube();
    Mesh quad();

    void renderCube();
    void renderSphere();
    void renderQuad();
}

#endif //RENDER_ENGINE_MESH_HPP
