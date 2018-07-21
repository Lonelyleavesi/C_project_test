#pragma once
#include "cocos2d.h"
#include<vector>
#include"Enemy.h"
#include"Hero.h"
USING_NS_CC;

class GameScene : public Scene {
public:
	GameScene() :m_Bullets(), m_totalScore(0),Pause_Status(TRUE) {};
	static Scene* createScene();

	bool init() override;

	void update(float) override;   //重写父类的update  每帧调用一次update

								   //void cyclePicture(int ID);   //封装一个函数用于添加循环图片 （循环图片方法2）
	CREATE_FUNC(GameScene)

	
	void GameOver();
private:
	bool Pause_Status;   //记录暂停状态
	bool m_isOver;		 //游戏是否结束
	Vector<Sprite *>m_Bullets;   //有效子弹的集合
	Vector<Sprite *>nm_Bullets;

	Vector<Enemy *>m_Enemys;   //存放有效敌人

	Vector<Ufo *>m_Ufos;

	Hero* Hero;
	int m_totalScore;    //记录本次游戏的总分
	
	void UpdateGame();
	void createBackGround();			//创建背景
	void createMnue();					//创建菜单 包括炸弹使用 暂停

	void createHero();					//英雄的创建

	//void creatGameUI();					//创建UI

	void createSimpleBullet(float);			//创建单颗子弹
	void createBullet(float);				//根据英雄状态创建多个子弹

	void createEnemy(const EnemyType&);		//创建敌机
	void createSmallEnemy(float);
	void createMiddleEnemy(float);
	void createBigEnemy(float);

	void createUfo(float);					//创建道具
	//void createFireup(float);
	//void createFiremax(float);
	//void createBomb(float);

	
	void cycleBG();							//背景循环
	
	void upDateBomb();						//更新炸弹的数量
};