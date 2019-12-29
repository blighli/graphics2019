//
// Created by Krisu on 2019-11-19.
//

#ifndef RENDER_ENGINE_MESH_HPP
#define RENDER_ENGINE_MESH_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <vector>
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>

#include "Shader.hpp"
#include "Texture.hpp"

class Vertex {
public:
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

// Only respond for Triangular Meshes
// Leave textures for others to concern
class Mesh {
public:
    Mesh(std::vector<Vertex> vertices,
    std::vector<unsigned int> indices);

    void render();

public:
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

private:
    void setupMesh();
};

#endif //RENDER_ENGINE_MESH_HPP
