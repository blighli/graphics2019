#define BITMAP_ID 0x4D42
#define PI 3.1415926

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glut.h>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

// 纹理数据结构
typedef struct {
	int width;				// 纹理宽度
	int height;				// 纹理高度
	unsigned int texID;		// 纹理对象 ID
	unsigned char* data;		// 实际纹理数据
}texture;

texture* sun, * earth, * moon;			// 纹理指针
static float year = 0, month = 0, day = 0, angle = 30;
static bool first = false;

// 调入位图作为纹理数据
unsigned char* LoadBmpFile(const char* filename, BITMAPINFOHEADER* bmpInfoHeader)
{
	FILE* file;
	BITMAPFILEHEADER bmpFileHeader;
	unsigned char* image;
	unsigned int imageIdx = 0;
	unsigned char tempRGB;

	file = fopen(filename, "rb");
	if (file == NULL)
		return 0;

	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);			// 读取 BMP 文件头

	if (bmpFileHeader.bfType != BITMAP_ID)							// 验证是否是一个 BMP 文件
	{
		fclose(file);
		return 0;
	}

	fread(bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);			// 读位图信息头
	fseek(file, bmpFileHeader.bfOffBits, SEEK_SET);					// 将文件指针移到位图数据的开始处
	image = (unsigned char*)malloc(bmpInfoHeader->biSizeImage);		// 分配内存给位图数据

	if (!image)
	{
		free(image);
		fclose(file);
		return 0;
	}

	fread(image, 1, bmpInfoHeader->biSizeImage, file);				// 读取位图数据

	if (image == NULL)
	{
		fclose(file);
		return 0;
	}

	// 反转 R 和 B 值以得到 RGB，因为位图颜色格式是 BGR
	for (imageIdx = 0; imageIdx < bmpInfoHeader->biSizeImage; imageIdx += 3)
	{
		tempRGB = image[imageIdx];
		image[imageIdx] = image[imageIdx + 2];
		image[imageIdx + 2] = tempRGB;
	}

	fclose(file);
	return image;
}

// 调入纹理文件
texture* LoadTexFile(const char* filename)
{
	BITMAPINFOHEADER texInfo;
	texture* thisTexture;

	thisTexture = (texture*)malloc(sizeof(texture));
	if (thisTexture == NULL)
		return 0;

	thisTexture->data = LoadBmpFile(filename, &texInfo);	// 调入纹理数据并检查有效性
	if (thisTexture->data == NULL)
	{
		free(thisTexture);
		return 0;
	}

	thisTexture->width = texInfo.biWidth;				// 设置纹理的宽和高
	thisTexture->height = texInfo.biHeight;

	glGenTextures(1, &thisTexture->texID);				//  生成纹理对象名

	return thisTexture;
}

// 初始化所有纹理数据和属性
BOOL LoadAllTextures()
{
	sun = LoadTexFile("./images/ty.bmp");
	if (sun == NULL)
		return FALSE;

	glBindTexture(GL_TEXTURE_2D, sun->texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, sun->width, sun->height, GL_RGB, GL_UNSIGNED_BYTE, sun->data);

	earth = LoadTexFile("./images/dq.bmp");
	if (earth == NULL)
		return FALSE;

	glBindTexture(GL_TEXTURE_2D, earth->texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, earth->width, earth->height, GL_RGB, GL_UNSIGNED_BYTE, earth->data);

	moon = LoadTexFile("./images/yl.bmp");
	if (moon == NULL)
		return FALSE;

	glBindTexture(GL_TEXTURE_2D, moon->texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, moon->width, moon->height, GL_RGB, GL_UNSIGNED_BYTE, moon->data);

	return TRUE;
}


void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks)
{
	GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat)iStacks;
	GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat)iSlices;
	GLfloat ds = 1.0f / (GLfloat)iSlices;
	GLfloat dt = 1.0f / (GLfloat)iStacks;
	GLfloat t = 1.0f;
	GLfloat s = 0.0f;
	GLint i, j;

	for (i = 0; i < iStacks; i++)
	{
		GLfloat rho = (GLfloat)i * drho;
		GLfloat srho = (GLfloat)(sin(rho));
		GLfloat crho = (GLfloat)(cos(rho));
		GLfloat srhodrho = (GLfloat)(sin(rho + drho));
		GLfloat crhodrho = (GLfloat)(cos(rho + drho));

		glBegin(GL_TRIANGLE_STRIP);
		s = 0.0f;
		for (j = 0; j <= iSlices; j++)
		{
			GLfloat theta = (j == iSlices) ? 0.0f : j * dtheta;
			GLfloat stheta = (GLfloat)(-sin(theta));
			GLfloat ctheta = (GLfloat)(cos(theta));

			GLfloat x = stheta * srho;
			GLfloat y = ctheta * srho;
			GLfloat z = crho;

			glTexCoord2f(s, t);
			glNormal3f(x, y, z);
			glVertex3f(x * fRadius, y * fRadius, z * fRadius);

			x = stheta * srhodrho;
			y = ctheta * srhodrho;
			z = crhodrho;
			glTexCoord2f(s, t - dt);
			s += ds;
			glNormal3f(x, y, z);
			glVertex3f(x * fRadius, y * fRadius, z * fRadius);
		}
		glEnd();

		t -= dt;
	}
}

void drawCircle(GLfloat r)
{
	int j = 0;
	glBegin(GL_POINTS);
	for (j = 0; j < 100; j++)
	{
		GLfloat theta = j * 2 * PI / 100;
		glVertex3f(r * cos(theta), 0.0f, r * sin(theta));
	}
	glEnd();
}