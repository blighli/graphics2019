//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_ENGINE_APPLICATION_HPP
#define RENDER_ENGINE_APPLICATION_HPP

// when using glfw3, glad must be included
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../basic/Shader.hpp"
#include "../basic/Camera.hpp"
#include "../basic/Texture.hpp"
#include <glm/glm.hpp>
#include "../objects/Skybox.hpp"
#include "../basic/Light.hpp"
#include "../objects/CookTorrancePbrObj.hpp"

// The base class of App
class Application {
public:
    Application(int argc, char *argv[]);
    static void initializeContext();

    inline int exec() {
        prepareUI();
        initializeScene();
        while (!applicationEnds) {
            renderPass();
        }
        cleanUp();
        return 0;
    }

private:
    void initializeScene();
    void processKeyboard();
    void renderPass();
    void renderScene();
    void cleanUp();
    void prepareUI();

private:
    bool applicationEnds = false;
    static GLFWwindow *window;
    static std::string glsl_version;

private:
    // TODO: manage scene with scene graph
    // Scnene configuration
    Camera camera;
    std::vector<Light> lights;
    Skybox skybox;
    CookTorrancePbrObj ctPbrObj;


    // GUI controls variables
    struct {
        glm::vec3 albedo     = {1.f, 1.f, 1.f};
        float     roughness  = 0.010;
        float     metallic   = 0.900;
        bool      useTexture = false;
        bool      useModel   = true;
    } ui;
};

#endif //RENDER_ENGINE_APPLICATION_HPP
