#include "stdafx.h"

//需要包含的头文件
#include <windows.h>

#include <GL/freeglut.h>

//定义输出窗口的大小
#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

//摄像机离物体的距离
GLfloat G_fDistance = 3.6f;
//物体的旋转角度 
GLfloat G_fAngle_horizon = 0.0f;
GLfloat G_fAngle_vertical = 0.0f;

////////////////////////////////////////////////
void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);

//响应键盘输入, 从而设定物体移近移远以及旋转的回调函数
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key,int x,int y);


////////////////////////////////////////////////
//主函数
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	//初始化OPENGL显示方式
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);

	//设定OPENGL窗口位置和大小
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
		
	//打开窗口
	glutCreateWindow ("hw1");

	//调用初始化函数
  myinit();

	//设定窗口大小变化的回调函数
	glutReshapeFunc(myReshape);

	//设定键盘控制的回调函数
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processNormalKeys);
	
	//开始OPENGL的循环
	glutDisplayFunc(display); 

	glutIdleFunc(display);

	glutMainLoop();

	return 0;
}

////////////////////////////////////////////////
//用户初始化函数
void myinit(void)
{
    //your initialization code
	  glEnable(GL_DEPTH_TEST);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//设置成模型矩阵模式
    glMatrixMode(GL_MODELVIEW);

	//载入单位化矩阵
    glLoadIdentity();

	//坐标中心向Z轴平移-G_fDistance (使坐标中心位于摄像机前方)
    glTranslatef(0.0, 0.0, -G_fDistance);
	  glRotatef(G_fAngle_horizon, 0.0f, 1.0f, 0.0f);
	  glRotatef(G_fAngle_vertical, 1.0f, 0.0f, 0.0f);

	////////////////////////////////////////////////
	//绘制物体

	//画一个茶壶
	glColor4f(1.0f, 0.98f, 0.94f, 1.0f);
	glutWireTeapot(1.0);

	//交换前后缓冲区
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
		case 27:	//"esc"
			exit(0);
	}
}
