#pragma once
#include"cocos2d.h"
#include"Constants.h"
USING_NS_CC;

enum class EnemyType {  //枚举类  如果不加class 里面的就是全球变量 
	SMALL_ENEMY = 0,			//枚举三种敌机 小中大
	MIDDLE_ENEMY = 1,
	BIG_ENEMY = 2
};

class Enemy : public Sprite {
public:
	Enemy() :m_type(EnemyType::SMALL_ENEMY), m_speed(SMALL_ENEMY_SPEED), m_hp(SMALL_ENEMY_HP), m_score(SMALL_ENEMY_SCORE) {};
	bool initWithSpriteFrameType(const EnemyType& type);

	static Enemy* create(const EnemyType& type);

	void move();
	bool gethurt(const int&);

	//移动速度  定义一个属性两个方法
	CC_SYNTHESIZE(float, m_speed, Speed);
	//生命值
	CC_SYNTHESIZE(int, m_hp, HP);
	//奖励积分
	CC_SYNTHESIZE(int, m_score, Score);
	
	int getHP();
	EnemyType getType();
private:
	EnemyType m_type;
	void down();      //敌机坠毁的方法
	void getinghit();

};