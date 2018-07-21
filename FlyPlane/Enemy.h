#pragma once
#include"cocos2d.h"
#include"Constants.h"
USING_NS_CC;

enum class EnemyType {  //ö����  �������class ����ľ���ȫ����� 
	SMALL_ENEMY = 0,			//ö�����ֵл� С�д�
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

	//�ƶ��ٶ�  ����һ��������������
	CC_SYNTHESIZE(float, m_speed, Speed);
	//����ֵ
	CC_SYNTHESIZE(int, m_hp, HP);
	//��������
	CC_SYNTHESIZE(int, m_score, Score);
	
	int getHP();
	EnemyType getType();
private:
	EnemyType m_type;
	void down();      //�л�׹�ٵķ���
	void getinghit();

};