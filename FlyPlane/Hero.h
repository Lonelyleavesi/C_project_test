#pragma once
#include"cocos2d.h"
#include"Constants.h"
#include "UFO.h"
USING_NS_CC;

class Hero : public Sprite {
public:
	bool initWithFrame( );           //�ɻ��ĳ�ʼ���봴��
	static Hero* createHero();
	//CREATE_FUNC(Hero);


	void Move();                  //Ӣ�۵��ƶ�
	int m_fireLevel;			  //�ɻ������ȼ� ���ӵ������й�
	int m_hurtLevel;			  //�ɻ������˺� �����ӵ���ɵ��˺�

	void LevelUP(UfoType type);
	void LevelDown(UfoType type);
	void TurnListener();

	int m_bomb;
	bool m_isMax;
	float m_Bullet_seq;
	CC_SYNTHESIZE(int, m_hp, HP);
	//CC_SYNTHESIZE(int, m_bomb, BOMB);
private:
	//����ֵ
	
	Vec2 m_offset;   //��¼��������ɻ����ĵ�ƫ���� ����ɻ�˲�Ʒ���1

	int m_doubleBulletsCount; //˫�ӵ�������
	int m_ThreeBulletsCount;  //���ӵ�������
	int m_maxBulletsCount;	  //ǿ���ӵ�������
	EventListenerTouchOneByOne* Listener;
	
};