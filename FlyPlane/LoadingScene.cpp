#include"LoadingScene.h"
#include"Constants.h"
#include"GameScene.h"

LoadingScene* LoadingScene::createScene(){
	return LoadingScene::create();
}

//btn_finish.png 回到游戏
//game_loading1.png 加载小飞机 向右
//game_loading2.png 小飞机喷烟 向右
//game_loading3.png 小飞机喷烟2 向右
//gameover.png    游戏结束  恭喜 您的最终成绩为
//shoot_copyright.png 游戏开始标题
bool LoadingScene::init() {
		if (!Scene::init())
		{
			return FALSE;
		}
		//通过加载精灵集文件，将精灵都缓存到精灵集缓存中
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist"); //再次加载另一个精灵缓存集
		
		auto bg = Sprite::createWithSpriteFrameName("background.png");   //开始场景的背景
		bg->setPosition(VISITABLE_ORIGIN + VISITABLE_SIZE / 2);
		this->addChild(bg,BACKGROUND_ZORDER);

		auto title = Sprite::createWithSpriteFrameName("shoot_copyright.png"); //标题
		title->setPosition(VISITABLE_ORIGIN + VISITABLE_SIZE / 2);
		this->addChild(title);
		this->saveAnimation();    //储存动画到缓存中
		return true;
}

void LoadingScene::saveAnimation() {
	
	this->saveStartAnimation();			//开始的动画
	this->saveHeroFlyAnimation();		//飞机飞行动画
	this->saveEnemy3Animation();		//大飞机动画
	this->saveEnemyDieAnimation();		//敌机死亡动画
	this->saveEnemyHurtAnimation();		//敌机受伤动画
	this->saveHeroDieAnimation();		//英雄死亡动画
	
}

void LoadingScene::saveStartAnimation() {
	//以下为动画   小飞机动画
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
		//场景跳转
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
	}), nullptr);
	loading->runAction(seq);
}

void LoadingScene::saveHeroFlyAnimation() {
	//创建动画并放入缓存
	//给主角添加动画
	//1.创建动画
	//a.创建动画对象
	auto hero_ani = Animation::create();
	//b.添加动画帧(从精灵帧缓存中根据名字查找出来的)
	hero_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
	hero_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
	//c.设置动画切换时长
	hero_ani->setDelayPerUnit(HERO_CHANGE_TIME);
	//d.设置循环次数
	hero_ani->setLoops(CC_REPEAT_FOREVER);  //CC_REPEAT_FOREVER = -1 意为永远循环
											//e.放入动画缓存
	AnimationCache::getInstance()->addAnimation(hero_ani, HERO_FLY_ANIMATION);
}

void LoadingScene::saveEnemy3Animation() {
	auto bigenemy_ani = Animation::create();
	//b.添加动画帧(从精灵帧缓存中根据名字查找出来的)
	bigenemy_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
	bigenemy_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png"));
	//c.设置动画切换时长
	bigenemy_ani->setDelayPerUnit(0.2f);
	//d.设置循环次数
	bigenemy_ani->setLoops(CC_REPEAT_FOREVER);  //CC_REPEAT_FOREVER = -1 意为永远循环
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
	//中型飞机需要重置 大型飞机 因为自带动画 会自己重置
	auto Enemy2_ani = Animation::create();
	Enemy2_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_hit.png"));
	Enemy2_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2.png"));
	Enemy2_ani->setDelayPerUnit(ENEMY_GETHURT_TIME); //设置时长
	AnimationCache::getInstance()->addAnimation(Enemy2_ani, ENEMY2_HURT_ANIMATION);
	auto Enemy3_ani = Animation::create();
	Enemy3_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_hit.png"));
	Enemy3_ani->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
	Enemy3_ani->setDelayPerUnit(ENEMY_GETHURT_TIME); //设置时长
	AnimationCache::getInstance()->addAnimation(Enemy3_ani, ENEMY3_HURT_ANIMATION);

}

void LoadingScene::saveHeroDieAnimation() {
	auto ani_herodie = Animation::create();
	for (int i = 0; i < 4; i++)
	{
		auto frameName = StringUtils::format("hero_blowup_n%d.png", i + 1);
		ani_herodie->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
	ani_herodie->setDelayPerUnit(HERO_DIE_TIME); //设置时长
	AnimationCache::getInstance()->addAnimation(ani_herodie, HERO_DIE_ANIMATION);
}

