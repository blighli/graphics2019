#define _CRT_SECURE_NO_WARNINGS  

#include <stdio.h>  
#include <string.h>  
#include<time.h>
#include <stdlib.h>
#include <windows.h>
//#include "GL/GLUT.H"  
#include"game.h"  


GLuint texture[5];

//视区  
float whRatio;
int wHeight = 0;
int wWidth = 0;

//控制飞机生成（最好能改成类）

float move_x[6][3];//移动飞机的x坐标
float move_y[6] = { 2.5f,2.0f,1.5f,1.0f,0.0f,-0.5f };//移动飞机的y坐标
bool isShip[6][3];//该位置是否存在飞机
int collisionTimes[6][3] = { 0 };//碰撞次数
bool isAttack[6][3][3];//该飞机是否发子弹
float attack_x[6][3][3];//子弹的x坐标
float attack_y[6][3][3];//子弹的y坐标
bool isSuccess[6][3][3];//是否击中飞船
bool isLeft[6][3];//飞机是否从左边产生
int shipType[6][3];//飞机的形状

//白线位置
float pos[25] = { 0.0f };//25条白线的x坐标
float pos_y[25][3];//25条白线的y坐标（3种状态）
float pos_x = 0.0f; //飞船的x坐标
int status = 0;

//计时
int count = 0;
int count2 = 0;
int count3 = 0;

//视点  
float center[] = { 0, 0, 0 };
float eye[] = { 0, 0, 5 };

//分数
int score = 0;

//生命值
int life = 300;


void drawRect(GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]   

	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = { { x1,y1 },{ x2,y1 },{ x2,y2 },{ x1,y2 } };
	int dir[4][2] = { { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } };
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++) {
		glTexCoord2iv(dir[i]);
		glVertex2fv(point[i]);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

inline bool collisionTest(float y1, float y2, float y3, float x1, float x2, float x3)
{
	if (x3 > x1 && x3 < x2 && y1 < y3 && y3 < y2) return true;
	else return false;
}

inline bool collisionTest(float y1, float y2, float y3, float y4, float x1, float x2, float x3)
{
	if (x3 > x1 && x3 < x2 && (y1<y3 && y1 > y4 || y2 < y3 && y2 > y4)) return true;
	else return false;
}


//绘制飞船发出的白线
void drawLine()
{
	glTranslatef(0.0, 2.9f, 0.0f);
	for (int i = 0; i < 25; i++) {
		glBegin(GL_LINES);
		glVertex3f(pos[i], -0.05f, 1.0f);
		glVertex3f(pos[i], 0.05f, 1.0f);
		glEnd();
		glTranslatef(0.0, -0.2f, 0.0f);
	}
}

//控制从右边产生的飞机的移动
void shipMoveRight(int i, int j)
{
	glPushMatrix();
	glTranslatef(move_x[i][j], move_y[i], 1.0f);
	glScalef(0.6, 0.5, 1);
	move_x[i][j] -= 0.0005f;
	if (move_x[i][j] < -3.5f) {
		isShip[i][j] = false;
		collisionTimes[i][j] = 0;
	}
	if (collisionTimes[i][j] >= 5) {
		glColor3f(1, 0, 0);
	}
	drawRect(texture[shipType[i][j]]);
	glColor3f(1, 1, 1);
	glPopMatrix();

}

//控制从左边产生的飞机的移动
void shipMoveLeft(int i, int j)
{
	glPushMatrix();
	glTranslatef(move_x[i][j], move_y[i], 1.0f);
	glScalef(0.6, 0.5, 1);
	move_x[i][j] += 0.0005f;
	if (move_x[i][j] > 3.5f) {
		isShip[i][j] = false;
		collisionTimes[i][j] = 0;
	}
	if (collisionTimes[i][j] >= 5) {
		glColor3f(1, 0, 0);
	}
	drawRect(texture[shipType[i][j]]);
	glColor3f(1, 1, 1);
	glPopMatrix();

}

void printData()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256]; //字符串缓冲区  

	frame++;
	int life2 = 10;
	int score2 = 10;
	time = glutGet(GLUT_ELAPSED_TIME);
	//返回两次调用glutGet(GLUT_ELAPSED_TIME)的时间间隔,单位为毫秒  
	if (time - timebase > 1000) { //时间间隔差大于1000ms时  
		life2 = life;
		score2 = score;
		sprintf(buffer, "Score : %d   Life : %d",
			score2, life2); //写入buffer中  
		timebase = time; //上一次的时间间隔  
		frame = 0;
	}


	char *c;
	glDisable(GL_DEPTH_TEST);     // 禁止深度测试  
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵  
	glPushMatrix();               // 保存原矩阵  
	glLoadIdentity();             // 装入单位矩阵  
	glOrtho(0, 480, 0, 480, -1, 1);    // 位置正投影  
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵  
	glPushMatrix();               // 保存原矩阵  
	glLoadIdentity();             // 装入单位矩阵  
	glRasterPos2f(10, 10);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); //绘制字符  
	}
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵  
	glPopMatrix();                // 重置为原保存矩阵  
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵  
	glPopMatrix();                // 重置为原保存矩阵  
	glEnable(GL_DEPTH_TEST);      // 开启深度测试  

	//弹出对话框提示游戏结束
	if (life2 == 0) {
		char result[30];
		char str[30];
		strcpy(result, "游戏结束，您的得分是：");
		_itoa(score2, str, 10);
		strcat(result, str);
		MessageBox(NULL, TEXT(result), TEXT("注意"), MB_ICONINFORMATION);
		exit(0);
	}
}

void drawScene()
{

	//绘制星空背景
	glPushMatrix();
	glScalef(8, 6, 1);
	drawRect(texture[0]);
	glPopMatrix();

	//绘制白线
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	if (count >= 50 && count < 100) {
		glTranslatef(0.0f, 0.066f, 0.0f);
		status = 1;
	}
	else if (count >= 100 && count < 150) {
		glTranslatef(0.0f, 0.132f, 0.0f);
		status = 2;
	}
	else status = 0;
	drawLine();
	glPopMatrix();

	//设置深度缓存为只读
	glDepthMask(GL_FALSE);

	//绘制移动的飞机

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 3; j++) {
			if (isShip[i][j]) {
				//判断飞机从哪个方向产生
				if (!isLeft[i][j])shipMoveRight(i, j);
				else shipMoveLeft(i, j);
				if (isShip[i][j] == false)continue;
				int t = status;
				for (int k = 0; k < 25; k++) {
					//判断白线和飞机是否发生碰撞
					if (collisionTest(pos_y[k][t] - 0.05f, pos_y[k][t] + 0.05f, move_y[j] - 0.3f, move_x[i][j] - 0.1f, move_x[i][j] + 0.1f, pos[k])) {
						collisionTimes[i][j]++;
						if (shipType[i][j] == 3 && collisionTimes[i][j] > 15) {
							isShip[i][j] = false;
							score += 10;
							printData();
							collisionTimes[i][j] = 0;
						}
						else if (shipType[i][j] == 1 && collisionTimes[i][j] > 100) {
							isShip[i][j] = false;
							score += 15;
							printData();
							collisionTimes[i][j] = 0;
						}
					}
				}
			}
			for (int k = 0; k < 3; k++) {
				//绘制飞机的子弹
				if (isAttack[i][j][k]) {
					glPushMatrix();
					glColor3f(1, 0, 0);
					glTranslatef(attack_x[i][j][k], attack_y[i][j][k], 0.0f);
					glBegin(GL_LINES);
					glVertex3f(0.0f, -0.03f, 1.0f);
					glVertex3f(0.0f, 0.03f, 1.0f);
					glEnd();
					glPopMatrix();
					glColor3f(1, 1, 1);
					//判断子弹是否击中飞船
					if (!isSuccess[i][j][k] && collisionTest(attack_y[i][j][k] - 0.03f, attack_y[i][j][k] + 0.03f, -1.95f, -2.65f, pos_x - 0.38f, pos_x + 0.38f, attack_x[i][j][k])) {
						isSuccess[i][j][k] = true;
						life -= 10;
						printData();

					}
					//移动子弹位置
					if (count % 50 == 0) {
						attack_y[i][j][k] -= 0.05f;
					}
					//子弹消失
					if (attack_y[i][j][k] < -2.5f) {
						isAttack[i][j][k] = false;
						isSuccess[i][j][k] = false;
					}
				}
			}
		}
	}

	//绘制飞船
	glPushMatrix();
	glTranslatef(pos_x, -2.3f, 1.0f);
	glScalef(0.8, 0.7, 0);
	drawRect(texture[2]);
	glPopMatrix();

	//取消设置深度缓存为只读
	glDepthMask(GL_TRUE);


	//控制随机生成飞机
	if (count >= 150) {
		count = 0;
		for (int i = 1; i < 25; i++) {
			pos[i - 1] = pos[i];
		}
		pos[24] = pos_x;
		count2++;

		if (count2 == 5 || count2 == 10) {
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 3; j++) {
					if (isShip[i][j]) {
						for (int k = 0; k < 3; k++) {
							//控制飞机开始发射子弹
							if (isAttack[i][j][k] == false) {
								isAttack[i][j][k] = true;
								attack_x[i][j][k] = move_x[i][j];
								attack_y[i][j][k] = move_y[i];
								break;
							}
						}
					}
				}
			}
		}
		if (count2 == 10) {
			count2 = 0;
			int num = rand() % 6;
			for (int i = 0; i < 3; i++) {
				//找到可以生成飞机的位置
				if (isShip[num][i] == false) {
					bool flag = false;
					//保证飞机从左右发射时不相撞，在同一行已经有飞机的时候，和已有飞机方向相同
					for (int j = 0; j < 3; j++) {
						if (isShip[num][j] == true) {
							flag = true;
							isLeft[num][i] = isLeft[num][j];
							break;
						}
					}
					//同一行不存在飞机的时候，随机生成飞机产生方向
					if (!flag) {
						isLeft[num][i] = rand() % 2;

					}
					isShip[num][i] = true;
					//随机生成飞机种类
					int random = rand() % 3;
					if (random == 0) {
						shipType[num][i] = 1;
					}
					else shipType[num][i] = 3;
					//初始化操作
					collisionTimes[num][i] = 0;
					if (isLeft[num][i])move_x[num][i] = -3.0f;
					else move_x[num][i] = 3.0f;
					break;
				}
			}
		}
	}
	else count++;
}

void updateView(int height, int width)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);//设置矩阵模式为投影       
	glLoadIdentity();   //初始化矩阵为单位矩阵          
	whRatio = (GLfloat)width / (GLfloat)height; //设置显示比例     
	glOrtho(-3, 3, -3, 3, -100, 100); //正投影
	glMatrixMode(GL_MODELVIEW);  //设置矩阵模式为模型    
}

void reshape(int width, int height)
{
	if (height == 0)      //如果高度为0      
	{
		height = 1;   //让高度为1（避免出现分母为0的现象）      
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth); //更新视角      
}


void idle()
{
	glutPostRedisplay();
}



void init()
{
	srand(unsigned(time(NULL)));
	glEnable(GL_DEPTH_TEST);//开启深度测试   
	glEnable(GL_ALPHA_TEST);//开启alpha测试
	glAlphaFunc(GL_GREATER, 0.1);
	glEnable(GL_LIGHTING);  //开启光照模式   

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	memset(pos, -10.0f, sizeof(pos));
	glGenTextures(4, texture);
	loadTex(0, "1.bmp", texture);
	loadTex_alpha(1, "ship3.bmp", texture);
	loadTex_alpha(2, "ship.bmp", texture);
	loadTex_alpha(3, "ship2.bmp", texture);

	pos_y[0][0] = 2.9f;
	pos_y[0][1] = 2.9f + 0.66f;
	pos_y[0][2] = 2.9f + 0.132f;

	for (int i = 0; i < 3; i++) {
		for (int j = 1; j < 25; j++) {
			pos_y[j][i] = pos_y[j - 1][i] - 0.2f;
		}
	}

	memset(isShip, false, sizeof(isShip));
	memset(isAttack, false, sizeof(isAttack));

	isShip[0][0] = true;
	shipType[0][0] = 3;
	move_x[0][0] = 3.0f;


}

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 'a': {
		pos_x -= 0.05f;
		if (pos_x < -2.90f)pos_x = -2.90f;
		break;
	}
	case 'd': {
		pos_x += 0.05f;
		if (pos_x > 2.90f)pos_x = 2.90f;
		break;
	}
	}

	updateView(wHeight, wWidth); //更新视角  
}



void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色和深度缓存    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();   //初始化矩阵为单位矩阵      
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);                // 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上  
	glPolygonMode(GL_FRONT, GL_FILL);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	// 启用光照计算
	glEnable(GL_LIGHTING);
	// 指定环境光强度（RGBA）
	GLfloat ambientLight[] = { 2.0f, 2.0f, 2.0f, 1.0f };

	// 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	// 启用颜色追踪
	glEnable(GL_COLOR_MATERIAL);
	// 设置多边形正面的环境光和散射光材料属性，追踪glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	drawScene();//绘制场景   
	printData();
	glutSwapBuffers();//交换缓冲区  
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);//对glut的初始化         
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//初始化显示模式:RGB颜色模型，深度测试，双缓冲           
	glutInitWindowSize(800, 600);//设置窗口大小         
	int windowHandle = glutCreateWindow("Simple GLUT App");//设置窗口标题           
	glutDisplayFunc(redraw); //注册绘制回调函数         
	glutReshapeFunc(reshape);   //注册重绘回调函数         
	glutKeyboardFunc(key); //注册按键回调函数
	glutIdleFunc(idle);//注册全局回调函数：空闲时调用       

	init();

	glutMainLoop();  // glut事件处理循环       
	return 0;
}
