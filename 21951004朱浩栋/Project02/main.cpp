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
#define _USE_MATH_DEFINES

using namespace std;

Vertex rect01[10000];
Vertex rect02[10000];
Vertex rect2[]={
        {20,0,0,0,1,0,1,0,0},
        {-20,0,0,0,1,0,1,0,0},
        {0,0,-20,0,1,0,1,0,0},
};
int SphereNumb,SphereNumb2;
Shader _shader;
Camera _camera;
float _width=640;
float _height=480;
float Zx=0;

int modelSphere(Vertex* v, float x, float y, float z, float r, int n){
    float dn = 2*M_PI/n;
    float zt,phi;
    int count=0;
    if (n % 2 == 1) return 0;
    for(int i=0; i<n/2; i++){
        phi = 2*dn*i;
        v[count].x=x;v[count].y=y+r;v[count].z=z;
        v[count].nx=0;v[count].ny=1;v[count].nz=0;
        v[count].r=1-2.0*i/n; v[count].g=0; v[count].b=2.0*i/n; count++;
        for(int j=0; j<n/2-1; j++){
            zt = dn*(j+1);
            v[count].x=x+r*sin(zt)*sin(phi); v[count].y=y+r*cos(zt); v[count].z=z+ r*sin(zt)*cos(phi);
            v[count].nx=sin(zt)*sin(phi); v[count].ny=cos(zt); v[count].nz=sin(zt)*cos(phi);
            v[count].r=1-2.0*i/n; v[count].g=0; v[count].b=2.0*i/n; count++;
            v[count].x=x+r*sin(zt)*sin(phi+dn); v[count].y=y+r*cos(zt); v[count].z=z+ r*sin(zt)*cos(phi+dn);
            v[count].nx=sin(zt)*sin(phi+dn); v[count].ny=cos(zt); v[count].nz=sin(zt)*cos(phi+dn);
            v[count].r=1-2.0*i/n; v[count].g=0; v[count].b=2.0*i/n; count++;
        }
        v[count].x=x;v[count].y=y-r;v[count].z=z;
        v[count].nx=0;v[count].ny=-1;v[count].nz=0;
        v[count].r=1-2.0*i/n; v[count].g=0; v[count].b=2.0*i/n; count++;
        for(int j=n/2-2; j>=0; j--){
            zt = dn*(j+1);
            v[count].x=x+r*sin(zt)*sin(phi+2*dn); v[count].y=y+r*cos(zt); v[count].z=z+ r*sin(zt)*cos(phi+2*dn);
            v[count].nx=sin(zt)*sin(phi+2*dn); v[count].ny=cos(zt); v[count].nz=sin(zt)*cos(phi+2*dn);
            v[count].r=1-2.0*i/n; v[count].g=0; v[count].b=2.0*i/n; count++;
            v[count].x=x+r*sin(zt)*sin(phi+dn); v[count].y=y+r*cos(zt); v[count].z=z+ r*sin(zt)*cos(phi+dn);
            v[count].nx=sin(zt)*sin(phi+dn); v[count].ny=cos(zt); v[count].nz=sin(zt)*cos(phi+dn);
            v[count].r=1-2.0*i/n; v[count].g=0; v[count].b=2.0*i/n; count++;
        }
    }
    return count-1;
}

void glRender(GLFWwindow* window){


    _shader.begin();
    glUniformMatrix4fv(_shader._proj, 1, GL_FALSE, ((matrix4)(_camera._matProj)).data());//!将摄影机信息输入
    glUniformMatrix4fv(_shader._view, 1, GL_FALSE, ((matrix4)(_camera._matView)).data());
    glUniform3f(_shader._camera,_camera._eye.x,_camera._eye.y,_camera._eye.z);
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
    //
    glVertexAttribPointer(_shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), rect01);//!输入顶点信息
    glVertexAttribPointer(_shader._normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(rect01[0].nx));//!输入法向信息
    glVertexAttribPointer(_shader._color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(rect01[0].r));//!输入颜色信息

    for(int i=0;i<10;i++){
        glUniform1i(_shader._isTrans[i],0);
    }

    glUniform1f(_shader._metallic,0.5);
    glUniform1f(_shader._roughness,0.5);

    matrix4 trans;trans[0].x = 60; trans[0].y = 0; trans[0].z = 0;
    glUniform1i(_shader._isTrans[0],2);
    glUniformMatrix4fv(_shader._trans[0], 1, GL_FALSE, trans.data());

    matrix4 rot = CELL::createRotate(tvec3<float>(0,0,0),tvec3<float>(0,1,0),Zx);
    Zx+=1;
    glUniform1i(_shader._isTrans[1],1);
    glUniformMatrix4fv(_shader._trans[1], 1, GL_FALSE, rot.data());

    rot = CELL::createRotate(tvec3<float>(60*cos(PI/180*Zx),0,-60*sin(PI/180*Zx)),tvec3<float>(0,1,0),2*Zx);
    glUniform1i(_shader._isTrans[2],1);
    glUniformMatrix4fv(_shader._trans[2], 1, GL_FALSE, rot.data());

    trans[0].x = 0; trans[0].y = 0; trans[0].z = -10;
    glUniform1i(_shader._isTrans[3],2);
    glUniformMatrix4fv(_shader._trans[3], 1, GL_FALSE, trans.data());


    glDrawArrays(GL_TRIANGLE_STRIP, 0, SphereNumb);

    //
    glVertexAttribPointer(_shader._position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), rect02);//!输入顶点信息
    glVertexAttribPointer(_shader._normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(rect02[0].nx));//!输入法向信息
    glVertexAttribPointer(_shader._color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &(rect02[0].r));//!输入颜色信息

    for(int i=0;i<10;i++){
        glUniform1i(_shader._isTrans[i],0);
    }

    trans[0].x = 60; trans[0].y = 0; trans[0].z = 0;
    glUniform1i(_shader._isTrans[0],2);
    glUniformMatrix4fv(_shader._trans[0], 1, GL_FALSE, trans.data());

    rot = CELL::createRotate(tvec3<float>(0,0,0),tvec3<float>(0,1,0),Zx);
    glUniform1i(_shader._isTrans[1],1);
    glUniformMatrix4fv(_shader._trans[1], 1, GL_FALSE, rot.data());

    trans[0].x = 20; trans[0].y = 0; trans[0].z = 0;
    glUniform1i(_shader._isTrans[2],2);
    glUniformMatrix4fv(_shader._trans[2], 1, GL_FALSE, trans.data());

    rot = CELL::createRotate(tvec3<float>(60*cos(PI/180*Zx),0,-60*sin(PI/180*Zx)),tvec3<float>(0,1,0),2*Zx);
    glUniform1i(_shader._isTrans[3],1);
    glUniformMatrix4fv(_shader._trans[3], 1, GL_FALSE, rot.data());

    trans[0].x = -0; trans[0].y = 0; trans[0].z = -10;
    glUniform1i(_shader._isTrans[4],2);
    glUniformMatrix4fv(_shader._trans[4], 1, GL_FALSE, trans.data());


    glDrawArrays(GL_TRIANGLE_STRIP, 0, SphereNumb2);

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

    _shader.end();



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
    SphereNumb = modelSphere(rect01,0,0,0,10,100);
    SphereNumb2 = modelSphere(rect02,0,0,0,5,100);

    while (!glfwWindowShouldClose(Window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0,0,_width,_height);
        glRender(Window);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
