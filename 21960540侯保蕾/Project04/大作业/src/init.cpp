#include "init.h"
#include "main.h"
//初始化游戏
void init(void){
	//清除屏幕
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);//着色
	//重置按键
	for (int i = 0; i < 256; i++){
		keyStates[i] = false;
	}
	//用障碍物填充地图
	bitmap.push_back({ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true });
	bitmap.push_back({ true, false, false, false, false, false, false, false, false, false, false, false, false, false, true });
	bitmap.push_back({ true, false, true, true, true, true, false, true, true, false, true, true, true, false, true });
	bitmap.push_back({ true, false, false, false, false, true, false, true, false, false, false, false, true, false, true});
	bitmap.push_back({ true, false, true, true, false, false, false, false, false, true, true, false, false, false, true});
	bitmap.push_back({ true, false, false, true, true, false, true, true, true, true, false, false, true, false, true});
	bitmap.push_back({ true, true, false, false, false, false, true, false, true, true, false, true, true, false, true});
	bitmap.push_back({ true, true, true, true, true, false, false, false, true, false, false, false, false, false, true});
	bitmap.push_back({ true, true, false, false, false, false, true, false, true, true, false, true, true, false, true });
	bitmap.push_back({ true, false, false, true, true, false, true, true, true, true, false, false, true, false, true });
	bitmap.push_back({ true, false, true, true, false, false, false, false, false, true, true, false, false, false, true });
	bitmap.push_back({ true, false, false, false, false, true, false, true, false, false, false, false, true, false, true });
	bitmap.push_back({ true, false, true, true, true, true, false, true, true, false, true, true, true, false, true });
	bitmap.push_back({ true, false, false, false, false, false, false, false, false, false, false, false, false, false, true });
	bitmap.push_back({ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true });
}