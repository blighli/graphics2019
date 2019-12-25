//
// Created by Krisu on 2019-12-03.
//

#ifndef RENDER_ENGINE_SCENEGRAPH_HPP
#define RENDER_ENGINE_SCENEGRAPH_HPP

#include "../basic/Object.hpp"
#include <vector>
#include <memory>

// TODO: A real Scene Graph
class SceneGraph : public Object {
public:
    SceneGraph() = default;
    inline void appendObject(Object *obj) { scene.push_back(obj); }
    void prepare() override {
        for (Object *objp : scene) {
            objp->prepare();
        }
    }
    void render() override {
        for (Object *objp : scene) {
            objp->render();
        }
    }

private:
    std::vector<Object *> scene;
};


#endif //RENDER_ENGINE_SCENEGRAPH_HPP
