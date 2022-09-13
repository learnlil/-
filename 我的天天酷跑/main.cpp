/*
������ܿ�����־
1.������Ŀ
2.�����ز�
3.������Ϸ����
    ʵ�ʿ�������
	�û���������
	ѡ��ͼ�ο�
	1)������Ϸ����
	2)�����Ϸ����
	  a.3�ر�����ͬ���ٶ�ͬʱ�ƶ�
	  b.ѭ������������ʵ��
	 3��ʵ����Ϸ����
	   a.���ر�����Դ
	   b.��Ⱦ(����֪ʶ������)
	   �������⣺����ͼƬ��png��ʽͼƬ���ֺ�ɫ
4.ʵ����ҵı���
5.ʵ����ҵ���Ծ
6.ʵ�����С�ڹ�
7.�����ϰ���ṹ����������
8.ʹ���ϰ���ṹ������³�ʼ��
9.��װ�����ϰ������ʾ
10.ʵ����ҵ��¶׼���
11.ʵ�֡����ӡ��ϰ���

C++  �㷨  ���ݿ� Windows��win32+mfc) Qt linux������
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

//����ͼƬ
IMAGE imgBgs[3];
int bgX[3];//����ͼƬx����
int bgSpeed[3] = { 2,4,8 };

IMAGE imgHeros[12];
int heroX;//���x����
int heroY;//���y����
int heroIndex;//��ұ��ܵ�ͼƬ�����
bool heroJump;//��ʾ���������Ծ
bool heroDown;
int heroJumpHeightMax;
int heroJumpOff;
bool upDate;//
int heroBlood;
int score;
IMAGE imgSZ[10];
//�¶�
IMAGE imgHeroDown[12];

//IMAGE imgTortoise;//С�ڹ�
//int torToiseX;//С�ڹ�ˮƽ����
//int torToiseY;//С�ڹ���ֵ����
//bool torToiseExist;//��ǰ�����Ƿ����С�ڹ�
typedef enum
{
	TORTOISE,//�ڹ� 0
	LION,//ʨ�� 1
	HOOK1,//2
	HOOK2,//3
	HOOK3,//4
	HOOK4,//5
	OBSTACLE_TYPE_COUNT //6
} obstacle_type;

//vector< vector<int>> data;

//IMAGE obstracleImgs[3][12];
vector<vector<IMAGE>>obstacleImgs;//IMAGE obstacleImgs[][];�����Ӵ�ϰ���ͼƬ

class obstacle
{
public:
	obstacle_type type;//�ϰ��������
	int x, y;//�ϰ�������
	int imgIndex;//��ǰ��ʾ��ͼƬde���
	int speed;
	int power;//ɱ����
	bool exist;
	bool hited;//��ʾ�Ƿ��Ѿ�������ײ
	bool passed;//��ʾ�Ƿ��Ѿ���ͨ��

};
obstacle obstacles[OBSTACLE_COUNT];

//��Ϸ��ʼ��
void init()
{
	//������Ϸ����
	initgraph(WIN_WIDTH,WIN_HEIGHT);

	

	//���ر�����Դ
	char name[64];
	for (int i = 0; i < 3; i++)
	{
		//"res/bg001.png"  "res/bg002.png" "res/bg003.png"
		sprintf(name, "res/bg%03d.png", i + 1);
		loadimage(&imgBgs[i],name);
		bgX[i] = 0;
	}

	//�������ﱼ�ܵ�ͼƬ���ز�
	for (int i = 0; i < 12; i++)
	{//"res/hero1.png".....
		sprintf(name, "res/hero%d.png", i + 1);
		loadimage(&imgHeros[i], name);
	}
	//������ҵĳ�ʼλ��
	heroX = WIN_WIDTH * 0.5 - imgHeros[0].getwidth() * 0.5;
	heroY = 345 - imgHeros[0].getheight();
	heroIndex = 0;
	heroJump = false;
	heroJumpHeightMax = 345 - imgHeros[0].getheight() - 120;
	heroJumpOff = -10;
	upDate = true;

	//����С�ڹ��ز�
	//loadimage(&imgTortoise, "res/t1.png");//�˴���Ҫ�Ż�
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

	//����ʨ��
	IMAGE imgLion;
	vector<IMAGE> imgLionArray;
	for (int i = 0; i < 6; i++)
	{
		sprintf(name, "res/p%d.png", i + 1);
		loadimage(&imgLion, name);
		imgLionArray.push_back(imgLion);
	}
	obstacleImgs.push_back(imgLionArray);
	//��ʼ���ϰ����
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		obstacles[i].exist = false;

	}
	
	//�����¶��ز�
	//loadimage(&imgHeroDown[0], "res/d1.png");
	//loadimage(&imgHeroDown[0], "res/d2.png");
	
	for (int i = 0; i < 12; i++)//�¶�֡�ز�
	{//"res/hero1.png".....
		sprintf(name, "res/bark/g%02d.png", i + 1);
		loadimage(&imgHeroDown[i], name);
	}
	heroDown = false;
	//����
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
	//Ԥ������Ч
	preLoadSound("res/hit.mp3");

	mciSendString("play res/bg.mp3", 0, 0, 0);
	score = 0;
	//��������ͼƬ
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
		obstacles[i].speed = 4;//�ٶ�
		obstacles[i].power = 5;//�˺�ֵ
		
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
			if (!heroDown)//���¶ף�������Ծ
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
				//cout << "Ѫ��ʣ��" << heroBlood << endl;
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
	heroIndex = 0;//�޸�
}
void down()
{
	heroDown = true;
	upDate = true;
	heroIndex = 0;
}
//�����û�����
void keyEvent()
{
	if (_kbhit())//����а������£�����true
	{
		char ch;
		ch = _getch();//getch()����Ҫ���»س�����ֱ�Ӷ�ȡ
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
	
	//ʵ����Ծ
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
	else//����Ծ
	{
		heroIndex = (heroIndex + 1) % 12;
	}

	//�����ϰ���
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
	
	//���������ϰ�������
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
	//��Һ��ϰ������ײ���
	checkHit();
}
//��Ⱦ"��Ϸ����"
void updateBg()
{

	putimagePNG2(bgX[0], 0, &imgBgs[0]);
	putimagePNG2(bgX[1], 119, &imgBgs[1]);
	putimagePNG2(bgX[2], 330, &imgBgs[2]);
}

//���ﶯ��
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
	//��ȾС�ڹ�
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
		FlushBatchDraw();//����δ��ɵ�ͼƬeasyxͼ�ο��е�
		mciSendString("stop res/bg.mp3", 0, 0, 0);
		system("pause");
		//��֮ͣ��ʼ��һ��
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
	//��ʾ��ʼ����
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