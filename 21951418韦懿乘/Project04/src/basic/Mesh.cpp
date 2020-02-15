//
// Created by Krisu on 2019-11-19.
//

#include <utility>
#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           unsigned int mesh_type) :
            vertices(std::move(vertices)), indices(std::move(indices)),
            mesh_type(mesh_type) {
    setupMesh();
}

void Mesh::render() const {
    // render mesh
    glBindVertexArray(VAO);
    glDrawElements(mesh_type, (int)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // VBO store vertices
    glGenBuffers(1, &EBO); // EBO store indices

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // Buiding EBO using indices from mFaces[]
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Positions
        glEnableVertexAttribArray(0); // parameter is the position.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              nullptr);
        // Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, normal));
        // Texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, texCoords));
    }
    glBindVertexArray(0);  // Unbind
}


namespace Primitive {
    // R = 1
    // TODO: Replace with a better sphere
    Mesh sphere() {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                float xSegment = (float) x / (float) X_SEGMENTS;
                float ySegment = (float) y / (float) Y_SEGMENTS;
                float xPos = std::cos(xSegment*2.f*PI) * std::sin(ySegment*PI);
                float yPos = std::cos(ySegment*PI);
                float zPos = std::sin(xSegment*2.0f*PI) * std::sin(ySegment*PI);
                vertices.push_back({ {xPos, yPos, zPos},
                                     {xPos, yPos, zPos},
                                     {xSegment, ySegment} });
            }
        }

        for (int y = 0; y < Y_SEGMENTS; ++y) {
            if (y % 2 == 0) {
                for (int x = 0; x <= X_SEGMENTS; ++x) {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            } else {
                for (int x = X_SEGMENTS; x >= 0; --x) {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
        }

        return std::move(Mesh(vertices, indices, GL_TRIANGLE_STRIP));
    }

    // L = 2
    Mesh cube() {
        const static float vdata[] = {
                // position, normal, uv
                // back face
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
                // front face
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                // left face
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                // right face
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
                // bottom face
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
                1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                // top face
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
        };
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        for (int i = 0; i < 36; i++) {
            vertices.push_back({ glm::make_vec3(vdata + i*8),
                                 glm::make_vec3(vdata + i*8 + 3),
                                 glm::make_vec2(vdata + i*8 + 6) });
            auto v = glm::make_vec3(vdata + i*8 + 3);
            indices.push_back(i);
        }
        return std::move(Mesh(vertices, indices));
    }

    // L = 2
    Mesh quad() {
        const static float vdata[] = {
                // positions, normal, uv
                -1.0f,  1.0f, 0.0f,  0.f, 0.f, 1.f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f,  0.f, 0.f, 1.f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f,  0.f, 0.f, 1.f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f,  0.f, 0.f, 1.f, 1.0f, 0.0f,
        };
        std::vector<Vertex> vertices;
        std::vector<unsigned  int> indices { 0, 1, 2, 3 };
        vertices.reserve(4);
        for (int i = 0; i < 4; i++) {
            vertices.push_back({ glm::make_vec3(vdata + i*8),
                                 glm::make_vec3(vdata + i*8 + 3),
                                 glm::make_vec2(vdata + i*8 + 6) });
        }
        return std::move(Mesh(vertices, indices, GL_TRIANGLE_STRIP));
    }

    void renderCube() {
        static auto s = sphere();
        s.render();
    }

    void renderSphere() {
        static auto c = cube();
        c.render();
    }

    void renderQuad() {
        static auto q = quad();
        q.render();
    }
}