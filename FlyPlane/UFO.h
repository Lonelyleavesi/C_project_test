#pragma once
#include"cocos2d.h"
#include"Constants.h"
USING_NS_CC;

enum class UfoType{		//道具类型
	FIREUP,				//火力加强 多子弹
	FIREMAX,
	BOMB				//炸弹
};

class Ufo : public Sprite {
public:
	bool initWithSpriteFrameType(const UfoType& type);   //道具的初始化
	static Ufo* create(const UfoType& type);

	CC_SYNTHESIZE(float, m_speed, Speed);      //道具的移动速度

	UfoType getType();
private:

	UfoType m_type;
};