#include"LoadingScene.h"
#include"Constants.h"
#include"GameScene.h"

LoadingScene* LoadingScene::createScene(){
	return LoadingScene::create();
}

//btn_finish.png �ص���Ϸ
//game_loading1.png ����С�ɻ� ����
//game_loading2.png С�ɻ����� ����
//game_loading3.png С�ɻ�����2 ����
//gameover.png    ��Ϸ����  ��ϲ �������ճɼ�Ϊ
//shoot_copyright.png ��Ϸ��ʼ����
bool LoadingScene::init() {
		if (!Scene::init())
		{
			return FALSE;
		}
		//ͨ�����ؾ��鼯�ļ��������鶼���浽���鼯������
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist"); //�ٴμ�����һ�����黺�漯
		
		auto bg = Sprite::createWithSpriteFrameName("background.png");   //��ʼ�����ı���
		bg->setPosition(VISITABLE_ORIGIN + VISITABLE_SIZE / 2);
		this->addChild(bg,BACKGROUND_ZORDER);

		auto title = Sprite::createWithSpriteFrameName("shoot_copyright.png"); //����
		title->setPosition(VISITABLE_ORIGIN + VISITABLE_SIZE / 2);
		this->addChild(title);
		this->saveAnimation();    //���涯����������
		return true;
}

void LoadingScene::saveAnimation() {
	
	this->saveStartAnimation();			//��ʼ�Ķ���
	this->saveHeroFlyAnimation();		//�ɻ����ж���
	this->saveEnemy3Animation();		//��ɻ�����
	this->saveEnemyDieAnimation();		//�л���������
	this->saveEnemyHurtAnimation();		//�л����˶���
	this->saveHeroDieAnimation();		//Ӣ����������
	
}

void LoadingScene::saveStartAnimation() {
	//����Ϊ����   С�ɻ�����
	auto loading = Sprite::create();
	loading->setPosition(VISITABLE_ORIGIN + VISITABLE_SIZE / 2 + Vec2(0, -200));
	this->addChild(loading);
	auto ani = Animation::create();
	for (auto i = 0; i < 4; i++)
	{
		auto png = StringUtils::format("game_loading%d.png", i + 1);
		ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(png));

	}
	ani->setDelayPerUnit(0.1f);
	ani->setLoops(4);
	auto seq = Sequence::create(Animate::create(ani), CallFunc::create([]() {
		//������ת
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
	}), nullptr);
	loading->runAction(seq);
}

void LoadingScene::saveHeroFlyAnimation() {
	//�������������뻺��
	//��������Ӷ���
	//1.��������
	//a.������������
	auto hero_ani = Animation::create();
	//b.��Ӷ���֡(�Ӿ���֡�����и������ֲ��ҳ�����)
	hero_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
	hero_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
	//c.���ö����л�ʱ��
	hero_ani->setDelayPerUnit(HERO_CHANGE_TIME);
	//d.����ѭ������
	hero_ani->setLoops(CC_REPEAT_FOREVER);  //CC_REPEAT_FOREVER = -1 ��Ϊ��Զѭ��
											//e.���붯������
	AnimationCache::getInstance()->addAnimation(hero_ani, HERO_FLY_ANIMATION);
}

void LoadingScene::saveEnemy3Animation() {
	auto bigenemy_ani = Animation::create();
	//b.��Ӷ���֡(�Ӿ���֡�����и������ֲ��ҳ�����)
	bigenemy_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
	bigenemy_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png"));
	//c.���ö����л�ʱ��
	bigenemy_ani->setDelayPerUnit(0.2f);
	//d.����ѭ������
	bigenemy_ani->setLoops(CC_REPEAT_FOREVER);  //CC_REPEAT_FOREVER = -1 ��Ϊ��Զѭ��
	AnimationCache::getInstance()->addAnimation(bigenemy_ani, ENEMY3_FLY_ANIMATION);
}

void LoadingScene::saveEnemyDieAnimation() {
	auto ani_enemy1die = Animation::create();
	auto ani_enemy2die = Animation::create();
	auto ani_enemy3die = Animation::create();
	std::string frameName = "SMALL_ENEMY";
	for (int i = 1; i <= 4; i++)
	{
		{
			auto frameName = StringUtils::format("enemy1_down%d.png", i);
			ani_enemy1die->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
			ani_enemy1die->setDelayPerUnit(ENEMY_DIE_TIME);
			AnimationCache::getInstance()->addAnimation(ani_enemy1die, ENEMY1_DIE_ANIMATION);
		}

		{
			auto frameName = StringUtils::format("enemy2_down%d.png", i);
			ani_enemy2die->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
			ani_enemy2die->setDelayPerUnit(ENEMY_DIE_TIME);
			AnimationCache::getInstance()->addAnimation(ani_enemy2die, ENEMY2_DIE_ANIMATION);
		}
	}
	for (int i = 1; i <= 6; i++)
	{
		auto frameName = StringUtils::format("enemy3_down%d.png", i);
		ani_enemy3die->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
		ani_enemy3die->setDelayPerUnit(ENEMY_DIE_TIME);
		AnimationCache::getInstance()->addAnimation(ani_enemy3die, ENEMY3_DIE_ANIMATION);
	}
}

void LoadingScene::saveEnemyHurtAnimation() {
	//���ͷɻ���Ҫ���� ���ͷɻ� ��Ϊ�Դ����� ���Լ�����
	auto Enemy2_ani = Animation::create();
	Enemy2_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_hit.png"));
	Enemy2_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2.png"));
	Enemy2_ani->setDelayPerUnit(ENEMY_GETHURT_TIME); //����ʱ��
	AnimationCache::getInstance()->addAnimation(Enemy2_ani, ENEMY2_HURT_ANIMATION);
	auto Enemy3_ani = Animation::create();
	Enemy3_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_hit.png"));
	Enemy3_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
	Enemy3_ani->setDelayPerUnit(ENEMY_GETHURT_TIME); //����ʱ��
	AnimationCache::getInstance()->addAnimation(Enemy3_ani, ENEMY3_HURT_ANIMATION);

}

void LoadingScene::saveHeroDieAnimation() {
	auto ani_herodie = Animation::create();
	for (int i = 0; i < 4; i++)
	{
		auto frameName = StringUtils::format("hero_blowup_n%d.png", i + 1);
		ani_herodie->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
	ani_herodie->setDelayPerUnit(HERO_DIE_TIME); //����ʱ��
	AnimationCache::getInstance()->addAnimation(ani_herodie, HERO_DIE_ANIMATION);
}

