#pragma once
#include"cocos2d.h"
#include"Constants.h"
#include "UFO.h"
USING_NS_CC;

class Hero : public Sprite {
public:
	bool initWithFrame( );           //飞机的初始化与创造
	static Hero* createHero();
	//CREATE_FUNC(Hero);


	void Move();                  //英雄的移动
	int m_fireLevel;			  //飞机火力等级 与子弹排数有关
	int m_hurtLevel;			  //飞机火力伤害 单发子弹造成的伤害

	void LevelUP(UfoType type);
	void LevelDown(UfoType type);
	void TurnListener();

	int m_bomb;
	bool m_isMax;
	float m_Bullet_seq;
	CC_SYNTHESIZE(int, m_hp, HP);
	//CC_SYNTHESIZE(int, m_bomb, BOMB);
private:
	//生命值
	
	Vec2 m_offset;   //记录触摸点与飞机中心的偏移量 解决飞机瞬移方法1

	int m_doubleBulletsCount; //双子弹的数量
	int m_ThreeBulletsCount;  //三子弹的数量
	int m_maxBulletsCount;	  //强力子弹的数量
	EventListenerTouchOneByOne* Listener;
	
};