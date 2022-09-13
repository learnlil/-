/*
天天酷跑开发日志
1.创建项目
2.导入素材
3.创建游戏界面
    实际开发流程
	用户界面入手
	选择图形库
	1)创建游戏窗口
	2)设计游戏背景
	  a.3重背景不同的速度同时移动
	  b.循环滚动背景的实现
	 3）实现游戏背景
	   a.加载背景资源
	   b.渲染(背景知识：坐标)
	   遇到问题：背景图片的png格式图片出现黑色
4.实现玩家的奔跑
5.实现玩家的跳跃
6.实现随机小乌龟
7.创建障碍物结构体数据类型
8.使用障碍物结构体后重新初始化
9.封装后多个障碍物的显示
10.实现玩家的下蹲技能
11.实现“柱子”障碍物

C++  算法  数据库 Windows（win32+mfc) Qt linux服务器
*/

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

//背景图片
IMAGE imgBgs[3];
int bgX[3];//背景图片x坐标
int bgSpeed[3] = { 2,4,8 };

IMAGE imgHeros[12];
int heroX;//玩家x坐标
int heroY;//玩家y坐标
int heroIndex;//玩家奔跑的图片的序号
bool heroJump;//表示玩家正在跳跃
bool heroDown;
int heroJumpHeightMax;
int heroJumpOff;
bool upDate;//
int heroBlood;
int score;
IMAGE imgSZ[10];
//下蹲
IMAGE imgHeroDown[12];

//IMAGE imgTortoise;//小乌龟
//int torToiseX;//小乌龟水平坐标
//int torToiseY;//小乌龟数值坐标
//bool torToiseExist;//当前窗口是否存在小乌龟
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

//游戏初始化
void init()
{
	//创建游戏窗口
	initgraph(WIN_WIDTH,WIN_HEIGHT);

	

	//加载背景资源
	char name[64];
	for (int i = 0; i < 3; i++)
	{
		//"res/bg001.png"  "res/bg002.png" "res/bg003.png"
		sprintf(name, "res/bg%03d.png", i + 1);
		loadimage(&imgBgs[i],name);
		bgX[i] = 0;
	}

	//加载人物奔跑的图片针素材
	for (int i = 0; i < 12; i++)
	{//"res/hero1.png".....
		sprintf(name, "res/hero%d.png", i + 1);
		loadimage(&imgHeros[i], name);
	}
	//设置玩家的初始位置
	heroX = WIN_WIDTH * 0.5 - imgHeros[0].getwidth() * 0.5;
	heroY = 345 - imgHeros[0].getheight();
	heroIndex = 0;
	heroJump = false;
	heroJumpHeightMax = 345 - imgHeros[0].getheight() - 120;
	heroJumpOff = -10;
	upDate = true;

	//加载小乌龟素材
	//loadimage(&imgTortoise, "res/t1.png");//此处需要优化
	//torToiseExist = false;
	IMAGE imgTort;
	loadimage(&imgTort, "res/t1.png");
	vector<IMAGE> imgTortArray;
	for (int i = 0; i < 7; i++)
	{
		sprintf(name, "res/t%d.png", i + 1);
		loadimage(&imgTort, name);
		imgTortArray.push_back(imgTort);

	}
	obstacleImgs.push_back(imgTortArray);

	//加载狮子
	IMAGE imgLion;
	vector<IMAGE> imgLionArray;
	for (int i = 0; i < 6; i++)
	{
		sprintf(name, "res/p%d.png", i + 1);
		loadimage(&imgLion, name);
		imgLionArray.push_back(imgLion);
	}
	obstacleImgs.push_back(imgLionArray);
	//初始化障碍物池
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		obstacles[i].exist = false;

	}
	
	//加载下蹲素材
	//loadimage(&imgHeroDown[0], "res/d1.png");
	//loadimage(&imgHeroDown[0], "res/d2.png");
	
	for (int i = 0; i < 12; i++)//下蹲帧素材
	{//"res/hero1.png".....
		sprintf(name, "res/bark/g%02d.png", i + 1);
		loadimage(&imgHeroDown[i], name);
	}
	heroDown = false;
	//柱子
	IMAGE imgH;
	
	for (int i = 0; i < 4; i++)
	{
		vector<IMAGE> imgHookArray;
		sprintf(name, "res/h%d.png", i + 1);
		loadimage(&imgH, name,63,260,true);
		imgHookArray.push_back(imgH);
		obstacleImgs.push_back(imgHookArray);
	}
	heroBlood = 100;
	//预加载音效
	preLoadSound("res/hit.mp3");

	mciSendString("play res/bg.mp3", 0, 0, 0);
	score = 0;
	//加载数字图片
	for(int i = 0; i < 10; i++)
	{
		sprintf(name, "res/sz/%d.png", i);
		loadimage(&imgSZ[i], name);
	}
}


void createObstacle()
{
	int i=0;
	for (i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist == false)
			break;
	}
	if (i >= OBSTACLE_COUNT)
	{
		return;
	}
	obstacles[i].exist = true;
	obstacles[i].hited = false;
	obstacles[i].imgIndex = 0;
	//obstacles[i].type =(obstacle_type)(rand() % OBSTACLE_TYPE_COUNT);
	obstacles[i].type = (obstacle_type)(rand() % 3);
	if (obstacles[i].type == HOOK1)
	{
		obstacles[i].type= (obstacle_type)((int)obstacles[i].type+rand() % 4);
	}
	obstacles[i].x = WIN_WIDTH;
	obstacles[i].y = 350 - obstacleImgs[obstacles[i].type][0].getheight();
	if (obstacles[i].type == TORTOISE)
	{
		obstacles[i].speed = 4;//速度
		obstacles[i].power = 5;//伤害值
		
	}
	else if (obstacles[i].type == LION)
	{
		obstacles[i].speed = 6;
		obstacles[i].power = 10;
	}
	else if (obstacles[i].type >= HOOK1&&obstacles[i].type<=HOOK4)
	{
		obstacles[i].speed = 4;
		obstacles[i].power = 20;
		obstacles[i].y = 0;
	}
	obstacles[i].passed = false;
}
void checkHit()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist&&obstacles[i].hited==false)
		{
			int a1x, a1y, a2x, a2y;
			int off = 30;
			if (!heroDown)//非下蹲（奔跑跳跃
			{
				a1x = heroX + off;
				a1y = heroY + off;
				a2x = heroX +imgHeros[heroIndex].getwidth() - off;
				a2y = heroY +imgHeros[heroIndex].getheight() - off;
			}
			else
			{
				a1x = heroX + off;
				a1y = 345-imgHeroDown[heroIndex].getheight()+off*2;
				a2x = heroX + imgHeroDown[heroIndex].getwidth() - off;
				a2y = 345+off*2;
			}
			IMAGE img = obstacleImgs[obstacles[i].type][obstacles[i].imgIndex];
			int b1x = obstacles[i].x + off/3;
			int b1y = obstacles[i].y + off/3;
			int b2x = obstacles[i].x +img.getwidth() - off/3;
			int b2y = obstacles[i].y +img.getheight() - off/3;

			if (rectIntersect(a1x,a1y,a2x,a2y,b1x,b1y,b2x,b2y))
			{
				heroBlood -= obstacles[i].power;
				//cout << "血量剩余" << heroBlood << endl;
				playSound("res/hit.mp3");
				obstacles[i].hited = true;
			}
		}
	}
}
void jump()
{
	heroJump = true;
	upDate = true;
	heroIndex = 0;//修改
}
void down()
{
	heroDown = true;
	upDate = true;
	heroIndex = 0;
}
//处理用户按键
void keyEvent()
{
	if (_kbhit())//如果有按键按下，返回true
	{
		char ch;
		ch = _getch();//getch()不需要按下回车即可直接读取
		if (ch == ' ')
		{
			jump();
		}
		else if (ch == 's')
		{
			down();

		}
	}

}
void fly()
{
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgSpeed[i];
		if (bgX[i] < -WIN_WIDTH)
		{
			bgX[i] = 0;
		}
	}
	
	//实现跳跃
	if (heroJump)
	{
		if (heroY < heroJumpHeightMax)
		{
			heroJumpOff = 10;
		}
		heroY += heroJumpOff;
		if (heroY > 345 - imgHeros[0].getheight())
		{
			heroJump = false;
			heroJumpOff = -10;
		}
	}
	else if (heroDown)
	{
		
		static int count = 0;
		int delays[12] = {1,2,1,2,1,2,1,2,1,2,1,2,};
		count++;
		if (count >= delays[heroIndex])
		{
			count = 0;
			heroIndex++;
			if (heroIndex >= 12)
			{
				heroIndex = 0;
				heroDown = false;
			}
		}
	}
	else//不跳跃
	{
		heroIndex = (heroIndex + 1) % 12;
	}

	//创建障碍物
	static int frameCount = 0;
	static int enemyFre = 50;
	frameCount++;
	if (frameCount > enemyFre)
	{
		frameCount = 0; 
		//if (!torToiseExist)
		//{
		//	torToiseExist = true;
		//	torToiseX = WIN_WIDTH;
		//	torToiseY = 345 - imgTortoise.getheight() + 5;
		//
		//	enemyFre =rand() % 300;
		//}
		enemyFre =30 + rand() % 50;//50~99
		createObstacle();
	}
	//if (torToiseExist)
	//{
	//	torToiseX -= bgSpeed[2];
	//	if (torToiseX < -imgTortoise.getwidth())
	//	{
	//		torToiseExist = false;
	//	}
	//}
	
	//更新所有障碍物坐标
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist)
		{
			obstacles[i].x -= obstacles[i].speed+bgSpeed[2];
			if (obstacles[i].x < -obstacleImgs[obstacles[i].type][0].getwidth() * 2)
			{
				obstacles[i].exist = false;
				
			}
			int len =(int) obstacleImgs[obstacles[i].type].size();
			obstacles[i].imgIndex = (obstacles[i].imgIndex + 1) % len;

		}
	}
	//玩家和障碍物的碰撞检测
	checkHit();
}
//渲染"游戏背景"
void updateBg()
{

	putimagePNG2(bgX[0], 0, &imgBgs[0]);
	putimagePNG2(bgX[1], 119, &imgBgs[1]);
	putimagePNG2(bgX[2], 330, &imgBgs[2]);
}

//人物动作
void updateHero()
{
	if (!heroDown)
	{
		putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);
	}
	else
	{
		int y = 375 - imgHeroDown[heroIndex].getheight();
			putimagePNG2(heroX, y, &imgHeroDown[heroIndex]);
	}
}

void updateEnemy()
{
	//渲染小乌龟
	//if (torToiseExist)
	//{
	//	putimagePNG2(torToiseX, torToiseY,WIN_WIDTH,&imgTortoise);
	//}
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist)
		{
			putimagePNG2(obstacles[i].x,obstacles[i].y,WIN_WIDTH,
				&obstacleImgs[obstacles[i].type][obstacles[i].imgIndex]);
		}
	}
}
void updateBloodBar()
{
	drawBloodBar(10, 10, 200, 10, 2, BLUE, DARKGRAY, RED,(float)(heroBlood / 100.0));
}
void checkOver()
{
	if (heroBlood <= 0)
	{
		loadimage(0, "res/over.png");
		FlushBatchDraw();//绘制未完成的图片easyx图形库中的
		mciSendString("stop res/bg.mp3", 0, 0, 0);
		system("pause");
		//暂停之后开始下一局
		heroBlood = 100;
		score = 0;
		mciSendString("play res/bg.mp3", 0, 0, 0);
	}
}
void checkScore()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist&&obstacles[i].passed==false
			&&obstacles[i].hited==false
			&&obstacles[i].x+obstacleImgs[obstacles[i].type][0].getwidth()<heroX)
		{
			score++;
			obstacles[i].passed = true;
			cout << "score: " << score << endl;
		}
	}
}
void updateScore()
{
	//50->"50"
	char str[8];
	sprintf(str, "%d", score);
	int x = 20;
	int y = 25;
	for (int i = 0; str[i]; i++)
	{
		int sz=str[i] - '0';
		putimagePNG2(x, y, &imgSZ[sz]);
		x += imgSZ[sz].getwidth() + 5;

	}
}
void checkWin()
{
	if (score >= WIN_SCORE)
	{
		FlushBatchDraw();
		mciSendString("play res/win.mp3", 0, 0, 0);
		Sleep(1000);
		loadimage(0, "res/win.png");
		FlushBatchDraw();
		mciSendString("stop res/bg.mp3", 0, 0, 0);
		system("pause");

		heroBlood = 100;
		score = 0;
		mciSendString("play res/bg.mp3 repeat", 0, 0, 0);
	}
}
int getDelay() {
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = (int)currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}
int main()
{
	init();
	//显示初始画面
	loadimage(0, "res/over.png");
	system("pause");

	int timer=0;
	while (1)
	{
		
		keyEvent();
		timer+=getDelay();
		if (timer > 30)
		{
			timer = 0;
			upDate = true;
		}
		//Sleep(30);
		if (upDate)
		{
			upDate = false;
			BeginBatchDraw();
			updateBg();
			//putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);
			updateHero();
			updateEnemy();
			updateBloodBar();
			updateScore();
			checkWin();
			EndBatchDraw();

			checkOver();
			checkScore();
			
			fly();

		}
	}
	
	

	system("pause");
	return 0;
}