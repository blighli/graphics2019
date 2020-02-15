#include "gameover.h"
#include "main.h"
//游戏结束
void gameOver(){
	int pacmanX = (int)(1.5 + xIncrement);
	int pacmanY = (int)(1.5 + yIncrement);
	int monster1X = (int)(monster1[0]);
	int monster1Y = (int)(monster1[1]);
	int monster2X = (int)(monster2[0]);
	int monster2Y = (int)(monster2[1]);
	int monster3X = (int)(monster3[0]);
	int monster3Y = (int)(monster3[1]);
	int monster4X = (int)(monster4[0]);
	int monster4Y = (int)(monster4[1]);
	if (pacmanX == monster1X && pacmanY == monster1Y){
		over = true;
	}
	if (pacmanX == monster2X && pacmanY == monster2Y){
		over = true;
	}
	if (pacmanX == monster3X && pacmanY == monster3Y){
		over = true;
	}
	if (pacmanX == monster4X && pacmanY == monster4Y){
		over = true;
	}
	if (points == 106){
		over = true;
	}
}