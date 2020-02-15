//
// Created by Krisu on 2019-12-09.
//

#ifndef RENDER_ENGINE_TRANSFORM_HPP
#define RENDER_ENGINE_TRANSFORM_HPP

#include <glm/gtc/matrix_transform.hpp>
#include "basic/Shader.hpp"

// TODO: separate vertex only behaviour to Transform
// A transform is append to a node/objects in scene graph, to perform a local
// transformation according to the local space of its parent node
class Transform {
public:
    Transform(): mat(glm::identity<glm::mat4>()) {}
    Transform(const Transform &) = default;
    Transform &operator=(const Transform &) = default;

    inline void append(glm::mat4 rhs) { mat *= rhs; }
    void setShaders(Shader &shader) {
    }

private:
    glm::mat4 mat;
};


#endif //RENDER_ENGINE_TRANSFORM_HPP
