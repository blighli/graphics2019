//
//  main.cpp
//  LearnOpenGL
//
//  Created by yingyingzhu on 2019/9/7.
//  Copyright © 2019 yingyingzhu. All rights reserved.
//
#include <iostream>
#include <glut.h>
#include <math.h>

using namespace std;


#define PI 3.1415926
void Rotate();
static float year = 0, day = 0, sunYear = 0, month = 0;
float light_angle = 0;
float light_radius = 8.0;
double x = 0, y = 0;
double aix_x = 0.0, aix_y = 0.5, aix_z = 0.5;
GLdouble angle = 100.0;

void lPosition()
{
	float y, z;
	y = light_radius * cos(light_angle);
	z = light_radius * sin(light_angle);
	float light_position[] = { 3.0,y,z,0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//void glLightfv (GLenum light, GLenum pname, const GLfloat *params);
	//创建指定的光源,light可以是GL_LIGHT0、GL_LIGHT1，pname定义的是光源的属性，它指定了一个命名参数。params表示表示pname属性将要被设置的值
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	lPosition();
	glShadeModel(GL_SMOOTH);   //将制定的两点颜色进行平滑插值
	glEnable(GL_LIGHTING);     //启用灯源
	glEnable(GL_LIGHT0);       //启用0号灯光源
	glEnable(GL_DEPTH_TEST);   //启用深度测试。根据坐标的远近自动隐蔽被遮挡的图形
	glEnable(GL_COLOR_MATERIAL);  //执行后，图形将根据光线的照耀进行反射。
}

void material_sun()          //设置太阳材质
{
	GLfloat mat_specular[] = { 1.0,0.0,0.0,1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat lmodel_ambient[] = { 1.0,0.0,0.0,1.0 };  //太阳颜色为红色
	GLfloat white_light[] = { 1.0,1.0,1.0,1.0 };
	//指定用于计算的当前材质属性。
	//    void WINAPI glMaterialfv(
	//                             GLenum face,
	//                             GLenum pname,
	//                             const GLfloat *params
	//                             );
	//指定用于光照计算的当前材质属性。参数face的取值GL_FRONT,GL_BACK或GL_FRONT_AND_BACK,指出材质属性将应用于物体的哪面。
	//pname指出要设置的哪种材质属性。param为要设置的属性值，是一个指向数组的指针（向量版本）或一个数值（非向量版本）。只有设置参数值是GL_SHININESS，才能使用非向量版本。
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}

void material_moon()
{
	GLfloat mat_specular[] = { 1.0,0,0,0.0,1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat lmodel_ambient[] = { 1.0,1.0,0.0,1.0 };
	GLfloat white_light[] = { 1.0,1.0,1.0,1.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}

void material_earth()
{
	GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat lmodel_ambient[] = { 0.1,0.2,0.6,1.0 };
	GLfloat white_light[] = { 1.0, 1.0,1.0, 1.0 };


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}

void sun()
{
	glPushMatrix();
	material_sun();
	glTranslatef(-15, 0, 0);
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);//太阳自转
	glutSolidSphere(10, 200, 200); //画球体
	glPopMatrix();
}

void earth()
{
	glPushMatrix();
	material_earth();
	glTranslatef(-15, 0, 0);
	glRotatef((GLfloat)year, aix_x, aix_y, aix_z);
	glTranslatef(15, 0, 0);
	glTranslatef(-2, 0, 0);
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
	glTranslatef(2, 0, 0);
	glutSolidSphere(1.0, 20, 16);
	material_moon();
	glRotatef(month / 100, 0.0, 0.0, 1.0);
	glTranslatef(2, 0.0, 0.0);
	glutSolidSphere(0.3, 10, 10);
	glPopMatrix();
}

void display(void)
{
	lPosition();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//可以用|运算符组合不同的缓冲标志位，表明要清除的缓冲，上述表示要清除颜色缓冲以及深度缓冲，可以使用以下标志位
	//GL_COLOR_BUFFER_BIT:当前可写的颜色缓冲
	//GL_DEPTH_BUFFER_BIT:深度缓冲
	//GL_ACCUM_BUFFER_BIT:累积缓冲
	//GL_STENCIL_BUFFER_BIT:模板缓冲

	sun();
	earth();

	Rotate();
	glutSwapBuffers(); //交换两个缓冲区指针
}


void myidle()
{
	day += 10.0;
	if (day >= 360)
		day = day - 360;
	glutPostRedisplay();
}

void Rotate()          //设置地球的公转周期
{

	//  year=(year+0.8)%360;
	year += 0.08;
	if (year >= 360)
		year -= 360;
	glutPostRedisplay();
	month += 0.03;
	if (month >= 360)
		month -= 360;
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle, (GLfloat)w / (GLfloat)h, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -50.0);
}
int main(int argc, char** argv)
{
	cout << argc << endl;
	cout << &argc << endl;

	for (int i = 0; i < argc; i++)
	{
		cout << *argv[i] << endl;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("earth system");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(myidle);
	glutMainLoop();
	return 0;
}