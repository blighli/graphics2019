//
// Created by Krisu on 2019-12-03.
//

#ifndef RENDER_ENGINE_SCENEGRAPH_HPP
#define RENDER_ENGINE_SCENEGRAPH_HPP

#include <vector>
#include <memory>

#include "basic/Object.hpp"
#include "basic/Transform.hpp"
#include "basic/Camera.hpp"
#include "engine/SceneNode.hpp"

// SceneGraph -> Node -> Node -> Object
//            -> Node -> ...
//            -> ...
//
// SceneGraph       : entry of the graph
// SceneNode(interior) : responsible for lights and other special effects
// Obeject(leaf)    : real entity to be rendered

// TODO: A method to construct a scene graph from a scene description
//       file
class SceneGraph {
public:
    SceneGraph() = default;
    explicit SceneGraph(const std::string &scene_path) {
        Assimp::Importer ai_importer;
        const aiScene *ai_scene = ai_importer.ReadFile(scene_path,
                                                 aiProcess_Triangulate      |
                                             /*  aiProcess_FlipUVs          | */
                                                 aiProcess_GenSmoothNormals);

        if (!ai_scene || (ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
            !ai_scene->mRootNode) {
            throw std::runtime_error(std::string("assimp error: ") +
                                     ai_importer.GetErrorString());
        }
    }

    inline void setRoot(SceneNode *root) { this->p_root = root; }
    inline SceneNode *root() { return p_root; }

//    void prepareScene() {
//        prepareNode(p_root);
//    }
//    void renderScene() {
////        p_root->updateLights();
//        renderNode(p_root);
//    }

private:

private:
    SceneNode *p_root;
};


#endif //RENDER_ENGINE_SCENEGRAPH_HPP
