//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_OBJECT_HPP
#define RENDER_ENGINE_OBJECT_HPP

#include "Shader.hpp"

class Object {
public:
    explicit Object(const Shader &renderShader): renderShader(renderShader) { }

    virtual void prepare() = 0;
    virtual void render() = 0;

    template <typename ...Args>
    inline void setShaderUnif(const std::string &name, Args &&...args) {
        renderShader.use();
        renderShader.setValue(name, std::forward<Args>(args) ...);
    }

protected:
    // The shader that is respondsible for rendering the obj
    Shader renderShader;
};


#endif //RENDER_ENGINE_OBJECT_HPP
