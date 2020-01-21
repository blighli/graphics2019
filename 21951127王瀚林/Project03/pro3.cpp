#include "ObjLoader.h"
#include "iostream"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

string filePath = "bunny.obj";
ObjLoader objModel = ObjLoader(filePath);
static int degreeY = 0;
static int degreeX = 90;
static int oldPosY = -1;
static int oldPosX = -1;
GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };//环境光
GLfloat diffuseLight[] = { 0.30f, 0.30f, 0.30f, 1.0f };//漫反射光
GLfloat specular[] = { 0.30f, 0.30f , 0.30f , 1.0f };//镜面光
//GLfloat gray[] = { 0.75f, 0.75f, 0.75f, 1.0f };
GLfloat lightPosition1[] = { 10.0f, 0.0f, 0.0f, 0.0f };
GLfloat lightPosition2[] = { 0.0f, 10.0f, 0.0f, 0.0f };
bool lightSwitch1 = true; //灯光1
bool lightSwitch2 = true; //灯光2
bool modeSwitch = true; //操作切换
static GLfloat s_eye[] = { 0.0, 0.0, 5.0 };
static GLfloat s_at[] = { 0.0, 0.0, 0.0 };
static GLfloat s_angle = -90.0;
static GLfloat s_angle_h = 180.0;
float PI = 3.14159f;
float speed = 1.0;

GLfloat xOffset = 0.0f, yOffset = 0.0f, zOffset = 0.0f;

//启用光照
void setLightRes() {
	if (lightSwitch1) {
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	else {
		glDisable(GL_LIGHT0);
	}

	if (lightSwitch2) {
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHT1);
	}

}


//初始化
void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("ThirdGL");
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	setLightRes();
	glEnable(GL_DEPTH_TEST);
}

void display()
{
	glColor3f(1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);                            
	glLoadIdentity();                              												
	setLightRes();

	//gluLookAt(0.0, 0.0, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//设定camera
	gluLookAt(s_eye[0], s_eye[1], s_eye[2], s_at[0], s_at[1], s_at[2], 0.0, 1.0, 0.0);
	//物体移动
	glTranslatef(xOffset, yOffset, zOffset);
	//gluLookAt(r*cos(c*degree), 0, r*sin(c*degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glScalef(0.1, 0.1, 0.1);
	glTranslatef(0, 0, -1000);
	glRotatef(degreeX, 0, 1, 0);
    glRotatef(degreeY, 1, 0, 0);
	objModel.Draw();
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
}


//鼠标移动回调
void moseMove(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		oldPosX = x; oldPosY = y;
	}
}

//点击回调实现鼠标视角操作
void changeViewPoint(int x, int y)
{
	if (modeSwitch) {
		int temp = x - oldPosX;
		degreeX += temp;
		temp = y - oldPosY;
		degreeY += temp;

	}
	else {
		int temp = x - oldPosX;
		s_angle += temp;
		temp = y - oldPosY;
		s_angle_h += temp;
		if (s_angle_h < 95.0)s_angle_h = 95.0;
		if (s_angle_h > 265.0)s_angle_h = 265.0;
		float rad = float(PI * s_angle / 180.0f);
		float rad_h = float(PI * s_angle_h / 180.0f);
		s_at[0] = float(s_eye[0] + 100 * cos(rad));
		s_at[2] = float(s_eye[2] + 100 * sin(rad) + 100 * cos(rad_h));
		s_at[1] = float(s_eye[1] + 100 * sin(rad_h));
	}
	oldPosX = x;
	oldPosY = y;
}


void idle()
{
	glutPostRedisplay();
}

//模型控制操作
void keyboardSetting1(unsigned char key, GLint x, GLint y)
{
	if (key == 's')
	{
		yOffset -= 5.0;
	}
	if (key == 'd')
	{
		xOffset += 5.0;
	}
	if (key == 'w')
	{
		yOffset += 5.0;
	}
	if (key == 'a')
	{
		xOffset -= 5.0;
	}
	if (key == 'q')
	{
		zOffset += 5.0;
	}
	if (key == 'e')
	{
		zOffset -= 5.0;
	}
	if (key == 'q')
	{
		zOffset += 5.0;
	}
	if (key == 'e')
	{
		zOffset -= 5.0;
	}
}

//场景漫游操作
void keyboardSetting2(unsigned char key, GLint x, GLint y)
{
	float rad = float(PI * s_angle / 180.0f);    
	float rad_h = float(PI * s_angle_h / 180.0f);
	if (key == 'w')
	{
		s_eye[2] += (float)(sin(rad)+cos(rad_h)) * speed;
		s_eye[1] += (float)sin(rad_h) * speed;
		s_eye[0] += (float)cos(rad) * speed;

	}
	if (key == 's')
	{
		s_eye[2] -= (float)(sin(rad) + cos(rad_h)) * speed;
		s_eye[1] -= (float)sin(rad_h) * speed;
		s_eye[0] -= (float)cos(rad) * speed;
	}
	//设定朝向
	s_at[0] = float(s_eye[0] + 100 * cos(rad));
	s_at[2] = float(s_eye[2] + 100 * sin(rad) + cos(rad_h));
	s_at[1] = float(s_eye[1] + 100 * sin(rad_h));
}

void reset() {
	int degreeY = 0;
	int degreeX = 90;
	lightSwitch1 = true;
	lightSwitch2 = true;
	for (int i = 0; i < 3; i++) {
		s_eye[i] = 0.0;
		s_at[i] = 0.0;
	}
	s_eye[2] = 5.0;
	s_angle = -90.0;
	s_angle_h = 180.0;
}


//键盘控制回调
void keyboard(unsigned char key, GLint x, GLint y)
{
	if (modeSwitch) {
		keyboardSetting1(key, x, y);
	}	
	else
	{
		keyboardSetting2(key, x, y);
	}
	
	if (key == '1')
	{
		lightSwitch1 = !lightSwitch1;
	}
	if (key == '2')
	{
		lightSwitch2 = !lightSwitch2;
	}
	if (key == '3')
	{
		modeSwitch = !modeSwitch;
		reset();
	}
	glutPostRedisplay();
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(moseMove);
	glutMotionFunc(changeViewPoint);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
