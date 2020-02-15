#include "main.h"
#include "control.h"
#include "food.h"
#include "gameresult.h"
#include "gameover.h"
#include "gamestart.h"
#include "init.h"
#include "monster.h"
#include "createpacman.h"
#include "laberynth.h"

using namespace std;

bool replay = false; //检查是否启动游戏
bool over = true; //检查游戏是否结束
float squareSize = 50.0; //一个单元大小
float xIncrement = 0; // x坐标
float yIncrement = 0; // y坐标
int rotation = 0; // 定位
float* monster1 = new float[3] {10.5, 8.5, 1.0}; //第一个怪物的坐标和方向
float* monster2 = new float[3] {13.5, 1.5, 2.0}; //第二个怪物的坐标和方向
float* monster3 = new float[3] {4.5, 6.5, 3.0}; //第三个怪物的坐标和方向
float* monster4 = new float[3] {2.5, 13.5, 4.0}; //第四个怪物的坐标和方向
vector<int> border = { 0, 0, 15, 1, 15, 15, 14, 1, 0, 14, 15, 15, 1, 14, 0, 0 }; //墙坐标

//障碍物坐标 (为了清晰分为三个)
vector<int> obstaclesTop = { 2, 2, 3, 6, 3, 6, 4, 5, 4, 2, 5, 4, 5, 3, 6, 5, 6, 1, 9, 2, 7, 2, 8, 5, 9, 5, 10, 3, 10, 4, 11, 2, 11, 5, 12, 6, 12, 6, 13, 2 };
vector<int> obstaclesMiddle = { 2, 9, 3, 7, 3, 7, 4, 8, 4, 9, 5, 11, 5, 6, 6, 10, 6, 10, 7, 8, 7, 8, 8, 9, 6, 7, 7, 6, 8, 6, 9, 7, 10, 6, 9, 10, 9, 10, 8, 8, 11, 9, 10, 11, 11, 8, 12, 7, 12, 7, 13, 9 };
vector<int> obstaclesBottom = { 2, 10, 3, 13, 3, 13, 4, 12, 5, 12, 6, 13, 6, 13, 7, 11, 8, 11, 9, 13, 9, 13, 10, 12, 11, 12, 12, 13, 12, 13, 13, 10 };
deque<float> food = { 1.5, 1.5, 1.5, 2.5, 1.5, 3.5, 1.5, 4.5, 1.5, 5.5, 1.5, 6.5, 1.5, 7.5, 1.5, 8.5, 1.5, 9.5, 1.5, 10.5, 1.5, 11.5, 1.5, 12.5, 1.5, 13.5, 2.5, 1.5, 2.5, 6.5, 2.5, 9.5, 2.5, 13.5, 3.5, 1.5, 3.5, 2.5, 3.5, 3.5, 3.5, 4.5, 3.5, 6.5, 3.5, 8.5, 3.5, 9.5, 3.5, 10.5, 3.5, 11.5, 3.5, 13.5, 4.5, 1.5, 4.5, 4.5, 4.5, 5.5, 4.5, 6.5, 4.5, 7.5, 4.5, 8.5, 4.5, 11.5, 4.5, 12.5, 4.5, 13.5, 5.5, 1.5, 5.5, 2.5, 5.5, 5.5, 5.5, 10.5, 5.5, 11.5, 5.5, 13.5, 6.5, 2.5, 6.5, 3.5, 6.5, 4.5, 6.5, 5.5, 6.5, 7.5, 6.5, 10.5, 6.5, 13.5, 7.5, 5.5, 7.5, 6.5, 7.5, 7.5, 7.5, 9.5, 7.5, 10.5, 7.5, 11.5, 7.5, 12.5, 7.5, 13.5, 8.5, 2.5, 8.5, 3.5, 8.5, 4.5, 8.5, 5.5, 8.5, 7.5, 8.5, 10.5, 8.5, 13.5, 9.5, 1.5, 9.5, 2.5, 9.5, 5.5, 9.5, 10.5, 9.5, 11.5, 9.5, 13.5, 10.5, 1.5, 10.5, 4.5, 10.5, 5.5, 10.5, 6.5, 10.5, 7.5, 10.5, 8.5, 10.5, 11.5, 10.5, 12.5, 10.5, 13.5, 11.5, 1.5, 11.5, 2.5, 11.5, 3.5, 11.5, 4.5, 11.5, 5.5, 11.5, 6.5, 11.5, 8.5, 11.5, 9.5, 11.5, 10.5, 11.5, 11.5, 11.5, 13.5, 12.5, 1.5, 12.5, 6.5, 12.5, 9.5, 12.5, 13.5, 13.5, 1.5, 13.5, 2.5, 13.5, 3.5, 13.5, 4.5, 13.5, 5.5, 13.5, 6.5, 13.5, 7.5, 13.5, 8.5, 13.5, 9.5, 13.5, 10.5, 13.5, 11.5, 13.5, 12.5, 13.5, 13.5 };
vector<vector<bool>> bitmap; // 2d图像，可移动区域
bool* keyStates = new bool[256]; // 按键记录
int points = 0; // 得分

//主函数
int main(int argc, char** argv){
	//初始化并创建屏幕
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//显示方式(双缓重区，深度缓冲)
	glutInitWindowSize(750, 750);//窗口大小
	glutInitWindowPosition(500, 50);//窗口起始位置
	glutCreateWindow("Pacman Homework Demo");

	//定义所有控制功能
	glutDisplayFunc(display);//窗口更新
	glutReshapeFunc(reshape);//窗口变化
	glutIdleFunc(display);//循环画图
	glutKeyboardFunc(keyPressed);//按键盘操作
	glutKeyboardUpFunc(keyUp);//

	//运行游戏
	init();
	glutMainLoop();
	return 0;
}
