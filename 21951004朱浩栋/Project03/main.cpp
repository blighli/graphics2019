//#include <GL/glut.h>
//#include <GL/glad.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include <Shader.hpp>
#include <tchar.h>
#include <Vertex.h>
#include <Camera.hpp>
#include <cmath>
#include <assimp/camera.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define _USE_MATH_DEFINES

using namespace std;



Shader _shader;
Camera _camera;
float _width=640;
float _height=480;
float Zx=0;



void glRender(GLFWwindow* window,Model model){


    glUniformMatrix4fv(_shader._proj, 1, GL_FALSE, ((matrix4)(_camera._matProj)).data());//!将摄影机信息输入 P
    glUniformMatrix4fv(_shader._view, 1, GL_FALSE, ((matrix4)(_camera._matView)).data());//!将摄影机信息输入 V
    glUniform3f(_shader._camera,_camera._eye.x,_camera._eye.y,_camera._eye.z);//!单独输入了一个摄像机位置
    //!不管理光照
    //!输入点光源的信息
    for(int i=0;i<20;i++){
        switch(i){
            case 0:
                glUniform1i(_shader._isLight[i],1);
                glUniform3f(_shader._lightPos[i],0,0,0);
                glUniform3f(_shader._lightColor[i],20000,20000,20000);
                break;
            default:
                glUniform1i(_shader._isTrans[i],0);
        }
    }
    //!有关顶点的信息输入放在mesh.draw里面

    model.Draw(_shader);
//    glVertexAttribPointer(_shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), rect01);//!输入顶点信息
//    glVertexAttribPointer(_shader._normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(rect01[0].nx));//!输入法向信息
//    glVertexAttribPointer(_shader._color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(rect01[0].r));//!输入纹理

//    for(int i=0;i<10;i++){
//        glUniform1i(_shader._isTrans[i],0);
//    }
//
//    glUniform1f(_shader._metallic,0.5);
//    glUniform1f(_shader._roughness,0.5);
//
//    matrix4 trans;trans[0].x = 60; trans[0].y = 0; trans[0].z = 0;
//    glUniform1i(_shader._isTrans[0],2);
//    glUniformMatrix4fv(_shader._trans[0], 1, GL_FALSE, trans.data());
//
//    matrix4 rot = CELL::createRotate(tvec3<float>(0,0,0),tvec3<float>(0,1,0),Zx);
//    Zx+=1;
//    glUniform1i(_shader._isTrans[1],1);
//    glUniformMatrix4fv(_shader._trans[1], 1, GL_FALSE, rot.data());
//
//    rot = CELL::createRotate(tvec3<float>(60*cos(PI/180*Zx),0,-60*sin(PI/180*Zx)),tvec3<float>(0,1,0),2*Zx);
//    glUniform1i(_shader._isTrans[2],1);
//    glUniformMatrix4fv(_shader._trans[2], 1, GL_FALSE, rot.data());
//
//    trans[0].x = 0; trans[0].y = 0; trans[0].z = -10;
//    glUniform1i(_shader._isTrans[3],2);
//    glUniformMatrix4fv(_shader._trans[3], 1, GL_FALSE, trans.data());


//    glDrawArrays(GL_TRIANGLE_STRIP, 0, SphereNumb);



//region
//    for(int i=0;i<5;i++){
//        for(int j=0;j<5;j++){
//            matrix4 trans;trans[0].x = -50*2+50*i; trans[0].y = -50*2+50*j; trans[0].z = -150;
//            glUniform1i(_shader._isTrans[1],2);
//            glUniformMatrix4fv(_shader._trans[1], 1, GL_FALSE, trans.data());
//            glUniform1f(_shader._metallic,0.2*i);
//            glUniform1f(_shader._roughness,0.2*j);
//            glDrawArrays(GL_TRIANGLE_STRIP, 0, SphereNumb);
//        }
//    }
//endregion





}

int main(){

    GLFWwindow* Window;
    //region GLFWInit
    if (!glfwInit()) return -1;
    Window = glfwCreateWindow(640, 480, "Hello World",  nullptr , nullptr);
    if (!Window){
        glfwTerminate();return -1;
    }
    glfwMakeContextCurrent(Window);
    //endregion
    //region GLEWInit
    glewExperimental = true;
    if (glewInit()!= GLEW_OK){
        cout << "Failed to init GLEW!\n";
        return -1;
    }
    //endregion
    _camera.update();
    _camera.perspective(60,_width/_height,20,1500);

    std::string vsFile  =   "../txt/vs.txt";
    std::string fsFile =    "../txt/fs.txt";
    _shader.initialize(vsFile, fsFile);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    _shader.begin();
    Model MM("IronMan.obj",_shader);

    while (!glfwWindowShouldClose(Window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0,0,_width,_height);

        glRender(Window,MM);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
