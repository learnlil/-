#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<cmath>
#include<Windows.h>
#include<string>
#include<graphics.h>
#include<conio.h>
#include<vector>
#include "tools.h"
using namespace std;
#define OBSTACLE_COUNT 10
#define WIN_WIDTH 1012
#define WIN_HEIGHT 396
#define WIN_SCORE 100

class background
{
public:
	//背景图片
	IMAGE imgBgs[3];
	int bgX[3];//背景图片x坐标
	int bgSpeed[3] = { 2,4,8 };
	void updatebackground();
};
void background::updatebackground()
{
	//加载背景资源
	char name[64];
	for (int i = 0; i < 3; i++)
	{
		//"res/bg001.png"  "res/bg002.png" "res/bg003.png"
		sprintf(name, "res/bg%03d.png", i + 1);
		loadimage(&imgBgs[i], name);
		bgX[i] = 0;
	}
}
class Hero
{
public:

	IMAGE imgHeros[12];
	IMAGE imgHeroDown[12];
	int heroX;//玩家x坐标
	int heroY;//玩家y坐标
	int heroIndex;//玩家奔跑的图片的序号
	bool heroJump;//表示玩家正在跳跃
	bool heroDown;
	bool upDate;
	int heroJumpHeightMax;
	int heroJumpOff;
	int heroBlood;
	void heroXY();
};
void Hero::heroXY()
{
	//设置玩家的初始位置
	heroX = WIN_WIDTH * 0.5 - imgHeros[0].getwidth() * 0.5;
	heroY = 345 - imgHeros[0].getheight();
	heroIndex = 0;
	heroJump = false;
	heroJumpHeightMax = 345 - imgHeros[0].getheight() - 120;
	heroJumpOff = -10;
	upDate = true;
}

class score
{
public:

	int score;
	IMAGE imgSZ[10];
};

class update
{
public:
	bool upDate;//
};

typedef enum
{
	TORTOISE,//乌龟 0
	LION,//狮子 1
	HOOK1,//2
	HOOK2,//3
	HOOK3,//4
	HOOK4,//5
	OBSTACLE_TYPE_COUNT //6
} obstacle_type;

//vector< vector<int>> data;

//IMAGE obstracleImgs[3][12];
vector<vector<IMAGE>>obstacleImgs;//IMAGE obstacleImgs[][];存放所哟障碍物图片

class obstacle
{
public:
	obstacle_type type;//障碍物的类型
	int x, y;//障碍物坐标
	int imgIndex;//当前显示的图片de序号
	int speed;
	int power;//杀伤力
	bool exist;
	bool hited;//表示是否已经发生碰撞
	bool passed;//表示是否已经被通过

};
obstacle obstacles[OBSTACLE_COUNT];

int main()
{

}