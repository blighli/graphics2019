#pragma once

#define BITMAP_ID 0x4D42
#define PI 3.1415926

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <freeglut.h>

//纹理数据结构
typedef struct {
	int width;				
	int height;				
	unsigned int texID;		//纹理对象 ID
	unsigned char * data;	//实际纹理数据
}texture;

//调节运转速度
static float year = 0, month = 0, day = 0, angle = 15;
static bool first = false;

//调入位图作为纹理数据
unsigned char * LoadBmpFile(char * filename, BITMAPINFOHEADER * bmpInfoHeader);

//调入纹理文件
texture * LoadTexFile(char * filename);

//初始化所有纹理数据和属性
BOOL LoadAllTextures();
void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks);

void drawCircle(GLfloat r);