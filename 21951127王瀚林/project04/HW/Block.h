#pragma once
#include <iostream>

class Block
{
public:
	float zPos = -2.0f; //平台位置
	float Width = 1.0; //平台大小
	float V = 0.03; //平台移动速度

	void SetBlock(float z, float width); //放置平台 参数:位置，平台大小

	void MoveBlock(); //平台位置刷新

	bool isCoverd(float tolerance); //平台检测 参数:判定宽容度
};

