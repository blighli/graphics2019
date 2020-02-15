//
// Created by Krisu on 2019-11-18.
//

#include "Model.hpp"
#include <stb_image.h>

// loadModel -> processNode -> processMesh
//                          -> processNode(child)

void Model::loadModel(const std::string &path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path,
                                           aiProcess_Triangulate |
                                           aiProcess_FlipUVs |
                                           aiProcess_GenSmoothNormals);

    // check root node and flags of imcomplete after import
    if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
        !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        exit(-1);
    }
    directory = path.substr(0, path.find_last_of('/'));

    // pass first node (root node) to recursive processNode
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // do the same for each child
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Model::Model(const std::string &directory) {
    loadModel(directory);
}

void Model::render() {
    // TODO: bind textures
    for (auto &mesh : meshes) {
        mesh.render();
    }
}

std::vector<Texture> Model::loadMaterialTextures(
        aiMaterial *mat,
        aiTextureType type,
        const std::string &typeName) {

    //TODO: Fix Texture issues
    std::vector<Texture> textures;
//    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
//        aiString str;
//        mat->GetTexture(type, i, &str);
//        bool skip = false;
//        for (auto &t : textures_loaded) {
//            if (std::strcmp(t.path().c_str(), str.C_Str()) == 0) {
//                textures.push_back(t);
//                skip = true;
//                break;
//            }
//        }
//        if (!skip) {
//            // not already loaded before
//            Texture texture(str.C_Str(), typeName);
//            textures.push_back(texture);
//            textures_loaded.push_back(texture);
//        }
//    }
    return textures;
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
        // process each vertex positions
        // position:
        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                           mesh->mVertices[i].z};
        // normal:
        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y,
                         mesh->mNormals[i].z};

        // texture coordinates
        if (mesh->mTextureCoords[0]) { // contain?
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;

            vertex.texCoords = vec;
        } else {
            vertex.texCoords = glm::vec2(0.0, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace &face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // TODO: fix with rust-steel pipeline
    // process material
//    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
//    std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
//                                                            aiTextureType_DIFFUSE,
//                                                            "texture_diffuse");
//    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//    std::vector<Texture> specularMaps = loadMaterialTextures(material,
//                                                             aiTextureType_SPECULAR,
//                                                             "texture_specular");
//    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    return Mesh(vertices, indices);
}









