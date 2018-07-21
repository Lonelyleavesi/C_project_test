#pragma once

#define BULLETSNUM			 10 
#define VISITABLE_SIZE		 Director::getInstance()->getVisibleSize()
#define VISITABLE_ORIGIN     Director::getInstance()->getVisibleOrigin()

//��ǩ
#define BACKGROUND_TAG_1	 1
#define BACKGROUND_TAG_2	 2
#define HERO_TAG			 3
#define SMALLENEMY_TAG       4
#define UFO_TAG				 5
#define LABEL_SCORE_TAG		 10  //UI
#define	MENU_TAG		     11
#define ITEM_BOMB_TAG        12
#define LABEL_BOMB_TAG       13
#define ITEM_RESTART_TAG	 14
#define	REST_MENU_TAG		 15


//ZORDER �ڵ���ϵ
#define BACKGROUND_ZORDER   -1
#define FORGROUND_ZORDER     1
#define UI_ZORDER            99

//�ٶ�
#define BACKGROUND_SPEED	 1
#define BULLET_SPEED		 10   //�ӵ��ٶ�
#define MAX_BULLET_SPEED     15
#define SMALL_ENEMY_SPEED	 -8.0f	  //��ͬ�л��ٶ�
#define MIDDLE_ENEMY_SPEED   -4.0f
#define BIG_ENEMY_SPEED	     -1.3f


//��ʱ�����
#define CREATE_BULLET_INERVAL		  0.1f  //�ӵ����ּ��
#define MAX_BULLET_INERVAL            0.03f
#define CREATE_SMALL_ENEMY_INTERVAL   0.3f  //��ͬ�л����ֵļ��
#define CREATE_MIDDLE_ENEMY_INTERVAL  3.0f
#define CREATE_BIG_ENEMY_INTERVAL	  15.0f
		
#define ENEMY_GETHURT_TIME			  0.5f

#define CREATE_SMALL_ENEMY_DELAY	   0.35f   //��ͬ�ɻ����ֵ��ӳ�
#define CREATE_MIDDLE_ENEMY_DELAY	   2.0f
#define CREATE_BIG_ENEMY_DELAY		  5.0f
  
#define UFO_INTERVAL			30.0f   //������ߵĳ���ʱ��
#define UFO_FIREUP_INTERVAL		30.0f   //���ֵ��߳��ֵļ��
#define UFO_BOMB_INTERVAL		30.0f   

#define UFO_DELAY			  1.0f   //������߳��ֵ��ӳ�
#define UFO_FIREUP_DELAY	  1.0f   //���ֵ��߳��ֵ��ӳ�
#define UFO_BOMB_DELAY		  20.0f  

#define UFO_FIRSTDOWN_TIME        0.7f   //���ߵ�һ��������ʱ��
#define UFO_UP_TIME				  2.0f   //���ߵ�һ����ȥ��ʱ��
#define UFO_SECONDDOWN_TIME       1.0f   //���ߵڶ���������ʱ��

//����
#define HERO_CHANGE_TIME			  0.2f   //���鶯���л���ʱ��
#define ENEMY_DIE_TIME			      0.2f
#define HERO_DIE_TIME				  0.2f

//HP
#define SMALL_ENEMY_HP			3      //���ֵл�������
#define MIDDLE_ENEMY_HP			20
#define BIG_ENEMY_HP			80

//��ͬ�л�����
#define SMALL_ENEMY_SCORE		9
#define MIDDLE_ENEMY_SCORE		99
#define BIG_ENEMY_SCORE			999

//��ɫ
#define BULLET_COLOR			Color3B(rand() % 255, rand() % 255, rand() % 255)//Color3B(255, 215, 0)
#define MAX_BULLET_COLOR		Color3B(255,255,255)
#define ENEMY_COLOR				Color3B(rand() % 255, rand() % 255, rand() % 255)
#define UFO_COLOR				Color3B(255,0,0)
#define UFO_FIREUP_COLOR		Color3B(255,0,0)
#define UFO_BOMB_COLOR			Color3B(255,0,0)
#define UFO_FIREMAX_COLOR		Color3B(0,255,0)
#define UI_SCORE_COLOR			Color3B(255, 0, 0)

//�����Χ
#define UFO_RAND_RANGE          100

//����Ч��
#define NUM_DOUBLEBULLET        200   //˫�ӵ�������
#define NUM_MAXBULLET			200

//�����˺�
#define NUM_BOMB_DAMAGE         50

//���������
#define HERO_FLY_ANIMATION		"HEROFLY"
#define HERO_DIE_ANIMATION		"HERODIE"
#define ENEMY3_FLY_ANIMATION	"ENEMY3FLY"
#define ENEMY2_HURT_ANIMATION	"ENEM2HURT"
#define ENEMY3_HURT_ANIMATION	"ENEM3HURT"
#define ENEMY1_DIE_ANIMATION	"ENEMY1DIE"
#define ENEMY2_DIE_ANIMATION	"ENEMY2DIE"
#define ENEMY3_DIE_ANIMATION	"ENEMY3DIE"

//��߷ּ�ֵ
#define HIGH_SCORE_KEY "HEIGHSCORE"