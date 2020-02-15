//
// Created by Krisu on 2019-11-22.
//

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "engine/Application.hpp"
#include "Model.hpp"
#include "objects/Skybox.hpp"
#include "basic/Material.hpp"
#include "engine/SceneGraph.hpp"
#include "Application.hpp"

std::string Application::glsl_version;
GLFWwindow *Application::window = nullptr;

static void glfwErrorCallback(int error, const char *description) {
    std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

void Application::initializeContext(){
    // Setup ImGui context
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        exit(-1);
    }

    glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int width = 1280, height = 720;
    window = glfwCreateWindow(width, height, "Render engine", nullptr, nullptr);
    if (!window) {
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    // vsync
    glfwSwapInterval(1);
    // MSAA
//    glfwWindowHint(GLFW_SAMPLES, 4);
//    glEnable(GL_MULTISAMPLE);

    if (gladLoadGL() == 0) {
        std::cerr << "Failed to initialize glad loader\n";
        exit(-1);
    }
}



void Application::prepareUI() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}


void Application::renderPass() {
    applicationEnds = glfwWindowShouldClose(window);

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool show_another_window = false;
    static ImVec4 ball_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    // A simple window
    {
        static float f = 0.0f;
        static int cnt = 0;

        ImGui::Begin("Settings");

        ImGui::Text("Properties: ");
        ImGui::Checkbox("Use Texture", &ui.useTexture);
        ImGui::Checkbox("Use Model", &ui.useModel);

        ImGui::SliderFloat("Roughness", &ui.roughness, 0.001f, 1.0f);
        ImGui::SliderFloat("Metallic", &ui.metallic, 0.001f, 1.0f);
        ImGui::ColorEdit3("Albedo", (float *)&ui.albedo.x);

        if (ImGui::Button("cnt plus plus")) {
            cnt++;
        }
        ImGui::Checkbox("Show another window", &show_another_window);
        ImGui::SameLine();
        ImGui::Text("cnt = %d", cnt);

        ImGui::Text("Frame rate: %.1f FPS", ImGui::GetIO().Framerate);

        ImGui::End();
    }
    // Another window
    if (show_another_window) {
        ImGui::Begin("Another window", &show_another_window);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("close")) {
            show_another_window = false;
        }
        ImGui::End();
    }
    processKeyboard();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Rendering scene here
    renderScene();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}



void Application::prepareScene() {
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    skybox.prepare();

    // TODO: auto scene graph construction from scene description file
    // TODO: Scene Graph
}



void Application::renderScene() {
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // -------------------Preparing----------------------------
    static std::vector<PointLight> lights = {
            {{-10.0f, 10.0f,  0.0f}, {300.0f, 300.0f, 300.0f}},
            {{10.0f,  10.0f,  0.0f}, {300.0f, 300.0f, 300.0f}},
            {{-10.0f, -10.0f, 0.0f}, {300.0f, 300.0f, 300.0f}},
            {{10.0f,  -10.0f, 0.0f}, {300.0f, 300.0f, 300.0f}},
    };
    static Mesh sphere_mesh = Primitive::sphere();
    static Mesh cube_mesh = Primitive::cube();
    static auto *M_block = new Material;
    static auto *M_ball = new Material;
    static auto *M_strip = new Material;

    static std::vector<Material*> materials { M_block, M_ball, M_strip };

    static bool first_run = true;
    if (first_run) {
        auto *irradiance_map = new Texture{skybox.getIrradianceMap(),
                                           TextureType::CubeMap};
        auto *prefilter_map = new Texture{skybox.getPrefilterMap(),
                                          TextureType::CubeMap};
        auto *brdfLUT_map = new Texture{skybox.getBrdfLUTTexture(),
                                        TextureType::Texture2D};
        for (auto material : materials) {
            material->appendTexture("irradiance_map", irradiance_map);
            material->appendTexture("prefilter_map", prefilter_map);
            material->appendTexture("brdfLUT_map", brdfLUT_map);
        }
        camera.Position = {-4.019716, 0.732713, 7.832322};
    }
    // Global Settings update for each pass

    int w, h;
    glfwGetWindowSize(window, &w, &h);
    auto projection = glm::perspective(glm::radians(camera.Zoom), (float)w/h, 0.1f, 100.0f);
    auto view       = camera.GetViewMatrix();

    for (auto material : materials) {
        material->shader->set("projection", projection);
        material->shader->set("view", view);
        material->shader->set("cam_pos", camera.Position);

        for (int i = 0; i < 4; i++) {
            auto str_i = std::to_string(i);
            material->shader->set("lightPositions["+str_i+"]",
                                  lights[i].position + glm::vec3(sin(glfwGetTime()*5)));
            material->shader->set("lightColors["+str_i+"]", lights[i].color);
        }
    }

    // setting material
    M_block->shader->set("use_texture", 0);
    M_block->setAlbedo({1, 1, 1});
    M_block->setRoughness(0.8);
    M_block->setMetallic(0.25);
    M_block->updateShader();

    M_ball->shader->set("use_texture", 0);
    M_ball->setAlbedo({0.0, 0.0, 0.5});
    M_ball->setRoughness(0.1);
    M_ball->setMetallic(0.95);
    M_ball->updateShader();



    static constexpr const size_t BW = 7, BH = 8;
    static int blocks_state[BH][BW] = {
            1, 0, 1, 1, 1, 1, 1,
            1, 0, 1, 0, 0, 0, 1,
            1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 1, 1, 0, 1,
            1, 0, 0, 0, 0, 0, 1,
            1, 1, 1, 1, 1, 1, 1
    };


    static const float BLOCK_SIZE = 4.f;
    for (int i = 0; i < BH; i++) {
        for (int j = 0; j < BW; j++) {
            float x = (j - BH/2.) * BLOCK_SIZE;
            float y = (BH-i-1 -BH/2. + 3) * BLOCK_SIZE;
            float z = 0;
            glm::mat4 block_model = glm::mat4(1.f);
            glm::mat4 bottom_block_model;

            block_model = glm::rotate(block_model, glm::radians(90.f), {1, 0, 0});
            block_model = glm::translate(block_model, {x, y, z});
            bottom_block_model = block_model;
            bottom_block_model = glm::translate(block_model, {0, 0, 4});
            block_model = glm::scale(block_model, {2, 2, 2});
            bottom_block_model = glm::scale(bottom_block_model, {2, 2, 2});

            M_block->shader->set("model", block_model);
            M_block->setAlbedo(glm::vec3(1.f) - glm::vec3((1.f*i+j)/(BW+BH)));
            M_block->updateShader();
            if (blocks_state[i][j] == 1) {
                cube_mesh.render();
            }
            M_block->shader->set("model", bottom_block_model);
            cube_mesh.render();
        }
    }
    printf("(%f, %f, %f)\n", camera.Position.x, camera.Position.y, camera.Position.z);

    // Sky box
    skybox.shader.set("view", view);
    skybox.shader.set("projection", projection);
    skybox.render();

    first_run = false;
}



void Application::cleanUp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}



void Application::processKeyboard() {
    float deltaTime = 1.0 / ImGui::GetIO().Framerate;

#define if_press(GLFW_KEY_XXX, function_call) \
    if (glfwGetKey(window, GLFW_KEY_XXX) == GLFW_PRESS) { function_call; }

    if_press(GLFW_KEY_ESCAPE, glfwSetWindowShouldClose(window, true));
    if_press(GLFW_KEY_W, camera.processKeyboard(FORWARD, deltaTime));
    if_press(GLFW_KEY_S, camera.processKeyboard(BACKWARD, deltaTime));
    if_press(GLFW_KEY_A, camera.processKeyboard(LEFT, deltaTime));
    if_press(GLFW_KEY_D, camera.processKeyboard(RIGHT, deltaTime));
    if_press(GLFW_KEY_Q, camera.processMouseMovement(deltaTime * -1000, 0));
    if_press(GLFW_KEY_E, camera.processMouseMovement(deltaTime *  1000, 0));
    if_press(GLFW_KEY_C, camera.processMouseMovement(0, deltaTime * -1000));
    if_press(GLFW_KEY_Z, camera.processMouseMovement(0, deltaTime *  1000));
    if_press(GLFW_KEY_J, )

#undef if_press
}

void Application::processArgs(int argc, char **argv) {
    // TODO: process programm arguments if there is any
}

