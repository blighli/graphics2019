//glad glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

//common
#include <common/camera.hpp>
#include <common/shader.hpp>
#include <common/skinnedMesh.hpp>

//standard lib
#include <iostream>

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void processInput(GLFWwindow *window);

// screen
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = (float) SCR_WIDTH / 2.0;
float lastY = (float) SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // Initialise GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OS

    GLFWwindow *window = glfwCreateWindow(1024, 768, "Project 03", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window.\n" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD.\n" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader bobShader("shaders/bone.vs", "shaders/bone.fs");
    Shader nanosuitShader("shaders/nanosuit.vs", "shaders/nanosuit.fs");

    //SkinnedMesh bob;
    //bob.LoadMesh("models/bob/boblampclean.md5mesh");
    SkinnedMesh nanosuit;
    nanosuit.LoadMesh("models/nanosuit/nanosuit.obj");

    while (!glfwWindowShouldClose(window)) {
        //time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        //clear
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        //bob render
        //bobShader.use();
        //mat4 projection = perspective(camera.Zoom, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
        //mat4 view = camera.GetViewMatrix();
        //mat4 model = mat4(1.0f);
        //model = translate(model, vec3(0, -2.0, 0));
        //model = rotate(model, glm::radians(-90.0f), vec3(1, 0, 0));
        //model = scale(model, vec3(0.05f));
        //mat4 MVP = projection * view * model;
        //bobShader.setMat4("MVP", MVP);
//
        //std::vector<mat4> transforms;
        //bob.BoneTransform(currentFrame, transforms);
        //for (unsigned int i = 0; i < transforms.size(); i++) {
        //    bobShader.setMat4("gBones[" + std::to_string(i) + "]", transforms[i]);
        //}
        //bobShader.setInt("diffuseTexture", 0);
        //bob.Render();

        //nanosuit render
        nanosuitShader.use();
        mat4 projection = perspective(camera.Zoom, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
        mat4 view = camera.GetViewMatrix();
        mat4 model = mat4(1.0f);
        model = translate(model, vec3(0, -6.0, 0));
        model = scale(model, vec3(0.5f));
        mat4 MVP = projection * view * model;
        nanosuitShader.setMat4("MVP", MVP);
        nanosuitShader.setMat4("M", model);
        nanosuitShader.setVec3("lightPos", vec3(3 * cos(currentFrame), 3, 3 * sin(currentFrame)));
        nanosuitShader.setVec3("viewPos", camera.Position);
        //spot light
        nanosuitShader.setFloat("sl.cutoff", glm::cos(glm::radians(8.5f)));
        nanosuitShader.setFloat("sl.outcutoff", glm::cos(glm::radians(12.5f)));
        nanosuitShader.setVec3("sl.direction", camera.Front);
        nanosuitShader.setVec3("sl.position", camera.Position);

        nanosuitShader.setInt("diffuseTexture", 0);
        nanosuitShader.setInt("specularTexture", 1);
        nanosuit.Render();


        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}