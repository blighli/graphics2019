//  ConsoleApplication1.cpp : 定义控制台应用程序的入口点

#include "BallDefinition.h"

static int windowWidth = 1000, windowHeight = 600;
static int windowPosX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
static int windowPosY = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;

void Init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);	// 启用二维纹理

	GLfloat mat_shininess[] = { 10.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };			// 光源设置
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);		// 0号光源的位置
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);	// 使用lmodel_ambien

	LoadAllTextures();		// 调入纹理
}

void Myidle()
{
	day += angle;
	glutPostRedisplay();
}

void Display()
{
	GLfloat whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 2.0f };

	year = day / 365;
	month = day / 30;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat mat_ambient1[] = { 1, 0, 0, 1 };
	GLfloat mat_emission[] = { 1, 1, 1, 0 };

	GLfloat mat_ambient2[] = { 0.4, 0.4, 0.8, 1 };
	GLfloat  no_emission[] = { 0, 0, 0, 1 };

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, sun->texID);
	glRotatef(month, 0.0, 1.0, 0.0);			// 太阳自转
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient1);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glRotatef(90, -1, 0, 0);

	glDisable(GL_LIGHTING);
	gltDrawSphere(1.0, 60, 40);				// 绘制太阳
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glPopMatrix();

	glPushMatrix();
	glRotatef(year, 0.0, 1.0, 0.0);				// 月亮与地球一起绕太阳转（地球公转）
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, earth->texID);
	glTranslatef(3.0, 0.0, 0.0);
	glRotatef(month, 0.0, 1.0, 0.0);			// 地球自转 
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
	glRotatef(90, -1, 0, 0);
	gltDrawSphere(0.4, 40, 40);				// 绘制地球	
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, moon->texID);
	glTranslatef(3.0, 0.0, 0.0);
	glRotatef(60, -1, 1, 0);
	glRotatef(month, 0, 1.0, 0);				// 月球绕地球转
	glTranslatef(0.6, 0.0, 0.0);
	glRotatef(90, -1, 0, 0);
	gltDrawSphere(0.07, 20, 20);				// 绘制月球
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();

}

void Myreshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);				// 指定视口大小
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1, 20);		// 透视投影

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	// 指定照相机的位置	
}

int main(int argc, char** argv)
{
	// 对GLUT进行初始化，并处理所有命令行参数
	glutInit(&argc, argv);

	// 指定使用RGBA模式和双缓冲窗口
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// 设置窗口创建时的宽高
	glutInitWindowPosition(windowPosX, windowPosY);
	glutInitWindowSize(windowWidth, windowHeight);

	// 创建一个窗口
	glutCreateWindow("OpenGL作业2");
	Init();

	// 回调函数，当窗口内容需要更新显示时，注册的回调函数会被执行
	glutDisplayFunc(Display);
	glutReshapeFunc(Myreshape);
	glutIdleFunc(Myidle);

	glutMainLoop();
	return 0;
}