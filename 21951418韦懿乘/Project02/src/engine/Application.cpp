//
// Created by Krisu on 2019-11-22.
//

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "engine/Application.hpp"
#include "basic/Model.hpp"
#include "objects/CookTorrancePbrObj.hpp"
#include "objects/Skybox.hpp"
#include "Application.hpp"

std::string Application::glsl_version;
GLFWwindow *Application::window = nullptr;

static void glfwErrorCallback(int error, const char *description) {
    std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

Application::Application(int argc, char **argv) {
    // TODO:
    // deal with arguments, if there is any
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
    glfwSwapInterval(1); // vsync

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



void Application::initializeScene() {
    camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f));
    skybox.prepare();
}



void Application::renderScene() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    static glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            (float)width / height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    // Sun:
    static Shader sunShader("shaders/emissive.vert", "shaders/emissive.frag");
    static Texture sunTex("resources/solar/sun.jpg");
    sunShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTex.id());
    sunShader.setValue("projection", projection);
    sunShader.setValue("view", view);
    glm::mat4 sun_model(1.f);
    sun_model = glm::translate(sun_model, glm::vec3(0.f, 0.f, 0.f));
    sun_model = glm::rotate(sun_model, float(0.05*glfwGetTime()), glm::vec3(0.f, 1.f, 0.f));
    sunShader.setValue("model", sun_model);
    Primitive::renderSphere();

    // Planets:
    static Shader planetShader("shaders/planet.vert", "shaders/planet.frag");
    planetShader.setValue("projection", projection);
    planetShader.setValue("view", view);
    // Earth:
    planetShader.use();
    static Texture earth("resources/solar/earth_daymap.jpg");
    static Texture earth_cloud("resources/solar/earth_clouds.jpg");
    static Texture earth_specular("resources/solar/earth_daymap.jpg");
    planetShader.setValue("specular_map", 1);
    planetShader.setValue("cloud_map", 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earth.id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, earth_specular.id());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, earth_cloud.id());

    planetShader.setValue("projection", projection);
    planetShader.setValue("view", view);
    planetShader.setValue("lightPos", glm::vec3(0.f));
    planetShader.setValue("viewPos", camera.Position);
    glm::mat4 earth_model(1.f);
    // global rotate
    earth_model = glm::rotate(earth_model, float(0.10*glfwGetTime()),
            glm::vec3(0.f, 1.f, 0.f));
    // R
    earth_model = glm::translate(earth_model, glm::vec3(0.f, 0.f, 4.f));
    // self rotate
    earth_model = glm::rotate(earth_model, float(0.2*glfwGetTime()),
            glm::vec3(0, 1, 0));
    earth_model = glm::scale(earth_model, glm::vec3(0.3));
    planetShader.setValue("model", earth_model);
    Primitive::renderSphere();

    // Moon
    planetShader.use();
    static Texture moon("resources/solar/moon.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, moon.id());
    glm::mat4 moon_model(1.f);
    // Global to sun:
    moon_model = glm::rotate(moon_model, float(0.10*glfwGetTime()),
            glm::vec3(0.f, 1.f, 0.f));
    moon_model = glm::translate(moon_model, glm::vec3(0.f, 0.f, 4.f));
    // Global to earth:
    moon_model = glm::rotate(moon_model, float(0.8*glfwGetTime()),
            glm::vec3(0.7f, 0.7f, 0.f));
    moon_model = glm::translate(moon_model, glm::vec3(0.f, 0.f, 0.5f));
    moon_model = glm::scale(moon_model, glm::vec3(0.07));
    planetShader.setValue("model", moon_model);
    Primitive::renderSphere();

    // Skybox
    skybox.setShaderUnif("view", view);
    skybox.setShaderUnif("projection", projection);
    skybox.render();
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

#define if_press_then(GLFW_KEY_XXX, function_call) \
    if (glfwGetKey(window, GLFW_KEY_XXX) == GLFW_PRESS) { function_call; }

    if_press_then(GLFW_KEY_ESCAPE, glfwSetWindowShouldClose(window, true));
    if_press_then(GLFW_KEY_W, camera.processKeyboard(FORWARD, deltaTime));
    if_press_then(GLFW_KEY_S, camera.processKeyboard(BACKWARD, deltaTime));
    if_press_then(GLFW_KEY_A, camera.processKeyboard(LEFT, deltaTime));
    if_press_then(GLFW_KEY_D, camera.processKeyboard(RIGHT, deltaTime));
    if_press_then(GLFW_KEY_Q, camera.processMouseMovement(deltaTime * -1000, 0));
    if_press_then(GLFW_KEY_E, camera.processMouseMovement(deltaTime *  1000, 0));
    if_press_then(GLFW_KEY_C, camera.processMouseMovement(0, deltaTime * -1000));
    if_press_then(GLFW_KEY_Z, camera.processMouseMovement(0, deltaTime *  1000));
}

