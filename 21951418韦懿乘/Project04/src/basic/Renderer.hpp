//
// Created by Krisu on 2019-12-29.
//

#ifndef RENDER_ENGINE_RENDERER_HPP
#define RENDER_ENGINE_RENDERER_HPP

#include "basic/Object.hpp"

// TODO: encapsulate all opengl associated API into renderer
class Renderer {
public:
    Renderer() = default;
    void draw(const Object &object);
    void draw(const Mesh &mesh) {
        mesh.render();
    }

private:
    // Storing state of graphics API:

};


#endif //RENDER_ENGINE_RENDERER_HPP
