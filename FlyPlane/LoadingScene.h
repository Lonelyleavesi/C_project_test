#pragma once
#include"cocos2d.h"

USING_NS_CC;

class LoadingScene : public Scene {   //¿ªÊ¼³¡¾°
public:
	static LoadingScene* createScene();
	bool init() override; 
	void saveAnimation();
	

	CREATE_FUNC(LoadingScene);

private:
	void saveStartAnimation();
	void saveHeroFlyAnimation();
	void saveEnemy3Animation();
	void saveEnemyHurtAnimation();
	void saveEnemyDieAnimation();
	void saveHeroDieAnimation();
};