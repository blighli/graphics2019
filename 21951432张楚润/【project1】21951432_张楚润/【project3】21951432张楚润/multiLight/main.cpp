//main.cpp
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL/SOIL.h>

#include <iostream>

#include "Shader.h"

#pragma comment(lib, "./SOIL.lib")

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32mxs.lib")


void key_callback(GLFWwindow* pWnd, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* pWnd, double xpos, double ypos);
void scroll_callback(GLFWwindow* pWnd, double xoffset, double yoffset);
void do_movement();

const GLuint WIDTH = 800, HEIGHT = 600;

Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

/////////////////////////////////////////////////////////////////////////
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* pWnd = glfwCreateWindow(WIDTH, HEIGHT, "MultipleLight", nullptr, nullptr);
    glfwMakeContextCurrent(pWnd);

    glfwSetKeyCallback(pWnd, key_callback);
    glfwSetCursorPosCallback(pWnd, mouse_callback);
    glfwSetScrollCallback(pWnd, scroll_callback);

    // 鼠标指针隐藏
    //glfwSetInputMode(pWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;

    glewInit();
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Shader 处理 箱子  以及  灯光
    Shader lightingObjShader("./obj_vertex.vs", "./obj_fragement.fs");
    Shader lampShader("./lamp_vertex.vs", "./lamp_fragement.fs");

    // 点数据
    GLfloat vertices[] = {  // 位置3  向量3  纹理2
        // 背面
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,

        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        //  前面
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,

        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        // 左面
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        // 右面
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        // 下面
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        // 上面
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // 箱子位置
    glm::vec3 objPos[] = {
        glm::vec3(-3.0f,  -1.0f,  -5.0f),       glm::vec3(2.0f,  5.0f, -1.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),         glm::vec3(0.0f, -2.0f, -5.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),          glm::vec3(-1.7f,  0.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),          glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),          glm::vec3(-1.3f,  1.0f, -1.5f) };

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    GLuint objVAO, VBO;

    glGenVertexArrays(1, &objVAO);

    // 光照物体
    glBindVertexArray(objVAO); // --- Begin
    {
        // 顶点数据复至GPU中
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 位置索引 传至Shader的
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            // 向量索引
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            // 纹理索引
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0); // --- End

     // 灯泡 (只需要用到一部分坐标点数据,用于绘制一个立方体即可)
    GLuint lampVAO, lampVBO;
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);
    {
        //glGenBuffers(1, &lampVBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    // 纹理1 木箱纹理贴图 --
    //  1. 加载图片 木箱图片
    int nTextureW = 0, nTextureH = 0;
    unsigned char* pChImg = nullptr;
    pChImg = SOIL_load_image("texture.png", &nTextureW, &nTextureH, 0, SOIL_LOAD_RGB);

    // 2. 纹理设置
    GLuint diffuseMap;
    glGenTextures(1, &diffuseMap);
    {
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nTextureW, nTextureH, 0, GL_RGB, GL_UNSIGNED_BYTE, pChImg);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    SOIL_free_image_data(pChImg);   // 释放内存

    // 纹理2 木箱边框反射贴图 --
    //  1. 加载图片 木箱边框图片
    pChImg = SOIL_load_image("texture_specular.png", &nTextureW, &nTextureH, 0, SOIL_LOAD_RGB);

    // 2. 纹理设置
    GLuint specularMap;
    glGenTextures(1, &specularMap);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nTextureW, nTextureH, 0, GL_RGB, GL_UNSIGNED_BYTE, pChImg);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(pChImg);

    // 设置Shader中的材质属性
    lightingObjShader.useShaderPrograme();
    glUniform1i(glGetUniformLocation(lightingObjShader.getPrograme(), "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingObjShader.getPrograme(), "material.specular"), 1);

    GLuint  nCurrentTime = glfwGetTime();
    GLuint nLastTime = glfwGetTime();
    GLuint nFPS = 0;
    glm::mat4 view;
    glm::mat4 model;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // 只绘边框
    while (!glfwWindowShouldClose(pWnd))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        do_movement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingObjShader.useShaderPrograme();


        GLint viewPosLoc = glGetUniformLocation(lightingObjShader.getPrograme(), "viewPos");
        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

        // 设置Shader中的材质属性
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "material.shininess"), 4.0f);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "dirLight.specular"), 0.5f, 0.5f, 0.5f);
        // Point light 1
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[0].linear"), 0.09);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[0].quadratic"), 0.032);
        // Point light 2
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[1].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[1].linear"), 0.09);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[1].quadratic"), 0.032);
        // Point light 3
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[2].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[2].linear"), 0.09);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[2].quadratic"), 0.032);
        // Point light 4
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[3].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[3].linear"), 0.09);
        glUniform1f(glGetUniformLocation(lightingObjShader.getPrograme(), "lights[3].quadratic"), 0.032);


        GLint modelLoc = glGetUniformLocation(lightingObjShader.getPrograme(), "model");
        GLint viewLoc = glGetUniformLocation(lightingObjShader.getPrograme(), "view");
        GLint projLoc = glGetUniformLocation(lightingObjShader.getPrograme(), "projection");

        view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // 绑定漫反射贴图
        glActiveTexture(GL_TEXTURE0);   // 箱子纹理
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);   // 箱子边框的镜面反射纹理
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // 绘制箱子 -------------------
        glBindVertexArray(objVAO); // 绑定箱子的顶点数据进行操作
        for (GLuint i = 0; i < 10; i++)
        {
            model = glm::mat4();

            model = glm::translate(model, objPos[i]);
            GLfloat angle = glfwGetTime();
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        for (GLuint i = 0; i < 10; i++)
        {
            model = glm::mat4();

            model = glm::translate(model, objPos[i] + objPos[i]);
            GLfloat angle = glfwGetTime();
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        for (GLuint i = 0; i < 10; i++)
        {
            model = glm::mat4();

            model = glm::translate(model, objPos[i] + objPos[i] + objPos[i]);
            GLfloat angle = glfwGetTime();
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);       // 解绑


        // 绘制灯泡 -------------------
        lampShader.useShaderPrograme();
        modelLoc = glGetUniformLocation(lampShader.getPrograme(), "model");
        viewLoc = glGetUniformLocation(lampShader.getPrograme(), "view");
        projLoc = glGetUniformLocation(lampShader.getPrograme(), "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        //model = glm::mat4();
        //model = glm::translate(model, lightPos);
        //model = glm::scale(model, glm::vec3(0.2f)); // 缩小
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(lampVAO); // 绑定灯泡的顶点数据进行操作
        {
            for (GLuint i = 0; i<4; i++)
            {
                model = glm::mat4();
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.2f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        glBindVertexArray(0);       // 解绑

        glfwSwapBuffers(pWnd);

        // -----------------------------------------------
        nLastTime = glfwGetTime();
        nFPS++;
        if (nLastTime - nCurrentTime > 1)
        {
            std::cout << "当前帧率:" << nFPS << std::endl;
            nFPS = 0;
            nCurrentTime = nLastTime;
        }
    }

    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* pWnd, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWnd, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement()
{
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* pWnd, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* pWnd, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
