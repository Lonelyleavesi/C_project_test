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

	void update(float) override;   //��д�����update  ÿ֡����һ��update

								   //void cyclePicture(int ID);   //��װһ�������������ѭ��ͼƬ ��ѭ��ͼƬ����2��
	CREATE_FUNC(GameScene)

	
	void GameOver();
private:
	bool Pause_Status;   //��¼��ͣ״̬
	bool m_isOver;		 //��Ϸ�Ƿ����
	Vector<Sprite *>m_Bullets;   //��Ч�ӵ��ļ���
	Vector<Sprite *>nm_Bullets;

	Vector<Enemy *>m_Enemys;   //�����Ч����

	Vector<Ufo *>m_Ufos;

	Hero* Hero;
	int m_totalScore;    //��¼������Ϸ���ܷ�
	
	void UpdateGame();
	void createBackGround();			//��������
	void createMnue();					//�����˵� ����ը��ʹ�� ��ͣ

	void createHero();					//Ӣ�۵Ĵ���

	//void creatGameUI();					//����UI

	void createSimpleBullet(float);			//���������ӵ�
	void createBullet(float);				//����Ӣ��״̬��������ӵ�

	void createEnemy(const EnemyType&);		//�����л�
	void createSmallEnemy(float);
	void createMiddleEnemy(float);
	void createBigEnemy(float);

	void createUfo(float);					//��������
	//void createFireup(float);
	//void createFiremax(float);
	//void createBomb(float);

	
	void cycleBG();							//����ѭ��
	
	void upDateBomb();						//����ը��������
};