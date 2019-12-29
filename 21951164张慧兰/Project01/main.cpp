#include <GL/glew.h>
#include <freeglut.h>


//定义变换控制变量
float fRotate = 0.0f;
float fScale = 1.0f;   
float fRevolve = 0.0f;
bool bPersp = false;
bool bAnim = false;//about旋转
bool bWire = false;
bool tAnim = false;
//定义窗口大小
int wHeight = 0;
int wWidth = 0;

void Draw_Leg()
{
	glScalef(1, 1, 3);//缩放
	glutSolidCube(1.0);//创建实心立方体
}

void Draw_Scene()
{   //画茶壶 
	glPushMatrix();
	glTranslatef(0, 0, 4 + 1);//移动
	glRotatef(90, 1, 0, 0); //旋转
	//（0,5,0）
	glutSolidTeapot(1);
	glPopMatrix();
	//画桌面
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();
	//画四条腿
	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

}

void updateView(int width, int height)//切换投影方式
{
	glViewport(0, 0, width, height);                        // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
	glLoadIdentity();                                    // Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;

	if (bPersp){
		//todo when 'p' operation, hint: use FUNCTION gluPerspective
	}
	else
		glOrtho(-3, 3, -3, 3, -100, 100);

	glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height == 0)                                        // Prevent A Divide By Zero By
	{
		height = 1;                                        // Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();//窗口重新绘制
}
float size = 0.1f;
float eye[] = { 0, 0, 8 };
float center[] = { 0, 0, 0 };
float move[] = { 0, 0, 0 };
//用键盘操控图形
void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	//相机移动
	case 'a': {
				  eye[0] = 3;
				  eye[1] = 0;
				  eye[2] = 8;
				  break;
	}
	case 'd': {
				  eye[0] = -3;
				  eye[1] = 0;
				  eye[2] = 8;
				  break;
	}
	case 'w': {
				  eye[0] = 0;
				  eye[1] = 3;
				  eye[2] = 8;
				  break;
	}
	case 's': {
				  eye[0] = 0;
				  eye[1] = -3;
				  eye[2] = 8;
				  break;
	}
	}
}


void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();                                    // Reset The Current Modelview Matrix
	//视点转换
	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);                // 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上
	//线框模式和填充模式转换
	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	//光照
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);//启动光照
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { 5, 5, 5, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);//光源位置，颜色
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);//环境光
	glEnable(GL_LIGHT0);//启动光源

	//    glTranslatef(0.0f, 0.0f,-6.0f);            // Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);            // Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Scene();    // Draw Scene

	glRotatef(fRevolve, 0, 1.0f, 0);            // Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	glutSolidTeapot(1);

	glTranslatef(move[0], move[1], move[2]);
	glutSolidTeapot(1);

	if (bAnim) fRotate += 0.5f;
	if (tAnim) fRevolve += 0.3f;

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("作业1");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}

