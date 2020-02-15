#include "control.h"
#include "main.h"
//设置按键
void keyPressed(unsigned char key, int x, int y){
	keyStates[key] = true;
}

//释放按键
void keyUp(unsigned char key, int x, int y){
	keyStates[key] = false;
}

//重置所有元素并开始游戏
void resetGame(){
	over = false;
	xIncrement = 0;
	yIncrement = 0; 
	rotation = 0;
	monster1 = new float[3] {10.5, 8.5, 1.0};
	monster2 = new float[3] {13.5, 1.5, 2.0};
	monster3 = new float[3] {4.5, 6.5, 3.0};
	monster4 = new float[3] {2.5, 13.5, 4.0};
	points = 0;
	for (int i = 0; i < 256; i++){
		keyStates[i] = false;
	}
	food = { 1.5, 1.5, 1.5, 2.5, 1.5, 3.5, 1.5, 4.5, 1.5, 5.5, 1.5, 6.5, 1.5, 7.5, 1.5, 8.5, 1.5, 9.5, 1.5, 10.5, 1.5, 11.5, 1.5, 12.5, 1.5, 13.5, 2.5, 1.5, 2.5, 6.5, 2.5, 9.5, 2.5, 13.5, 3.5, 1.5, 3.5, 2.5, 3.5, 3.5, 3.5, 4.5, 3.5, 6.5, 3.5, 8.5, 3.5, 9.5, 3.5, 10.5, 3.5, 11.5, 3.5, 13.5, 4.5, 1.5, 4.5, 4.5, 4.5, 5.5, 4.5, 6.5, 4.5, 7.5, 4.5, 8.5, 4.5, 11.5, 4.5, 12.5, 4.5, 13.5, 5.5, 1.5, 5.5, 2.5, 5.5, 5.5, 5.5, 10.5, 5.5, 11.5, 5.5, 13.5, 6.5, 2.5, 6.5, 3.5, 6.5, 4.5, 6.5, 5.5, 6.5, 7.5, 6.5, 10.5, 6.5, 13.5, 7.5, 5.5, 7.5, 6.5, 7.5, 7.5, 7.5, 9.5, 7.5, 10.5, 7.5, 11.5, 7.5, 12.5, 7.5, 13.5, 8.5, 2.5, 8.5, 3.5, 8.5, 4.5, 8.5, 5.5, 8.5, 7.5, 8.5, 10.5, 8.5, 13.5, 9.5, 1.5, 9.5, 2.5, 9.5, 5.5, 9.5, 10.5, 9.5, 11.5, 9.5, 13.5, 10.5, 1.5, 10.5, 4.5, 10.5, 5.5, 10.5, 6.5, 10.5, 7.5, 10.5, 8.5, 10.5, 11.5, 10.5, 12.5, 10.5, 13.5, 11.5, 1.5, 11.5, 2.5, 11.5, 3.5, 11.5, 4.5, 11.5, 5.5, 11.5, 6.5, 11.5, 8.5, 11.5, 9.5, 11.5, 10.5, 11.5, 11.5, 11.5, 13.5, 12.5, 1.5, 12.5, 6.5, 12.5, 9.5, 12.5, 13.5, 13.5, 1.5, 13.5, 2.5, 13.5, 3.5, 13.5, 4.5, 13.5, 5.5, 13.5, 6.5, 13.5, 7.5, 13.5, 8.5, 13.5, 9.5, 13.5, 10.5, 13.5, 11.5, 13.5, 12.5, 13.5, 13.5 };
}

//控制吃豆人移动
void keyOperations(){
	//获得当前位置
	float  x = (1.5 + xIncrement) * squareSize;
	float y = (1.5 + yIncrement) * squareSize;
	//更新按键
	if (keyStates['a']){
		x -= 2;
		int x1Quadrant = (int)((x - 16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		if (!bitmap.at(x1Quadrant).at((int)y/squareSize)){
			xIncrement -= 2 / squareSize;
			rotation = 2;
		}
	}
	if (keyStates['d']){
		x += 2;
		int x2Quadrant = (int)((x + 16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		if (!bitmap.at(x2Quadrant).at((int)y / squareSize)){
			xIncrement += 2 / squareSize;
			rotation = 0;
		}
	}
	if (keyStates['w']){
		y -= 2;
		int y1Quadrant = (int)((y - 16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		if (!bitmap.at((int)x/squareSize).at(y1Quadrant)){
			yIncrement -= 2 / squareSize;
			rotation = 3;
		}
	}
	if (keyStates['s']){
		y += 2;
		int y2Quadrant = (int)((y + 16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		if (!bitmap.at((int)x / squareSize).at(y2Quadrant)){
			yIncrement += 2 / squareSize;
			rotation = 1;
		}
	}
	if (keyStates[' ']){
		if (!replay && over){
			resetGame();
			replay = true;
		}
		else if (replay && over){
			replay = false;
		}
	}
}