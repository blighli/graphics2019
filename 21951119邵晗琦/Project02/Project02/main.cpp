#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include"stb_image.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <math.h>


GLfloat whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };


void RenderScene(void)
{
	//旋转角度
	static float fMoonRot = 0.0f;
	static float fEarthRot = 0.0f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 矩阵
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -300.0f);
	// 太阳
	glDisable(GL_LIGHTING);
	glColor3ub(255, 255, 0);
	glutSolidSphere(15.0f, 30, 17);
	glEnable(GL_LIGHTING);

	//光源
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);

	//画地球
	glColor3ub(0, 0, 255);
	glTranslatef(105.0f, 0.0f, 0.0f);
	glutSolidSphere(15.0f, 30, 17);
	//月亮
	glColor3ub(200, 200, 200);
	glRotatef(fMoonRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(30.0f, 0.0f, 0.0f);
	fMoonRot += 15.0f;
	if (fMoonRot > 360.0f)
		fMoonRot = 0.0f;

	glutSolidSphere(6.0f, 30, 17);
	glPopMatrix();


	fEarthRot += 5.0f;
	if (fEarthRot > 360.0f)
		fEarthRot = 0.0f;

	glutSwapBuffers();
}


// 渲染初始化函数，上下文环境设置. 
void SetupRC()
{

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);


	glEnable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);


	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void TimerFunc(int value)
{
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunc, 1);
}

void ChangeSize(int w, int h)
{
	GLfloat fAspect;

	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
， 
	gluPerspective(45.0f, fAspect, 1.0, 425.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Earth/Moon/Sun System");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(250, TimerFunc, 1);
	SetupRC();
	glutMainLoop();

	return 0;
}
