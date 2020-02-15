#include "food.h"
#include "main.h"

//检查食物是否被吃
bool foodEaten(int x, int y, float pacmanX, float pacmanY){
	if (x >= pacmanX - 16.0 *cos(359 * M_PI / 180.0) && x <= pacmanX + 16.0*cos(359 * M_PI / 180.0)){
		if (y >= pacmanY - 16.0*cos(359 * M_PI / 180.0) && y <= pacmanY + 16.0*cos(359 * M_PI / 180.0)){
			return true;
		}
	}
	return false;
}

//画上食物
void drawFood(float pacmanX, float pacmanY){
	deque<float> temp;
	//检查食物是否没有被吃掉
	for (deque<float>::size_type i = 0; i < food.size(); i = i + 2){
		if (!foodEaten(food.at(i)*squareSize, food.at(i + 1)*squareSize, pacmanX, pacmanY)){
			temp.push_back(food.at(i));
			temp.push_back(food.at(i + 1));
		}
		else {
			points++;
		}
	}
	food.swap(temp);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glColor3f(1.0, 1.0, 1.0);
	//画上食物
	for (deque<float>::size_type j = 0; j < food.size(); j = j + 2){
		glVertex2f(food.at(j)*squareSize, food.at(j + 1)*squareSize);//画点
	}
	glEnd();
}