#define STB_IMAGE_IMPLEMENTATION

#include <gl/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "stb_image.h"

static float r1 = 0, r2 = 0 , r3 = 0;
GLuint texture[3];
GLUquadric* qobj[3];
GLfloat ambientLight[] = { 0.05f, 0.05f, 0.05f, 1.0f };//环境光
GLfloat diffuseLight[] = { 0.95f, 0.95f, 0.95f, 1.0f };//漫反射光
GLfloat specular[] = { 1.0f, 1.0f , 1.0f , 1.0f };//镜面光
GLfloat gray[] = { 0.75f, 0.75f, 0.75f, 1.0f };

GLfloat xOffset = 0.0f, yOffset = 0.0f, zOffset = 0.0f;

/*
加载纹理
*/
void LoadGLTextures(GLuint* texture,const char* bmp_file_name, int tex_id) {
	

	glGenTextures(1, &texture[tex_id]);
	glBindTexture(GL_TEXTURE_2D, texture[tex_id]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(bmp_file_name, &width, &height, &nrChannels, 0);
	std::cout << "found tex channel:" << nrChannels << std::endl;
	if (data)
	{
		if(nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 1)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
		else
			std::cout << "unknow channel" << std::endl;
		std::cout << "load texture" << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}


/*
初始化
*/
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glClearDepth(1.0f);
	//glEnable(GL_DEPTH_TEST);
	//glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	//glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	//glEnable(GL_TEXTURE_GEN_S);
	//glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	//glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);        // Do not calculate inside of 
	glFrontFace(GL_CCW);        // Counterclockwise polygons face
	
	//光照
	glEnable(GL_LIGHTING);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	
	//glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	//glEnable(GL_LIGHT1);
	//材质
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	LoadGLTextures(texture, "sunmap.tga", 0);
	LoadGLTextures(texture, "earthmap.tga", 1);
	LoadGLTextures(texture, "moonmap.tga", 2);
	qobj[0] = gluNewQuadric();
	qobj[1] = gluNewQuadric();
	qobj[2] = gluNewQuadric();
}

/*
绘制
*/
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	glPushMatrix();
	gluLookAt(0.0, 0.0, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef(xOffset, yOffset, zOffset);

	//设定光源
	GLfloat lightPos[] = { 0.f, 0.0f, 0.f , 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	//绘制太阳	
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glRotatef((GLfloat)r1, 0.0, 1.0, 0.0); 
	gluQuadricTexture(qobj[0], GL_TRUE);
	gluSphere(qobj[0], 0.8, 30, 30);
	glEnable(GL_LIGHTING);

	//绘制地球
	glRotatef((GLfloat)r1, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);
	glRotatef((GLfloat)r2, 0.0, -1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	gluQuadricTexture(qobj[1], GL_TRUE);
	gluSphere(qobj[1], 0.4, 30, 30);


	//绘制月球
	glRotatef((GLfloat)r3, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glRotatef((GLfloat)r2, 0.0, 1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	gluQuadricTexture(qobj[2], GL_TRUE);
	gluSphere(qobj[2], 0.2, 20, 20);
	glPopMatrix();
	glutSwapBuffers();


}

/*
旋转角度设置
*/
void updateDisplay(void)
{
	//设置转速
	r1 = r1 + 0.5;
	if (r1 > 360.0)
	{
		r1 = r1 - 360.0;
	}
	r2 = r2 + 2;
	if (r2 > 360.0)
	{
		r2 = r2 - 360.0;
	}
	r3 = r3 + 4;
	if (r3 > 360.0)
	{
		r3 = r3 - 360.0;
	}
	glutPostRedisplay();
}

/*
窗口大小改变
*/
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
键盘控制
*/
void keyboard(unsigned char key, GLint x, GLint y)
{
	if (key == 's')
	{
		yOffset += 0.5;
	}
	if (key == 'd')
	{
		xOffset -= 0.5;
	}
	if (key == 'w')
	{
		yOffset -= 0.5;
	}
	if (key == 'a')
	{
		xOffset += 0.5;
	}
	if (key == 'q')
	{
		zOffset += 0.5;
	}
	if (key == 'e')
	{
		zOffset -= 0.5;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GLsecond");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(updateDisplay);
	glutMainLoop();
	return 0;
}