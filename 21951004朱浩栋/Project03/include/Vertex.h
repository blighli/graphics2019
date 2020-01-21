//
// Created by Moving on 2019/12/2 0002.
//

#ifndef MYGL_VERTEX_H
#define MYGL_VERTEX_H

#include <CELLMath.hpp>
#include <string>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <stb_image.h>
#include <assimp/postprocess.h>
//!顶点结构：位置、法线、纹理
struct Vertex {
    CELL::tvec3<float> Position;
    CELL::tvec3<float> Normal;
    CELL::tvec2<float> TexCoords;
};
//!纹理结构：编号、材质
struct Texture {
    unsigned int id;//!
    std::string type;
    aiString path;
};
//!网格结构：顶点、索引、纹理
//!构造：给定所有网格数据进行构造
//!绘制网格内的所有内容
//!Private：和shader对接
class Mesh {
public:
    //!  网格数据
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;//!
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Shader shader);
    void Draw(Shader shader);
private:
    //!  渲染数据
    unsigned int VAO, VBO, EBO;

    void setupMesh(Shader shader);
};

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Shader shader){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh(shader);
}

void Mesh::setupMesh(Shader shader) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);


    glVertexAttribPointer(shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(shader._normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glVertexAttribPointer(shader._uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);

}

void Mesh::Draw(Shader shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); //! 在绑定之前激活相应的纹理单元
        //! 获取纹理序号（ diffuse_textureN 中的 N ）
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        glUniform1i(shader.,i)
        shader.setFloat(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    //! 绘制网格
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
//!模型：网格
class Model
{
public:
    //!  函数
    Model(char *path, Shader shader)
    {
        loadModel(path,shader);
    }
    void Draw(Shader shader);
private:
    //!模型数据
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    //!  函数
    void loadModel(std::string path, Shader shader);
    void processNode(aiNode *node, const aiScene *scene, Shader shader);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, Shader shader);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         std::string typeName);
};

class vec3;

void Model::Draw(Shader shader)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}
//!使用loadModel来构造一个model
void Model::loadModel(std::string path, Shader shader)
{
    Assimp::Importer import;
    //!scene
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    //!directory用来记忆model的路径
    directory = path.substr(0, path.find_last_of('/'));
    //!在进行loadModel之后紧接着processNode
    processNode(scene->mRootNode, scene, shader);
}

void Model::processNode(aiNode *node, const aiScene *scene, Shader shader)
{
    //! 处理节点所有的网格（如果有的话）
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, shader));
    }
    //! 接下来对它的子节点重复这一过程
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, shader);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, Shader shader)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        //! 处理顶点位置、法线和纹理坐标
        CELL::tvec3<float> tmpVec;
        tmpVec.x = mesh->mVertices[i].x;
        tmpVec.y = mesh->mVertices[i].y;
        tmpVec.z = mesh->mVertices[i].z;
        vertex.Position = tmpVec;
        tmpVec.x = mesh->mNormals[i].x;
        tmpVec.y = mesh->mNormals[i].y;
        tmpVec.z = mesh->mNormals[i].z;
        vertex.Normal = tmpVec;
        if(mesh->mTextureCoords[0]) //! 网格是否有纹理坐标？
        {
            CELL::tvec2<float>  vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords =  CELL::tvec2<float>(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    //! 处理索引
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    //! 处理材质
    if(mesh->mMaterialIndex >= 0)
    {
        //!去scene->mMaterials中寻找材质
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                           aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,
                                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures, shader);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {   //! 如果纹理还没有被加载，则加载它
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // 添加到已加载的纹理中
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
#endif //MYGL_VERTEX_H
