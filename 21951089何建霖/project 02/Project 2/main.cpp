#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

static int sunRotation = 0;
static int earthRevolution = 0;
static int earthRotation = 0;
static int moonRevolution = 0;
static int moonRotation = 0;
static bool animated = true;
static int speed = 1;

GLuint sunTexture;
GLuint earthTexture;
GLuint moonTexture;

// 使用stb_image.h读取图片文件生成纹理对象
unsigned int createTexture(string fileName)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture.\n");
    }
    stbi_image_free(data);
    return texture;
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    sunTexture = createTexture("/Users/hejianlin/Desktop/何建霖201951089/project 02/Project 2/texture/sun.bmp");
    earthTexture = createTexture("/Users/hejianlin/Desktop/何建霖201951089/project 02/Project 2/texture/earth.jpg");
    moonTexture = createTexture("/Users/hejianlin/Desktop/何建霖201951089/project 02/Project 2/texture/moon.bmp");
}

void reshape(int w, int h)
{
    glutReshapeWindow(750, 700);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (GLdouble)w / (GLdouble)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, -10, 0, 0, 0, 0, 1, 0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    // 设置光照
    GLfloat light0Pos[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat ambientLight[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat diffuseLight[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat specref[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    glMateriali(GL_FRONT, GL_SHININESS, 5);
    // 太阳自发光
    GLfloat sunEmission[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, sunEmission);
    // 绘制太阳
    glRotatef((GLfloat)sunRotation, 0.0, 1.0, 0.0);
    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricTexture(quadObj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    gluSphere(quadObj, 1.5, 20, 20);
    gluDeleteQuadric(quadObj);
    glPopMatrix();
    // 地球与月球不发光
    GLfloat planetEmission[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, planetEmission);
    glPushMatrix();
    // 绘制地球
    glRotatef((GLfloat)earthRevolution, 0.0, 1.0, 0.0);
    glTranslatef(4.0, 0.0, 0.0);
    glPushMatrix();
    glRotatef((GLfloat)earthRotation, 0.0, 1.0, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    quadObj = gluNewQuadric();
    gluQuadricTexture(quadObj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    glRotatef(180, 1.0, 0.0, 0.0);
    gluSphere(quadObj, 0.6, 20, 20);
    gluDeleteQuadric(quadObj);
    glPopMatrix();
    // 绘制月球
    glRotatef((GLfloat)moonRevolution, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glRotatef((GLfloat)moonRotation, 0.0, 1.0, 0.0);
    quadObj = gluNewQuadric();
    gluQuadricTexture(quadObj, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, moonTexture);
    gluSphere(quadObj, 0.2, 20, 20);
    gluDeleteQuadric(quadObj);
    glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}

void timer(int value)
{
    if (animated)
    {
        sunRotation = (sunRotation + 1*speed) % 360;
        earthRevolution = (earthRevolution + 3*speed) % 360;
        earthRotation = (earthRotation + 6*speed) % 360;
        moonRotation = moonRevolution = (moonRevolution + 9*speed) % 360;
        glutPostRedisplay();
    }
    glutTimerFunc(50, timer, 1);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'q':   // 侧视观察
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0, 0, -10, 0, 0, 0, 0, 1, 0);
//            第一组eyex, eyey,eyez 相机在世界坐标的位置
//            第二组centerx,centery,centerz 相机镜头对准的物体在世界坐标的位置
//            第三组upx,upy,upz 相机向上的方向在世界坐标中的方向
            break;
        case 'w':   // 俯视观察
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0, 10, 0, 0, 0, 0, 0, 0, 1);
            break;
        case 'r':   // 开启动画
            animated = true;
            break;
        case 't':   // 暂停动画
            animated = false;
            break;
        case '1':
            speed = 1;
            break;
        case '2':
            speed = 2;
            break;
        case '3':
            speed = 8;
            break;
        case '4':
            speed = 0;
            break;
        case '5':
            speed = -1;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(300, 250);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Project 2");
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(50, timer, 1);
    glutMainLoop();
    return 0;
}
