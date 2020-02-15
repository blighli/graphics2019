#define NDEBUG
#include "stdafx.h"

#include <stdio.h>
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

#define WIRE_SHADING  1
#define SOLID_SHADING 2
#define LIGHT_SHADING 4
GLenum  G_shadingMode;	//三种绘制模式

//摄像机离物体的距离
float G_fDistance = 6.0f;
float G_fAngle_horizon = 0.0f;
float G_fAngle_vertical = 0.0f;

float G_fSunToEarth = 3.0f;
float G_fEarthToMoon = 0.5f;

//物体的旋转角度 
float G_fAngle_earth = 90.0f;
float G_fAngle_moon = 90.0f;

GLboolean G_bPause = false;

////////////////////////////////////////////////
//光照参数
float G_vLit0Position[4] = { 5.0f, 0.0f, 5.0f, 1.0f };
float G_vLit0Ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
float G_vLit0Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
float G_vLit0Specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
float G_vMaterialSpecu[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

float G_vLit1Position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
////////////////////////////////////////////////
void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);

//响应键盘输入, 从而设定物体移近移远以及旋转的回调函数
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key,int x,int y);

void planetMove(int value);

////////////////////////////////////////////////
//主函数
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	//初始化OPENGL显示方式
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
//	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);

	//设定OPENGL窗口位置和大小
	glutInitWindowSize (1000, 500); 
	glutInitWindowPosition (100, 100);

	//打开窗口
	glutCreateWindow ("OpenGL");
	printf("上、下键---绕x轴旋转\n");
	printf("左、右键---绕y轴旋转\n");
	printf("a/A键------拉近/推远\n");
	printf("空格键-----开始/暂停运动\n");
	printf("1/2/3键----线框/实体/光照绘制模式\n");

  myinit();

	glutReshapeFunc(myReshape);

	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	glutTimerFunc(10, planetMove, 0);

	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}

////////////////////////////////////////////////
//用户初始化函数
void myinit(void)
{
  //your initialization code
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
}

//窗口大小变化时的回调函数
void myReshape(GLsizei w, GLsizei h)
{
	//设定视区
    glViewport(0, 0, w, h);

	//设定透视方式
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60.0, 1.0*(GLfloat)w/(GLfloat)h, 1.0, 30.0);
}

void display(void)
{
	//设置清除屏幕的颜色，并清除屏幕和深度缓冲
    glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//shading model
	if(G_shadingMode == WIRE_SHADING)
	{
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if(G_shadingMode == SOLID_SHADING)
	{
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if(G_shadingMode == LIGHT_SHADING)
	{
		glEnable(GL_LIGHTING);
	}


	////////////////////////////////////////////////
	//设置光照
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, G_vLit0Position);		//设置光源的位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, G_vLit0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, G_vLit0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, G_vLit0Specular);

	////////////////////////////////////////////////
	//绘图
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	//坐标中心向Z轴平移-G_fDistance (使坐标中心位于摄像机前方)
    glTranslatef(0.0, 0.0, -G_fDistance);
	glRotatef(G_fAngle_horizon, 0.0, 1.0, 0.0);
	glRotatef(G_fAngle_vertical, 1.0, 0.0, 0.0);

	//the Sun
	glColor3f(0.8f, 0.8f, 0.0f);
	glutSolidSphere(0.5, 20, 16);
	
	//the Earth
	glRotatef(G_fAngle_earth, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0, 0.0, -G_fSunToEarth);
	glColor3f(0.0f, 0.8f, 0.0f);
	glutSolidSphere(0.3, 20, 16);
	
	//the Moon
	glRotatef(-30, 0.0f, 0.0f, 1.0f);	//月球轨道倾斜-30度
	glRotatef(G_fAngle_moon, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0, 0.0, -G_fEarthToMoon);
	glColor3f(0.0f, 0.8f, 0.8f);
	glutSolidSphere(0.1, 20, 16);

	glutSwapBuffers();
}


void processSpecialKeys(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_LEFT:
			G_fAngle_horizon -= 5.0f;
			break;
		case GLUT_KEY_RIGHT:
			G_fAngle_horizon += 5.0f;
			break;
		case GLUT_KEY_UP:
			G_fAngle_vertical -= 5.0f;
			break;
		case GLUT_KEY_DOWN:
			G_fAngle_vertical += 5.0f;
			break;
	}
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key,int x,int y)
{
	switch(key) {
		case 97:	//"a"
			G_fDistance -= 0.3f;
			break;
		case 65:		//"A"
			G_fDistance += 0.3f;
			break;
		case 32:	//space
			G_bPause = !G_bPause;
			break;
		case 49:	//"1"
			G_shadingMode = WIRE_SHADING;
			break;
		case 50:	//"2"
			G_shadingMode = SOLID_SHADING;
			break;
		case 51:	//"3"
			G_shadingMode = LIGHT_SHADING;
			break;
		case 27:	//"esc"
			exit(0);
	}
	glutPostRedisplay();
}

void planetMove(int value)
{
	if(!G_bPause)
	{
		G_fAngle_earth += 0.2f;
		G_fAngle_moon += 1.5f;

		glutPostRedisplay();
	}
	glutTimerFunc(10, planetMove, 0);
}
