//#include <GL/glut.h>
//#include <GL/glad.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include <Shader.hpp>
#include <Camera.hpp>
#include "CELLMath.hpp"
#include <box2d/box2d.h>
#include <time.h>

#include <vector>

#define _USE_MATH_DEFINES

using namespace std;

Vertex rect[10000];


//Vertex rect2[]={
//        {20,0,0,0,1,0,1,0,0},
//        {-20,0,0,0,1,0,1,0,0},
//        {0,0,-20,0,1,0,1,0,0},
//};
int SphereNumb;
Shader _shader;
Camera _camera;
Object obj;
int WallsNumb;
Object Walls[100];
Object P[30];
Object LR[2];
int TGNumb;
Object TG[100];
float PSpeed[30];
bool space_pressed = 0;
bool lr_pressed[2];
int test = 0;

float _width=1600;
float _height=900;
float Zx=0;

b2Body* b2Ball;

int modelSphere(Vertex* v, float x, float y, float z, float r, int n, float rr, float gg, float bb){
    float dn = 2*M_PI/n;
    float zt,phi;
    int count=0;
    if (n % 2 == 1) return 0;
    for(int i=0; i<n/2; i++){
        phi = 2*dn*i;
        v[count].x=x;v[count].y=y+r;v[count].z=z;
        v[count].nx=0;v[count].ny=1;v[count].nz=0;count++;
        for(int j=0; j<n/2-1; j++){
            zt = dn*(j+1);
            v[count].x=x+r*sin(zt)*sin(phi); v[count].y=y+r*cos(zt); v[count].z=z+ r*sin(zt)*cos(phi);
            v[count].nx=sin(zt)*sin(phi); v[count].ny=cos(zt); v[count].nz=sin(zt)*cos(phi); count++;
            v[count].x=x+r*sin(zt)*sin(phi+dn); v[count].y=y+r*cos(zt); v[count].z=z+ r*sin(zt)*cos(phi+dn);
            v[count].nx=sin(zt)*sin(phi+dn); v[count].ny=cos(zt); v[count].nz=sin(zt)*cos(phi+dn); count++;
        }
        v[count].x=x;v[count].y=y-r;v[count].z=z;
        v[count].nx=0;v[count].ny=-1;v[count].nz=0;count++;
        for(int j=n/2-2; j>=0; j--){
            zt = dn*(j+1);
            v[count].x=x+r*sin(zt)*sin(phi+2*dn); v[count].y=y+r*cos(zt); v[count].z=z+ r*sin(zt)*cos(phi+2*dn);
            v[count].nx=sin(zt)*sin(phi+2*dn); v[count].ny=cos(zt); v[count].nz=sin(zt)*cos(phi+2*dn); count++;
            v[count].x=x+r*sin(zt)*sin(phi+dn); v[count].y=y+r*cos(zt); v[count].z=z+ r*sin(zt)*cos(phi+dn);
            v[count].nx=sin(zt)*sin(phi+dn); v[count].ny=cos(zt); v[count].nz=sin(zt)*cos(phi+dn); count++;
        }
    }
    for(int i=0; i<count-1; i++){
        v[i].r=rr; v[i].g=gg; v[i].b=bb;
    }
    return count-1;
}
void modelLine(Vertex* v, float x1, float z1, float x2, float z2, float h, float rr, float gg, float bb){
    v[0].x = x1; v[0].y = h; v[0].z = z1;
    v[1].x = x1; v[1].y = 0; v[1].z = z1;
    v[2].x = x2; v[2].y = h; v[2].z = z2;
    v[3].x = x2; v[3].y = 0; v[3].z = z2;
    float tmp = sqrt((x1-x2)*(x1-x2)+(z1-z2)*(z1-z2) );
    for(int i=0; i<4; i++){
        v[i].r=rr; v[i].g=gg; v[i].b=bb;
        v[i].nx = -(z2-z1)/tmp; v[i].ny = 0; v[i].nz = -(x2-x1)/tmp;
    }
    return ;
}
void modelTG(Vertex* v, float x1, float z1, float x2, float z2,  float x3, float z3, float h, float rr, float gg, float bb){
    v[0].x = x3; v[0].y = h; v[0].z = z3;
    v[1].x = x2; v[1].y = h; v[1].z = z2;
    v[2].x = x1; v[2].y = h; v[2].z = z1;
    v[3].x = x2; v[3].y = 0; v[3].z = z2;
    v[4].x = x3; v[4].y = h; v[4].z = z3;
    v[5].x = x3; v[5].y = 0; v[5].z = z3;
    v[6].x = x1; v[6].y = h; v[6].z = z1;
    v[7].x = x1; v[7].y = 0; v[7].z = z1;
    v[8].x = x2; v[8].y = 0; v[8].z = z2;
    CELL::tvec3<float> t1 = CELL::normalize(CELL::tvec3<float>(x1-x2,0,z1-z2)), t2 = CELL::normalize(CELL::tvec3<float>(x2-x3,0,z2-z3)), t3 = CELL::normalize(CELL::tvec3<float>(x3-x1,0,z3-z1));
    CELL::tvec3<float> n1 = CELL::normalize(t1-t3+CELL::tvec3<float>(0,1,0)), n2 = CELL::normalize(t1-t3+CELL::tvec3<float>(0,-1,0)),
    n3 = CELL::normalize(t2-t1+CELL::tvec3<float>(0,1,0)), n4 = CELL::normalize(t2-t1+CELL::tvec3<float>(0,-1,0)),
    n5 = CELL::normalize(t3-t2+CELL::tvec3<float>(0,1,0)), n6 = CELL::normalize(t3-t2+CELL::tvec3<float>(0,-1,0));
    v[6].nx = n1.x; v[6].ny = n1.y; v[6].nz = n1.z;
    v[2].nx = n1.x; v[2].ny = n1.y; v[2].nz = n1.z;
    v[7].nx = n2.x; v[7].ny = n2.y; v[7].nz = n2.z;
    v[1].nx = n3.x; v[1].ny = n3.y; v[1].nz = n3.z;
    v[3].nx = n4.x; v[3].ny = n4.y; v[2].nz = n4.z;
    v[8].nx = n4.x; v[8].ny = n4.y; v[8].nz = n4.z;
    v[0].nx = n5.x; v[0].ny = n5.y; v[0].nz = n5.z;
    v[4].nx = n5.x; v[4].ny = n5.y; v[4].nz = n5.z;
    v[5].nx = n6.x; v[5].ny = n6.y; v[5].nz = n6.z;
    for(int i=0; i<9; i++){
        v[i].r=rr; v[i].g=gg; v[i].b=bb;

    }
    return ;
}

void glRender(GLFWwindow* window){



    glUniformMatrix4fv(_shader._proj, 1, GL_FALSE, ((matrix4)(_camera._matProj)).data());//!将摄影机信息输入
    glUniformMatrix4fv(_shader._view, 1, GL_FALSE, ((matrix4)(_camera._matView)).data());
    glUniform3f(_shader._camera,_camera._eye.x,_camera._eye.y,_camera._eye.z);

    for(int i=0;i<20;i++){
        switch(i){
            case 0:
                glUniform1i(_shader._isLight[i],1);
                glUniform3f(_shader._lightPos[i],-100,300,90);
                glUniform3f(_shader._lightColor[i],30000,30000,30000);
                break;
            case 1:
                glUniform1i(_shader._isLight[i],1);
                glUniform3f(_shader._lightPos[i],-100,300,-310);
                glUniform3f(_shader._lightColor[i],30000,30000,30000);
                break;
            case 2:
                glUniform1i(_shader._isLight[i],1);
                glUniform3f(_shader._lightPos[i],100,300,90);
                glUniform3f(_shader._lightColor[i],30000,30000,30000);
                break;
            case 3:
                glUniform1i(_shader._isLight[i],1);
                glUniform3f(_shader._lightPos[i],100,300,-310);
                glUniform3f(_shader._lightColor[i],30000,30000,30000);
                break;
            case 4:
                glUniform1i(_shader._isLight[i],1);
                glUniform3f(_shader._lightPos[i],obj.x,10,obj.z);
                glUniform3f(_shader._lightColor[i],3000,0,0);
                break;
            default:
                glUniform1i(_shader._isTrans[i],0);
        }
    }



    obj.Draw(_shader);
    for(int i = 0; i<WallsNumb; ++i){
        Walls[i].Draw(_shader);
    }
    for(int i = 0; i<TGNumb; ++i){
        TG[i].Draw(_shader);
    }
    for(int i = 0; i<30; ++i){
        P[i].Draw(_shader);
    }
    LR[0].Draw(_shader);
    LR[1].Draw(_shader);


}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_SPACE:
            space_pressed = 1;
            break;
        case GLFW_KEY_A:
            lr_pressed[0] = 1;
            break;
        case GLFW_KEY_LEFT:
            lr_pressed[0] = 1;
            break;
        case GLFW_KEY_D:
            lr_pressed[1] = 1;
            break;
        case GLFW_KEY_RIGHT:
            lr_pressed[1] = 1;
            break;
        default:
            break;
    }
}

int main(){
//    b2AABB worldAABB;
//    worldAABB.lowerBound.Set(-150,-150);
//    worldAABB.upperBound.Set(150,250);
    bool NewGame = 1;
    int lrTimer[2] = {-1,-1};
    srand((unsigned)time(NULL));

    //region TGPos
    CELL::tvec2<float> TGPos[100];
    int TGPosNumb = 15;
    TGPos[0] = CELL::tvec2<float>(-50,-160);TGPos[1] = CELL::tvec2<float>(-10,-160);TGPos[2] = CELL::tvec2<float>(30,-160);
    TGPos[3] = CELL::tvec2<float>(-30,-140);TGPos[4] = CELL::tvec2<float>(10,-140);

    TGPos[5] = CELL::tvec2<float>(-80,-70);TGPos[6] = CELL::tvec2<float>(-80,-30);TGPos[7] = CELL::tvec2<float>(-80,10);
    TGPos[8] = CELL::tvec2<float>(-60,-50);TGPos[9] = CELL::tvec2<float>(-60,-10);

    TGPos[10] = CELL::tvec2<float>(60,-70);TGPos[11] = CELL::tvec2<float>(60,-30);TGPos[12] = CELL::tvec2<float>(60,10);
    TGPos[13] = CELL::tvec2<float>(40,-50);TGPos[14] = CELL::tvec2<float>(40,-10);


    //endregion

    //region b2init
    b2Vec2 gravity(0.0f,-60.0f);
    b2World MyWorld(gravity);
    MyWorld.SetAllowSleeping(true);
    MyWorld.SetContinuousPhysics(true);
    //endregion
    //region b2wall
    b2BodyDef b2WallDef[10];
    b2WallDef[0].position.Set(-7.0f,-10.5f);
    b2WallDef[0].angle = -0.46364760900081;
    b2WallDef[1].position.Set(5.0f,-10.5f);
    b2WallDef[1].angle = -2.677945044589;
    b2WallDef[2].position.Set(-10.0f,5.0f);
    b2WallDef[3].position.Set(10.0f,5.0f);
    b2WallDef[4].position.Set(8.6f,4.0f);
    b2WallDef[5].position.Set(0.0f,19.0f);
    b2WallDef[6].position.Set(9.0f,-9.0f);
    b2WallDef[7].position.Set(-6.0f,-11.9f);
    b2WallDef[8].position.Set(4.0f,-11.9f);
    b2WallDef[9].position.Set(0.0f,-13.767766952966f);
    b2Body* b2Wall[10];
    for(int i=0;i<10;++i) b2Wall[i] = MyWorld.CreateBody(&b2WallDef[i]);
    b2PolygonShape b2WallShape[10];
    b2WallShape[0].SetAsBox(3.3541019662497f,0.01f);
    b2WallShape[1].SetAsBox(3.3541019662497f,0.01f);
    b2WallShape[2].SetAsBox(0.01f,14.0f);
    b2WallShape[3].SetAsBox(0.01f,14.0f);
    b2WallShape[4].SetAsBox(0.01f,13.0f);
    b2WallShape[5].SetAsBox(10.0f,0.01f);
    b2WallShape[6].SetAsBox(1.0f,0.01f);
    b2WallShape[7].SetAsBox(1.0f,0.01f);
    b2WallShape[8].SetAsBox(1.0f,0.01f);
    b2WallShape[9].SetAsBox(10.0f,0.01f);
    b2FixtureDef b2WallFixDef[10];
    for(int i=0;i<10;++i){
        b2WallFixDef[i].shape = &b2WallShape[i];
        b2WallFixDef[i].density = 0.0f;
        b2WallFixDef[i].friction = 0.1f;
        b2WallFixDef[i].restitution = 0.5f;
        if(i==7||i==8) b2WallFixDef[i].restitution = 0.0f;
        if(i==9) b2WallFixDef[i].friction = 0.02f;
    }
    b2Fixture* b2WallFix[10];
    for(int i=0;i<10;++i)  b2WallFix[i] = b2Wall[i]->CreateFixture(&b2WallFixDef[i]);
    //endregion
    //region b2TG
    b2BodyDef b2TGDef[TGPosNumb*3];
    for(int i=0;i<TGPosNumb;++i){
        b2TGDef[i*3].position.Set(TGPos[i].x/10-0.25,-TGPos[i].y/10+0.433);
        b2TGDef[i*3].angle = 2.0943951023932f;
        b2TGDef[i*3+1].position.Set(TGPos[i].x/10+0.25,-TGPos[i].y/10+0.433);
        b2TGDef[i*3+1].angle = 1.0471975511966f;
        b2TGDef[i*3+2].position.Set(TGPos[i].x/10,-TGPos[i].y/10+0.866);
    }
    b2Body* b2TG[TGPosNumb*3];
    for(int i=0;i<TGPosNumb*3;++i) {
        b2TG[i] = MyWorld.CreateBody(&b2TGDef[i]);
    }
    b2PolygonShape b2TGShape[TGPosNumb*3];
    for(int i=0;i<TGPosNumb*3;++i) b2TGShape[i].SetAsBox(1.0f,0.01f);
    b2FixtureDef b2TGFixDef[TGPosNumb*3];
    for(int i=0;i<TGPosNumb*3;++i){
        b2TGFixDef[i].shape = &b2TGShape[i];
        b2TGFixDef[i].density = 0.0f;
        b2TGFixDef[i].friction = 0.01f;
        b2TGFixDef[i].restitution = 1.0f;
    }
    b2Fixture* b2TGFix[TGPosNumb*3];
    for(int i=0;i<TGPosNumb*3;++i)  b2TGFix[i] = b2TG[i]->CreateFixture(&b2TGFixDef[i]);



    //endregion
    //region b2lr
    b2BodyDef b2LRDef[2];
    b2LRDef[0].position.Set(-4.1f,-11.9f);
    b2LRDef[1].position.Set(2.1f,-11.9f);
    b2LRDef[0].type = b2_dynamicBody;
    b2LRDef[1].type = b2_dynamicBody;
    b2Body* b2LR[2];
    for(int i=0;i<2;++i) b2LR[i] = MyWorld.CreateBody(&b2LRDef[i]);
    b2PolygonShape b2LRShape[2];
    b2LRShape[0].SetAsBox(2.6f,0.01f);
    b2LRShape[1].SetAsBox(2.6f,0.01f);
    b2FixtureDef b2LRFixDef[2];
    for(int i=0;i<2;++i){
        b2LRFixDef[i].shape = &b2LRShape[i];
        b2LRFixDef[i].density = 99.0f;
        b2LRFixDef[i].friction = 0.1f;
        b2LRFixDef[i].restitution = 0.5f;
    }
    b2Fixture* b2LRFix[2];
    for(int i=0;i<2;++i)  b2LRFix[i] = b2LR[i]->CreateFixture(&b2LRFixDef[i]);


    b2RevoluteJointDef b2LRJointDef[2];
    b2LRJointDef[0].Initialize(b2LR[0],b2Wall[0],b2Vec2(-4.0f,-12.0f));
    b2LRJointDef[1].Initialize(b2LR[1],b2Wall[1],b2Vec2(2.0f,-12.0f));
    MyWorld.CreateJoint(&b2LRJointDef[0]);MyWorld.CreateJoint(&b2LRJointDef[1]);
//endregion
    //region b2ball
    b2BodyDef b2BallDef;
    b2BallDef.type = b2_dynamicBody;
    b2BallDef.position.Set(9.6f,-6.0f);
    //b2BallDef.position.Set(7.5f,6.0f);
    b2Ball = MyWorld.CreateBody(&b2BallDef);
    b2CircleShape b2BallShape;
    b2BallShape.m_radius = 0.3f;
    b2FixtureDef b2BallFixDef;
    b2BallFixDef.shape = &b2BallShape;
    b2BallFixDef.density = 1.0f;
    b2BallFixDef.friction = 0.1f;
    b2BallFixDef.restitution = 0.5f;
    b2Fixture* b2BallFix;
    b2BallFix = b2Ball->CreateFixture(&b2BallFixDef);
    b2Ball->SetBullet(true);
    //endregion

    MyListener Listener;
    MyWorld.SetContactListener(&Listener);

    GLFWwindow* Window;
    //region GLFWInit
    if (!glfwInit()) return -1;
    Window = glfwCreateWindow(_width, _height, "Press Space To Start",  nullptr , nullptr);
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
    glfwSetKeyCallback(Window, key_callback);
    //endregion
    _camera.setEye(CELL::real3(0,300,100));
    _camera.update();
    _camera.perspective(60,_width/_height,20,1500);

    std::string vsFile  =   "../txt/vs.txt";
    std::string fsFile =    "../txt/fs.txt";
    _shader.initialize(vsFile, fsFile);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    _shader.begin();



    SphereNumb = modelSphere(rect,0,0,0,3,100,1,0,0);
    obj.setObject(rect,SphereNumb);
    SphereNumb = modelSphere(rect,0,0,0,1.5,100,0.4,0.4,1);
    for(int i = 0; i<30; ++i){
        P[i].setObject(rect,SphereNumb);
        P[i].x = -99+rand()%198;
        P[i].z = -171-rand()%18;
        PSpeed[i] = 3+rand()%2;
    }

    modelTG(rect,-40,120,-14,120,-30,125,20,0.87,0.49,0.172);
    LR[0].setObject(rect,9);
    LR[0].x0 = -40; LR[0].z0 = 120;
    modelTG(rect,20,120,-6,120,10,125,20,0.87,0.49,0.172);
    LR[1].setObject(rect,9);
    LR[1].x0 = 20; LR[1].z0 = 120;
    //region wall
    int count = 0;
    modelLine(rect,-100,90,-40,120,20,1,1,1);
    Walls[count++].setObject(rect,4);
    modelLine(rect,20,120,80,90,20,1,1,1);
    Walls[count++].setObject(rect,4);
    modelLine(rect,80,90,100,90,20,1,1,1);
    Walls[count++].setObject(rect,4);
    modelLine(rect,-100,-190,100,-190,20,1,1,1);
    Walls[count++].setObject(rect,4);
    modelLine(rect,-100,90,-100,-190,20,1,1,1);
    Walls[count++].setObject(rect,4);
    modelLine(rect,100,90,100,-190,20,1,1,1);
    Walls[count++].setObject(rect,4);
    modelLine(rect,86,90,86,-170,20,1,1,1);
    Walls[count++].setObject(rect,4);
    WallsNumb = count;
    //endregion

    count = 0;
    modelTG(rect,0,0,-5,-8.65,5,-8.65,10,1,1,0);
    for(int i=0; i<TGPosNumb; i++){
        TG[count++].setObject(rect,9); TG[count-1].x = TGPos[i].x; TG[count-1].z = TGPos[i].y;
    }
    TGNumb = count;


    while (!glfwWindowShouldClose(Window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0,0,_width,_height);
        MyWorld.Step(1.0f/60.f,100,100);
        obj.x = b2Ball->GetPosition().x*10.0;
        obj.z = -b2Ball->GetPosition().y*10.0;
        LR[0].x = b2LR[0]->GetPosition().x*10.0+40;
        LR[0].z = -b2LR[0]->GetPosition().y*10.0-120;
        LR[1].x = b2LR[1]->GetPosition().x*10.0-20;
        LR[1].z = -b2LR[1]->GetPosition().y*10.0-120;
        LR[0].deg = 180.0*(b2LR[0]->GetAngle())/PI;
        LR[1].deg = 180.0*(b2LR[1]->GetAngle())/PI;

        if(obj.z>=130.0f){
            b2Ball->SetTransform(b2Vec2(9.6f,-6.0f),0.0f);
            NewGame =1;Listener.score = 0;
        }

        if(NewGame==1){
            glfwSetWindowTitle(Window,"Press Space To Start");
        }
        else if(obj.x>=82&&obj.x<=100&&obj.z<=70) {
            glfwSetWindowTitle(Window,"Press Space");
        }
        else{
            glfwSetWindowTitle(Window,(string("Score: ")+to_string(Listener.score)).c_str());
        }
        if(obj.x>=82&&obj.x<=100&&obj.z>=80&&space_pressed){
            NewGame = 0;
            b2Ball->SetLinearVelocity(b2Vec2(0.0f,57.5f));
        }


        //region lrTimer
        if(lr_pressed[0]){
            b2LR[0]->SetAngularVelocity(100.0f);
            lrTimer[0] = 10;
        }
        if(lr_pressed[1]){
            b2LR[1]->SetAngularVelocity(-100.0f);
            lrTimer[1] = 10;
        }
        if(lrTimer[0]>=0) --lrTimer[0];
        if(lrTimer[1]>=0) --lrTimer[1];
        if(lrTimer[0]==-1) b2LR[0]->SetAngularVelocity(-4.0f);
        if(lrTimer[1]==-1) b2LR[1]->SetAngularVelocity(4.0f);
        //endregion

        if(obj.z<-170){
            b2Ball->ApplyForceToCenter(b2Vec2(-33.0f,0.0f),1);
        }

        for(int i = 0;i<30; ++i){
            P[i].x -= PSpeed[i];
            if(P[i].x<=-99){
                P[i].x = 99;
            }
        }

        glRender(Window);

        space_pressed = 0;lr_pressed[0] = 0; lr_pressed[1] = 0;
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    _shader.end();
    glfwTerminate();
    return 0;
}
