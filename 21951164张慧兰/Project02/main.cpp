#include <freeglut.h>
#include <math.h>


float seta = 200;
float mx = 0, my = 0, mz = 50, rx = 0, ry = 0, rz = 0;//平移和旋转
float mspeed = 5, rspeed = 1;
static int oldrx = 0, oldry = 0;//旋转
static int oldmx = 0, oldmy = 0;//平移
static int oldpx = 0, oldpy = 0;
bool a = false, b = false;
void myDisplay(void);

static GLuint texName;//纹理对象


void myTimerFunc(int val)
{
	++seta;
	if (seta >= 360)
		seta = 0;//每年为360天，超过360从0再开始
	myDisplay();
	glutTimerFunc(1, myTimerFunc, 0);
}

void SetRC()
{
	glShadeModel(GL_FLAT);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float amb[4] = { 0.8, 0.0, 0.0, 1 };					//红色环境光
	float dif[4] = { 1.0, 1.0, 1.0, 1 };					//白色漫射光和反射光
	float spe[4] = { 1.0, 1.0, 1.0, 1 };                   //黑色镜面反射
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, amb);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glPolygonMode(GL_BACK, GL_LINE);
}
void SetView()
{
	glRotatef(-ry, 0, 1, 0);		//h
	glRotatef(-rx, 1, 0, 0);		//p
	glRotatef(-rz, 0, 0, 1);		//r
	glTranslatef(-mx, -my, -mz);
}
void RenderWorld()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0, 5, -25);
	glRotatef(seta, 0, 1, 0);
	glPopMatrix();

	//太阳
	glPushMatrix();
	glTranslatef(0, -100, -600);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 0, 0);
	glRotatef(seta * 2, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidSphere(70, 100, 100);
	float pos[4] = { 0, 0, 0, 1 };    //以太阳为光源
	glLightfv(GL_LIGHT0, GL_POSITION, pos);	//设置光源位置
	glPopMatrix();
	//地球
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(0, 0, 1);
	glRotatef(seta, 0, 1, 0);//公转
	glTranslatef(175, 0, 0);
	glRotatef(seta, 0, 1, 0);//自转
	glRotatef(90, 1, 0, 0);

	glutSolidSphere(16, 200, 200);//地球
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 1, 0);
	glRotatef(seta * 6, 0, 0, -1);
	glTranslatef(35, 0, 0);
	glRotatef(seta * 1, 0, 1, 0);//月球自转
	glutSolidSphere(4.5, 60, 60);
	glPopMatrix();//月球

}
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	SetView();
	RenderWorld();
	//模型控制
	glPopMatrix();
	glutSwapBuffers();
}

void myReshape(int w, int h)
{
	GLfloat nRange = 100.0f;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, GLfloat(w) / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("作业2");
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(1, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	SetRC();
	glutMainLoop();
	return 0;
}