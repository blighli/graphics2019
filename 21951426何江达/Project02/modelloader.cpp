#include "modelloader.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <QDebug>
#include <limits>
#include <QImage>
#include <QOpenGLTexture>
#include <QVector2D>

ModelLoader::ModelLoader(bool transformToUnitCoordinates) :
    m_transformToUnitCoordinates(transformToUnitCoordinates)
{

}

// look for file using relative path
QString findFile(QString relativeFilePath, int scanDepth)
{
    QString str = relativeFilePath;
    for (int ii = -1; ii < scanDepth; ++ii)
    {
        if (QFile::exists(str))
        {
            return str;
        }
        str.prepend("../");
    }
    return "";
}

bool ModelLoader::Load(QString filePath, PathType pathType)
{
    QString l_filePath;
    if (pathType == RelativePath)
        l_filePath = findFile(filePath, 5);
    else
        l_filePath = filePath;


    directory = QString::fromStdString(filePath.toStdString().substr(0, filePath.toStdString().find_last_of('/')));
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile( l_filePath.toStdString(),
                                              aiProcess_GenSmoothNormals      |
                                              aiProcess_CalcTangentSpace       |
                                              aiProcess_Triangulate       |
                                              aiProcess_JoinIdenticalVertices  |
                                              aiProcess_SortByPType
                                            );

    if ( !scene)
    {
        qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
        return false;
    }

    if (scene->HasMaterials())
    {
        for (unsigned int ii = 0; ii < scene->mNumMaterials; ++ii)
        {
            QSharedPointer<MaterialInfo> mater = processMaterial(scene->mMaterials[ii]);
            m_materials.push_back(mater);
        }
    }

    if (scene->HasMeshes())
    {
        for (unsigned int ii = 0; ii < scene->mNumMeshes; ++ii)
        {
            m_meshes.push_back(processMesh(scene->mMeshes[ii]));
        }
    }
    else
    {
        qDebug() << "Error: No meshes found";
        return false;
    }

    if (scene->HasLights())
    {
        qDebug() << "Has Lights";
    }

    if (scene->mRootNode != NULL)
    {
        Node* rootNode = new Node;
        processNode(scene, scene->mRootNode, 0, *rootNode);
        m_rootNode.reset(rootNode);
    }
    else
    {
        qDebug() << "Error loading model";
        return false;
    }

    // This will transform the model to unit coordinates, so a model of any size or shape will fit on screen
    if (m_transformToUnitCoordinates)
        transformToUnitCoordinates();

    return true;
}

void ModelLoader::getBufferData( QVector<float>** vertices, QVector<float>** normals, QVector<unsigned int>** indices, QVector<float>** texCoords)
{
    if (vertices != 0)
        *vertices = &m_vertices;

    if (normals != 0)
        *normals = &m_normals;

    if (indices != 0)
        *indices = &m_indices;
    if (texCoords != 0)
        *texCoords = &m_textureCoord;
}

void ModelLoader::getTextureData(QVector<QVector<float> >** textureUV, QVector<float>** tangents, QVector<float>** bitangents)
{
    if (textureUV != 0)
        *textureUV = &m_textureUV;

    if (tangents != 0)
        *tangents = &m_tangents;

    if (bitangents != 0)
        *bitangents = &m_bitangents;
}
//!
QSharedPointer<MaterialInfo> ModelLoader::processMaterial(aiMaterial* material)
{
    QSharedPointer<MaterialInfo> mater(new MaterialInfo);

    aiString mname;
    material->Get( AI_MATKEY_NAME, mname);
    if (mname.length > 0)
        mater->Name = mname.C_Str();

    int shadingModel;
    material->Get( AI_MATKEY_SHADING_MODEL, shadingModel );

    if (shadingModel != aiShadingMode_Phong && shadingModel != aiShadingMode_Gouraud)
    {
        qDebug() << "This mesh's shading model is not implemented in this loader, setting to default material";
        mater->Name = "DefaultMaterial";
    }
    else
    {
        aiColor3D dif (0.f, 0.f, 0.f);
        aiColor3D amb (0.f, 0.f, 0.f);
        aiColor3D spec (0.f, 0.f, 0.f);
        float shine = 0.0;

        material->Get( AI_MATKEY_COLOR_AMBIENT, amb);
        material->Get( AI_MATKEY_COLOR_DIFFUSE, dif); //->Get(<material-key>,<where-to-store>))
        material->Get( AI_MATKEY_COLOR_SPECULAR, spec);
        material->Get( AI_MATKEY_SHININESS, shine);

        mater->Ambient = QVector3D(amb.r, amb.g, amb.b);
        mater->Diffuse = QVector3D(dif.r, dif.g, dif.b);
        mater->Specular = QVector3D(spec.r, spec.g, spec.b);
        mater->Shininess = shine;

        mater->Ambient *= .2f;
        if ( mater->Shininess == 0.0)
            mater->Shininess = 30;
    }

    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN
    mater->hasTexture = false;
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0 ||
            material->GetTextureCount(aiTextureType_SPECULAR) > 0 ||
            material->GetTextureCount(aiTextureType_HEIGHT) > 0 ||
            material->GetTextureCount(aiTextureType_AMBIENT) > 0
       )
    {
        mater->hasTexture = true;
        // 1. diffuse maps
        QVector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        mater->tex.append(diffuseMaps);
//    mater->tex.insert(mater->tex.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        QVector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        mater->tex.append(specularMaps);
//    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        QVector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        mater->tex.append(normalMaps);
//    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        QVector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        mater->tex.append(heightMaps);
//    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }
    return mater;
}
//! 对网格进行处理
QSharedPointer<Mesh> ModelLoader::processMesh(aiMesh* mesh)
{
    QSharedPointer<Mesh> newMesh(new Mesh);
    newMesh->name = mesh->mName.length != 0 ? mesh->mName.C_Str() : "";
    newMesh->indexOffset = m_indices.size();
    unsigned int indexCountBefore = m_indices.size();
    int vertindexoffset = m_vertices.size() / 3;

    // Get Vertices
    if (mesh->mNumVertices > 0)
    {
        for (uint ii = 0; ii < mesh->mNumVertices; ++ii)
        {
            aiVector3D& vec = mesh->mVertices[ii];

            m_vertices.push_back(vec.x);
            m_vertices.push_back(vec.y);
            m_vertices.push_back(vec.z);
        }
    }

    // Get Normals
    if (mesh->HasNormals())
    {
        for (uint ii = 0; ii < mesh->mNumVertices; ++ii)
        {
            aiVector3D& vec = mesh->mNormals[ii];
            m_normals.push_back(vec.x);
            m_normals.push_back(vec.y);
            m_normals.push_back(vec.z);
        };
    }
    // texture coordinates
    for (uint ii = 0; ii < mesh->mNumVertices; ++ii)
    {
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            m_textureCoord.push_back(mesh->mTextureCoords[0][ii].x);
            m_textureCoord.push_back(mesh->mTextureCoords[0][ii].y);
//            qDebug() << mesh->mTextureCoords[0][ii].x << " " << mesh->mTextureCoords[0][ii].y;
        }
        else
        {
            m_textureCoord.push_back(0.);
            m_textureCoord.push_back(0.);
//            qDebug() << 0 << " " << 0;
        }

    }

    // Get Texture coordinates
    if (mesh->GetNumUVChannels() > 0)
    {
        if ((unsigned int)m_textureUV.size() < mesh->GetNumUVChannels()) // Caution, assumes all meshes in this model have same number of uv channels
        {
            m_textureUV.resize(mesh->GetNumUVChannels());
            m_textureUVComponents.resize(mesh->GetNumUVChannels());
        }

        for ( unsigned int ich = 0; ich < mesh->GetNumUVChannels(); ++ich)
        {
            m_textureUVComponents[ich] = mesh->mNumUVComponents[ich];

            for (uint iind = 0; iind < mesh->mNumVertices; ++iind)
            {
                m_textureUV[ich].push_back(    mesh->mTextureCoords[ich][iind].x);
                if (mesh->mNumUVComponents[ich] > 1)
                {
                    m_textureUV[ich].push_back(mesh->mTextureCoords[ich][iind].y);
                    if (mesh->mNumUVComponents[ich] > 2)
                    {
                        m_textureUV[ich].push_back(    mesh->mTextureCoords[ich][iind].z);
                    }
                }
            }
        }
    }



    // Get Tangents and bitangents
    if (mesh->HasTangentsAndBitangents())
    {
        //addTangentsAndBitangents(mesh, m_tangents, m_bitangents);
        for (uint ii = 0; ii < mesh->mNumVertices; ++ii)
        {
            aiVector3D& vec = mesh->mTangents[ii];
            m_tangents.push_back(vec.x);
            m_tangents.push_back(vec.y);
            m_tangents.push_back(vec.z);

            aiVector3D& vec2 = mesh->mBitangents[ii];
            m_bitangents.push_back(vec2.x);
            m_bitangents.push_back(vec2.y);
            m_bitangents.push_back(vec2.z);
        };
    }

    // Get mesh indexes
    for (uint t = 0; t < mesh->mNumFaces; ++t)
    {
        aiFace* face = &mesh->mFaces[t];
        if (face->mNumIndices != 3)
        {
            qDebug() << "Warning: Mesh face with not exactly 3 indices, ignoring this primitive." << face->mNumIndices;
            continue;
        }

        m_indices.push_back(face->mIndices[0] + vertindexoffset);
        m_indices.push_back(face->mIndices[1] + vertindexoffset);
        m_indices.push_back(face->mIndices[2] + vertindexoffset);
    }

    newMesh->indexCount = m_indices.size() - indexCountBefore;
    newMesh->material = m_materials.at(mesh->mMaterialIndex);

    return newMesh;
}
//!  加载模型
QVector<Texture> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, QString typeName)
{
    QVector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < m_textures_loaded.size(); j++)
        {
            if (std::strcmp(m_textures_loaded[j].path.toStdString().data(), str.C_Str()) == 0)
            {
                textures.push_back(m_textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {
            // if texture hasn't been loaded already, load it
            Texture texture;
            QString filename = QString(str.C_Str());

            filename = directory + '/' + filename;
//            qDebug() << filename;
            texture.texture = new QOpenGLTexture(QImage(filename).mirrored());
            texture.texture->setWrapMode(QOpenGLTexture::Repeat);
            texture.texture->generateMipMaps();
            texture.texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture.texture->setMagnificationFilter(QOpenGLTexture::Linear);

//            texture.id = TextureFromFile(str.C_Str(), this->directory.toStdString());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m_textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

void ModelLoader::processNode(const aiScene* scene, aiNode* node, Node* parentNode, Node& newNode)
{
    newNode.name = node->mName.length != 0 ? node->mName.C_Str() : "";

    newNode.transformation = QMatrix4x4(node->mTransformation[0]);

    newNode.meshes.resize(node->mNumMeshes);
    for (uint imesh = 0; imesh < node->mNumMeshes; ++imesh)
    {
        QSharedPointer<Mesh> mesh = m_meshes[node->mMeshes[imesh]];
        newNode.meshes[imesh] = mesh;
    }

    for (uint ich = 0; ich < node->mNumChildren; ++ich)
    {
        newNode.nodes.push_back(Node());
        processNode(scene, node->mChildren[ich], parentNode, newNode.nodes[ich]);
    }
}

void ModelLoader::transformToUnitCoordinates()
{
    // This will transform the model to unit coordinates, so a model of any size or shape will fit on screen

    double amin = std::numeric_limits<double>::max();
    double amax = std::numeric_limits<double>::min();
    QVector3D minDimension(amin, amin, amin);
    QVector3D maxDimension(amax, amax, amax);

    // Get the minimum and maximum x,y,z values for the model
    findObjectDimensions(m_rootNode.data(), QMatrix4x4(), minDimension, maxDimension);

    // Calculate scale and translation needed to center and fit on screen
    float dist = qMax(maxDimension.x() - minDimension.x(), qMax(maxDimension.y() - minDimension.y(), maxDimension.z() - minDimension.z()));
    float sc = 1.0 / dist;
    QVector3D center = (maxDimension - minDimension) / 2;
    QVector3D trans = -(maxDimension - center);

    // Apply the scale and translation to a matrix
    QMatrix4x4 transformation;
    transformation.setToIdentity();
    transformation.scale(sc);
    transformation.translate(trans);

    // Multiply the transformation to the root node transformation matrix
    m_rootNode.data()->transformation = transformation * m_rootNode.data()->transformation;
}

void ModelLoader::findObjectDimensions(Node* node, QMatrix4x4 transformation, QVector3D& minDimension, QVector3D& maxDimension)
{
    transformation *= node->transformation;

    for (int ii = 0; ii < node->meshes.size(); ++ii)
    {
        int start = node->meshes[ii]->indexOffset;
        int end = start + node->meshes[ii]->indexCount;
        for (int ii = start; ii < end; ++ii)
        {
            int ind = m_indices[ii] * 3;
            QVector4D vec(m_vertices[ind], m_vertices[ind + 1], m_vertices[ind + 2], 1.0);
            vec = transformation * vec;

            if (vec.x() < minDimension.x())
                minDimension.setX(vec.x());
            if (vec.y() < minDimension.y())
                minDimension.setY(vec.y());
            if (vec.z() < minDimension.z())
                minDimension.setZ(vec.z());
            if (vec.x() > maxDimension.x())
                maxDimension.setX(vec.x());
            if (vec.y() > maxDimension.y())
                maxDimension.setY(vec.y());
            if (vec.z() > maxDimension.z())
                maxDimension.setZ(vec.z());
        }
    }

    for (int ii = 0; ii < node->nodes.size(); ++ii)
    {
        findObjectDimensions(&(node->nodes[ii]), transformation, minDimension, maxDimension);
    }
}
