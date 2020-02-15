#include "gameresult.h"
#include "main.h"
//游戏结果
void resultsDisplay(){
	if (points == 106){
		//胜利
		string message = "*************************************";
		string::iterator it = message.begin();
		glRasterPos2f(170, 250);
		while (it!=message.end())
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *it++);
		message = "CONGRATULATIONS, YOU WON! ";
		glColor3f(1, 1, 1);
		glRasterPos2f(200, 300);
		it = message.begin();
		while (it!=message.end())
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *it++);
		message = "*************************************";
		glRasterPos2f(170, 350);
		it = message.begin();
		while (it!=message.end())
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *it++);
		message = "To start or restart the game, press the space key.";
		glRasterPos2f(170, 550);
		it = message.begin();
		while (it!=message.end())
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *it++);
	}else {
		//Lost
		string message = "*************************************";
		string::iterator it = message.begin();
		glRasterPos2f(210, 250);
		while (it!=message.end())
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *it++);
		message = "SORRY, YOU LOST ... ";
		glColor3f(1, 1, 1);
		glRasterPos2f(250, 300);
		it = message.begin();
		while (it!=message.end())
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *it++);
		message = "*************************************";
		glRasterPos2f(210, 350);
		it = message.begin();
		while (it!=message.end())
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *it++);
		message = "You got: ";
		glRasterPos2f(260, 400);
		it = message.begin();
		while (it!=message.end())
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *it++);
		string result = to_string(points);
		message = (char*)result.c_str();
		glRasterPos2f(350, 400);
		it = message.begin();
		while (it!=message.end())
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *it++);
		message = " points!";
		glRasterPos2f(385, 400);
		it = message.begin();
		while (it!=message.end())
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *it++);
		message = "To start or restart the game, press the space key.";
		glRasterPos2f(170, 550);
		it = message.begin();
		while (it!=message.end())
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *it++);
	}
}
