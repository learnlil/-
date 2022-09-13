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
	//����ͼƬ
	IMAGE imgBgs[3];
	int bgX[3];//����ͼƬx����
	int bgSpeed[3] = { 2,4,8 };
	void updatebackground();
};
void background::updatebackground()
{
	//���ر�����Դ
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
	int heroX;//���x����
	int heroY;//���y����
	int heroIndex;//��ұ��ܵ�ͼƬ�����
	bool heroJump;//��ʾ���������Ծ
	bool heroDown;
	bool upDate;
	int heroJumpHeightMax;
	int heroJumpOff;
	int heroBlood;
	void heroXY();
};
void Hero::heroXY()
{
	//������ҵĳ�ʼλ��
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

int main()
{

}