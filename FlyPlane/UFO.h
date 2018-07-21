#pragma once
#include"cocos2d.h"
#include"Constants.h"
USING_NS_CC;

enum class UfoType{		//��������
	FIREUP,				//������ǿ ���ӵ�
	FIREMAX,
	BOMB				//ը��
};

class Ufo : public Sprite {
public:
	bool initWithSpriteFrameType(const UfoType& type);   //���ߵĳ�ʼ��
	static Ufo* create(const UfoType& type);

	CC_SYNTHESIZE(float, m_speed, Speed);      //���ߵ��ƶ��ٶ�

	UfoType getType();
private:

	UfoType m_type;
};