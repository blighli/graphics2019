//glad glfw
#include <glad/glad.h>
#include <glfw/glfw3.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

//common
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/camera.hpp>
#include <common/text2D.hpp>

//standard lib
#include <iostream>
#include <vector>

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

void renderSphere();

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

    GLFWwindow *window = glfwCreateWindow(1024, 768, "Project 02", nullptr, nullptr);
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

    Shader sunShader("shaders/sun.vs", "shaders/sun.fs");
    Shader earthShader("shaders/earth.vs", "shaders/earth.fs");
    Shader moonShader("shaders/moon.vs", "shaders/moon.fs");
    initText2D("models/Holstein.DDS");

    GLuint sunMap = loadTexture("models/sunmap.tga");
    GLuint earthMap = loadTexture("models/earthmap.tga");
    GLuint moonMap = loadTexture("models/moonmap.tga");

    //fps
    std::string text;
    int num_frames = 0;
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        //time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        //fps->spf
        num_frames++;
        if (currentFrame - lastTime >= 0.5) {
            float spf = 500.0 / num_frames;
            text = std::to_string(spf).substr(0, 5) + " ms/f";
            num_frames = 0;
            lastTime += 0.5;
        }
        //clear
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        vec3 sunPos = vec3(0, 0, 0);
        //render sun
        sunShader.use();
        mat4 projection = perspective(camera.Zoom, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
        mat4 view = camera.GetViewMatrix();
        mat4 model = mat4(1.0f);
        model = rotate(model, (float) glfwGetTime(), vec3(0, 1, 0));
        model = scale(model, vec3(2.0f));
        mat4 MVP = projection * view * model;
        sunShader.setMat4("MVP", MVP);
        sunShader.setInt("diffuseTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sunMap);
        renderSphere();
        //render earth
        earthShader.use();
        float earth_r = 4.0;
        float earth_x = earth_r * cos(glfwGetTime() * 2);
        float earth_y = 0.0;
        float earth_z = earth_r * sin(glfwGetTime() * 2);
        model = mat4(1.0f);
        model = translate(model, vec3(earth_x, earth_y, earth_z));
        model = rotate(model, (float) glfwGetTime(), vec3(0, 1, 0));
        MVP = projection * view * model;
        earthShader.setMat4("MVP", MVP);
        earthShader.setMat4("M", model);
        earthShader.setVec3("lightDir", sunPos-vec3(earth_x,earth_y,earth_z));
        earthShader.setInt("diffuseTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthMap);
        renderSphere();
        //render moon
        moonShader.use();
        float moon_r = 1.5;
        float moon_x = earth_x + moon_r * cos(glfwGetTime() * 5);
        float moon_y = earth_y;
        float moon_z = earth_z + moon_r * sin(glfwGetTime() * 5);
        model = mat4(1.0f);
        model = translate(model, vec3(moon_x,moon_y,moon_z));
        model = rotate(model, (float) glfwGetTime(), vec3(0, 1, 0));
        model = scale(model, vec3(0.3f));
        MVP = projection * view * model;
        moonShader.setMat4("MVP", MVP);
        moonShader.setMat4("M", model);
        moonShader.setVec3("lightDir", sunPos-vec3(moon_x,moon_y,moon_z));
        moonShader.setInt("diffuseTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, moonMap);
        renderSphere();

        //text
        printText2D(text.c_str(), 10, 550, 20);
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

unsigned int sphereVao = 0;
unsigned int indexCount;

void renderSphere() {
    if (sphereVao == 0) {
        std::vector<glm::vec3> position;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normal;
        std::vector<unsigned int> indices;
        const int X_SEGMENTS = 64;
        const int Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        for (int y = 0; y <= Y_SEGMENTS; ++y) {
            for (int x = 0; x <= X_SEGMENTS; ++x) {
                float xSeg = float(x) / float(X_SEGMENTS);
                float ySeg = float(y) / float(Y_SEGMENTS);
                float xPos = std::sin(ySeg * PI) * std::cos(xSeg * 2 * PI);
                float yPos = std::cos(ySeg * PI);
                float zPos = std::sin(ySeg * PI) * std::sin(xSeg * 2 * PI);
                position.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSeg, ySeg));
                normal.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }
        //indices
        bool oddRow = false;
        for (int y = 0; y < Y_SEGMENTS; ++y) {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (int x = 0; x <= X_SEGMENTS; ++x) {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            } else {
                for (int x = X_SEGMENTS; x >= 0; --x) {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = indices.size();
        //data
        std::vector<float> data;
        for (unsigned int i = 0; i < position.size(); i++) {
            data.push_back(position[i].x);
            data.push_back(position[i].y);
            data.push_back(position[i].z);
            if (!uv.empty()) {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (!normal.empty()) {
                data.push_back(normal[i].x);
                data.push_back(normal[i].y);
                data.push_back(normal[i].z);
            }
        }
        glGenVertexArrays(1, &sphereVao);
        glBindVertexArray(sphereVao);
        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    }
    glBindVertexArray(sphereVao);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, nullptr);
}
