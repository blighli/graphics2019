#define STB_IMAGE_IMPLEMENTATION

#include <gl/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "stb_image.h"


using namespace std;

GLfloat xOffset = 0.0f, yOffset = 0.0f, zOffset = 0.0f;
GLfloat angle_x = 0.0f, angle_y = 0.0f, angle_z = 0.0f;
GLint slices = 20;
GLint stacks = 20;
GLdouble r = 2.0;

GLuint texture;

GLfloat points[8][3] = { {1,1,-1},{-1,1,-1},{-1,1,1},{1,1,1},{1,-1,1},{-1,-1,1},{-1,-1,-1},{1,-1,-1} };
GLfloat tex[4][2] = { {1,1},{0,1},{0,0},{1,0} };
GLint vertice[6][4] = { {0,1,2,3},{4,5,6,7},{3,2,5,4},{7,6,1,0},{2,1,6,5},{0,3,4,7} };

/*
加载纹理
*/
void LoadGLTextures() {   

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("cat.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		std::cout << "load texture" << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}


/*
绘制立方体
*/
void CreateCube()
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			glTexCoord2fv(tex[j]);
			int pid = vertice[i][j];
			glVertex3fv(points[pid]);
		}
	}
	glEnd();
}

void init(void)
{

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	LoadGLTextures();

	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  
	gluLookAt(0.0, 0.0, 5.0, 
		0.0, 0.0, 0.0, 
		0.0, 1.0, 0.); 

	//angle_x = 0.1f;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glColor3f(0.0, 1.0, 0.0); 
	glLoadIdentity();
	glViewport(0, 0, 600, 600);
	glTranslatef(0, 0, -5);
	glTranslatef(xOffset, yOffset, zOffset);
	angle_y += 0.5f;
	angle_z += 0.5f;
	glRotatef(angle_x, 1.0, 0.0, 0.0);
	glRotatef(angle_y, 0.0, 1.0, 0.0);
	glRotatef(angle_z, 0.0, 0.0, 1.0);
	CreateCube();
	//glutWireSphere(r, slices, stacks);
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

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
	display();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GLfirst");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}