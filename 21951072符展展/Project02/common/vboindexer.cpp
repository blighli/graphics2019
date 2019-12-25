#include <vector>
#include <map>

#include <glm/glm.hpp>

#include "vboindexer.hpp"

#include <string.h> // for memcmp


// Returns true iif v1 can be considered equal to v2
bool is_near(float v1, float v2) {
    return fabs(v1 - v2) < 0.01f;
}

// Searches through all already-exported vertices
// for a similar one.
// Similar = same position + same UVs + same normal

struct PackedVertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;

    bool operator<(const PackedVertex that) const {
        return memcmp((void *) this, (void *) &that, sizeof(PackedVertex)) > 0;
    };
};

bool getSimilarVertexIndex_fast(
        PackedVertex &packed,
        std::map<PackedVertex, unsigned short> &VertexToOutIndex,
        unsigned short &result
) {
    std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
    if (it == VertexToOutIndex.end()) {
        return false;
    } else {
        result = it->second;
        return true;
    }
}

void indexVBO(
        std::vector<glm::vec3> &in_vertices,
        std::vector<glm::vec2> &in_uvs,
        std::vector<glm::vec3> &in_normals,

        std::vector<unsigned short> &out_indices,
        std::vector<glm::vec3> &out_vertices,
        std::vector<glm::vec2> &out_uvs,
        std::vector<glm::vec3> &out_normals
) {
    std::map<PackedVertex, unsigned short> VertexToOutIndex;

    // For each input vertex
    for (unsigned int i = 0; i < in_vertices.size(); i++) {

        PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};


        // Try to find a similar vertex in out_XXXX
        unsigned short index;
        bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

        if (found) { // A similar vertex is already in the VBO, use it instead !
            out_indices.push_back(index);
        } else { // If not, it needs to be added in the output data.
            out_vertices.push_back(in_vertices[i]);
            out_uvs.push_back(in_uvs[i]);
            out_normals.push_back(in_normals[i]);
            unsigned short newindex = (unsigned short) out_vertices.size() - 1;
            out_indices.push_back(newindex);
            VertexToOutIndex[packed] = newindex;
        }
    }
}


void indexVBO_TBN(
        std::vector<glm::vec3> &in_vertices,
        std::vector<glm::vec2> &in_uvs,
        std::vector<glm::vec3> &in_normals,
        std::vector<glm::vec3> &in_tangents,
        std::vector<glm::vec3> &in_bitangents,

        std::vector<unsigned short> &out_indices,
        std::vector<glm::vec3> &out_vertices,
        std::vector<glm::vec2> &out_uvs,
        std::vector<glm::vec3> &out_normals,
        std::vector<glm::vec3> &out_tangents,
        std::vector<glm::vec3> &out_bitangents
) {
    std::map<PackedVertex, unsigned short> VertexToOutIndex;
    // For each input vertex
    for (unsigned int i = 0; i < in_vertices.size(); i++) {
        PackedVertex packed{in_vertices[i], in_uvs[i], in_normals[i]};
        // Try to find a similar vertex in out_XXXX
        unsigned short index;
        bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

        if (found) { // A similar vertex is already in the VBO, use it instead !
            out_indices.push_back(index);

            // Average the tangents and the bitangents
            out_tangents[index] += in_tangents[i];
            out_bitangents[index] += in_bitangents[i];
        } else { // If not, it needs to be added in the output data.
            out_vertices.push_back(in_vertices[i]);
            out_uvs.push_back(in_uvs[i]);
            out_normals.push_back(in_normals[i]);
            out_tangents.push_back(in_tangents[i]);
            out_bitangents.push_back(in_bitangents[i]);
            out_indices.push_back((unsigned short) out_vertices.size() - 1);
        }
    }
}
