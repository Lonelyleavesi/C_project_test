#include"GameScene.h"
#include"time.h"
#include"Constants.h"
#include"OverScene.h"

Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init() {
	//1.先调用父类的init	
	if (!Scene::init())
	{
		return false;
	}
	srand((unsigned int)time(NULL));
	
	this->createBackGround();	//创建背景
	this-> createHero();		//添加主角飞机到图集并将新创建的Hero 保存在 场景类中的Hero成员
	if(m_isOver)Hero->Move();				//飞机的移动函数
	this->createMnue();			//创建菜单
	this->UpdateGame();			//游戏状态实时检测更新 以及飞机道具等随机出现相关计时器
	
	return true;
}


void GameScene::UpdateGame() {
	//每一帧调用一次Update函数
	scheduleUpdate();
	//定时创造子弹

	//定时创造敌人
	schedule(schedule_selector(GameScene::createSmallEnemy), CREATE_SMALL_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_SMALL_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createMiddleEnemy), CREATE_MIDDLE_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_MIDDLE_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createBigEnemy), CREATE_BIG_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_BIG_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createUfo), UFO_INTERVAL, CC_REPEAT_FOREVER, UFO_DELAY);
	//schedule(schedule_selector(GameScene::createBomb), UFO_BOMB_INTERVAL, CC_REPEAT_FOREVER, UFO_BOMB_DELAY);
	//component.scheduleOnce(function() {
	//	// 这里的 this 指向 component
	//	this.doSomething();
	//}, 2);
}
void GameScene::update(float delta) {
	//循环背景
	this->cycleBG();
	//遍历子弹集合
	schedule(schedule_selector(GameScene::createBullet), Hero->m_Bullet_seq);
	Vector<Sprite *>removableBullets;
	for (auto bullet : m_Bullets)
	{
		if(Hero->m_isMax)	bullet->setPositionY(bullet->getPositionY() + MAX_BULLET_SPEED);
		else bullet->setPositionY(bullet->getPositionY() + BULLET_SPEED);
		if (bullet->getPositionY() >= VISITABLE_SIZE.height) {
			this->removeChild(bullet);
			//不能在遍历集合时，修改成员的数量，所以，仅仅是吧无效子弹存放到临时集合用于销毁
			removableBullets.pushBack(bullet);
		}
	}
	//-------------------------------------------------------------------
	//遍历敌机
	Vector<Enemy *>removableenemy;
	for (auto enemy : m_Enemys)
	{
		enemy->move();
		if (enemy->getPositionY() + enemy->getContentSize().height / 2 <= 0)
		{
			this->removeChild(enemy);
			removableenemy.pushBack(enemy);
		}
	}
	//---------------------------------------------------------------------------------------
	
	//碰撞检测
	for (auto enemy : m_Enemys)
	{
		for (auto bullet : m_Bullets)
		{
			if (enemy->getBoundingBox().intersectsRect(bullet->getBoundingBox()))
			{
				//====================================================================================
				if (enemy->gethurt(Hero->m_hurtLevel)) {

					//log("constract...");
					m_totalScore += enemy->getScore();
					removableenemy.pushBack(enemy);
					//更新得分
					auto nodeScore = this->getChildByTag(LABEL_SCORE_TAG);  //先取得node
					auto lblScore = dynamic_cast<Label *>(nodeScore);		//强制转化为lbl类型
					std::string strScore = StringUtils::format("%d", m_totalScore);  //取得分数的string
					lblScore->setString(strScore);

					if (enemy->getType() == EnemyType::BIG_ENEMY)
						this->createUfo(1);
					//log("%d", m_totalScore);
				};  //敌机被销毁的动画 
				removableBullets.pushBack(bullet);
				this->removeChild(bullet);
			}
		}
		//-----检测敌机撞上英雄的敌机-------------------------------------------
		if (Hero->getBoundingBox().intersectsRect(enemy->getBoundingBox()))
		{
			this->GameOver();
		}
	}
	//========================================================================================================
	//遍历道具
	Vector<Ufo *>removableUFO;
	for (auto Ufo : m_Ufos)
	{

		if (Ufo->getPositionY() + Ufo->getContentSize().height / 2 <= 0)  //道具越界
		{
			removableUFO.pushBack(Ufo);
			this->removeChild(Ufo);
		}
		if (Ufo->getBoundingBox().intersectsRect(Hero->getBoundingBox()))  //道具与飞机碰撞
		{
			removableUFO.pushBack(Ufo);
			this->removeChild(Ufo);
			Hero->LevelUP(Ufo->getType());
			//log("get props,This Hero FireLEVEL is %d", Hero->m_fireLevel);
		}

	}
	this->upDateBomb();

	//子弹遍历后销毁放入removeable中的对象
	for (auto bullet : removableBullets)
	{
		m_Bullets.eraseObject(bullet);
	}
	removableBullets.clear();
	//敌机遍历后销毁
	for (auto Enemy : removableenemy)
	{
		m_Enemys.eraseObject(Enemy);
	}
	removableenemy.clear();
	//销毁道具
	for (auto Ufo : removableUFO)
	{
		m_Ufos.eraseObject(Ufo);
	}
	removableUFO.clear();
};
//=====================================================================================================================
void GameScene::createBackGround()
{
	//背景
	//auto bg1 = Sprite::create("shoot_background");
	auto bg1 = Sprite::createWithSpriteFrameName("background.png");
	bg1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);//移动锚点到左下角 
	//bg1->setPosition(origin.x+size.width/2,origin.y+bg1->getContentSize().height/2);  //纵坐标加精灵高度的一半使左下角对齐
	//bg1->setPosition(origin + size / 2);//向量可以相除相加,
	//开启抗锯齿
	bg1->getTexture()->setAliasTexParameters();
	this->addChild(bg1, BACKGROUND_ZORDER, BACKGROUND_TAG_1);  //把bg1加入到 对象池中 并且id =1  ，覆盖权重为-1（越小越底）


	auto bg2 = Sprite::createWithSpriteFrameName("background.png");  //bg2用于背景滚动时与bg1循环
	bg2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);//移动锚点到左下角
	bg2->setPositionY(VISITABLE_ORIGIN.y + bg1->getContentSize().height);  //先把第二张图片移动到第一张图片的上方
																 //开启抗锯齿
	bg2->getTexture()->setAliasTexParameters();
	this->addChild(bg2, BACKGROUND_ZORDER, BACKGROUND_TAG_2);
}

void GameScene::createMnue() {
	//UI 显示分值
	auto lblScore = Label::createWithBMFont("font.fnt", StringUtils::format("%d", this->m_totalScore));
	lblScore->setPosition(10, VISITABLE_SIZE.height - 10);
	lblScore->setColor(UI_SCORE_COLOR);
	lblScore->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->addChild(lblScore, UI_ZORDER, LABEL_SCORE_TAG);
	//---------------------------------------------------------------------------------------
	//炸弹数量菜单创建
	auto spBomb = Sprite::createWithSpriteFrameName("bomb.png");     //创建一个精灵
	auto itemBome = MenuItemSprite::create(spBomb, spBomb, [this, lblScore](Ref*) {    //通过精灵创建事件
		for (auto enemy : this->m_Enemys)
		{
			enemy->gethurt(NUM_BOMB_DAMAGE);
			this->m_totalScore += enemy->getScore();  //如果爆炸杀敌机加积分就使用这一行
		}
		//this->m_Enemys.clear();      //如果爆炸是杀所有敌机
		lblScore->setString(StringUtils::format("%d", this->m_totalScore)); //如果爆炸杀怪加积分就使用这一行
		this->Hero->LevelDown(UfoType::BOMB);
		this->upDateBomb();
	});
	itemBome->setPosition(itemBome->getContentSize());

	//----------------------------------------------------------------------------------------------
	//暂停菜单  
	auto spPauseNormal = Sprite::createWithSpriteFrameName("game_pause_nor.png");  //先取出2个暂停菜单元素
	auto spPauseSelected = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
	auto spResumeNormal = Sprite::createWithSpriteFrameName("game_resume_nor.png");  //两个恢复菜单元素
	auto spResumeSelected = Sprite::createWithSpriteFrameName("game_resume_pressed.png");
	auto itemPause = MenuItemSprite::create(spPauseNormal, spPauseSelected);   //
	auto itemResume = MenuItemSprite::create(spResumeNormal, spResumeSelected);

	auto toggle = MenuItemToggle::createWithCallback([this, itemBome](Ref *sender) {
		//获取当前选择项的下标（从0开始） 在这里 第1项是 暂停 ;0是恢复
		int index = dynamic_cast<MenuItemToggle *>(sender)->getSelectedIndex();
		if (index) {
			Director::getInstance()->pause();
			this->Hero->TurnListener();
			itemBome->setEnabled(FALSE);

		}
		else {

			Director::getInstance()->resume();
			this->Hero->TurnListener();
			itemBome->setEnabled(TRUE);
		}
	}, itemPause, itemResume, nullptr);
	toggle->setPosition(VISITABLE_SIZE - toggle->getContentSize());
	//上面元素加入菜单
	auto menu = Menu::create();          // 把炸弹 暂停加入到菜单 让菜单处理
	menu->addChild(itemBome, 1, ITEM_BOMB_TAG);
	menu->addChild(toggle);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, UI_ZORDER, MENU_TAG);               //在将菜单加入到场景中 默认中央
	//
	//炸弹的数量
	auto lblBomb = Label::createWithBMFont("font.fnt", StringUtils::format("X%d", Hero->m_bomb)); //添加炸弹的数量 X %d
	lblBomb->setPosition(itemBome->getPosition() + Vec2(itemBome->getContentSize().width, 0));
	lblBomb->setColor(Color3B(220, 80, 40));
	this->addChild(lblBomb, UI_ZORDER, LABEL_BOMB_TAG);
}

void GameScene::createHero() {
	auto hero = Hero::createHero( );
	hero->setPositionX(VISITABLE_ORIGIN.x + VISITABLE_SIZE.width / 2);
	hero->setPositionY(VISITABLE_ORIGIN.y + hero->getContentSize().height / 2);
	hero->setColor(Color3B(0, 255, 0));
	hero->setScale(0.5);
	this->addChild(hero, FORGROUND_ZORDER + 1, HERO_TAG);
	Hero = hero;
}

void GameScene::createBullet(float)//创建一个子弹并加入到集合
{
	if (Hero->m_isMax) {
		this->createSimpleBullet(-13);
		this->createSimpleBullet(13);
		Hero->LevelDown(UfoType::FIREMAX);
	}
	else if (Hero->m_fireLevel == 1) {  //一颗子弹
		this->createSimpleBullet(0);
	}
	else if (Hero->m_fireLevel ==2) { 	//双颗子弹
		this->createSimpleBullet(-13);
		this->createSimpleBullet(13);
		Hero->LevelDown(UfoType::FIREUP);
	}
	else if (Hero->m_fireLevel == 3) { 	//三颗子弹
		this->createSimpleBullet(-15);
		this->createSimpleBullet(15);
		this->createSimpleBullet(0);
		Hero->LevelDown(UfoType::FIREUP);
	}
}

void GameScene::createSimpleBullet(float OriginX)//创建一个子弹并加入到集合
{
	auto bullet = Sprite::createWithSpriteFrameName("bullet1.png");
	if (Hero->m_isMax)
	{
		bullet->setColor(MAX_BULLET_COLOR);
		bullet->setPosition(Hero->getPositionX() + OriginX, Hero->getPositionY());
		this->addChild(bullet, FORGROUND_ZORDER);
		//将新建子弹加入集合
		this->m_Bullets.pushBack(bullet);
	}
	else {	
		bullet->setColor(BULLET_COLOR);
		bullet->setPosition(Hero->getPositionX() + OriginX, Hero->getPositionY());
		this->addChild(bullet, FORGROUND_ZORDER);
		//将新建子弹加入集合
		this->m_Bullets.pushBack(bullet);
	}
}

void GameScene::createEnemy(const EnemyType& type)//创建一个敌机并加入到集合
{
	//auto Enemy = Sprite::createWithSpriteFrameName("enemy1.png");
	auto Enemy = Enemy::create(type);   //由于枚举时用的class枚举 所以在这里调用时需要 前置EnemyType::
	float minX = Enemy->getContentSize().width / 2;
	float maxX = VISITABLE_SIZE.width - minX;
	//随机数%（max - min + 1 ） + min
	//6~9 :  2 % (4)+6
	float x = rand() % (int)(maxX - minX) + minX;
	Enemy->setPosition(x, VISITABLE_SIZE.height + Enemy->getContentSize().height / 2);
	Enemy->setColor(ENEMY_COLOR);
	this->addChild(Enemy, FORGROUND_ZORDER);
	this->m_Enemys.pushBack(Enemy);
}


void GameScene::createSmallEnemy(float)
{
	this->createEnemy(EnemyType::SMALL_ENEMY);
}
void GameScene::createMiddleEnemy(float)
{
	this->createEnemy(EnemyType::MIDDLE_ENEMY);
}
void GameScene::createBigEnemy(float)
{
	this->createEnemy(EnemyType::BIG_ENEMY);
}

void GameScene::createUfo(float) {
	UfoType type;
	int Type_rand;
	Type_rand = rand() % 3;
	switch (Type_rand)
	{
	case 0: type = UfoType::FIREUP; break;
	case 1: type = UfoType::BOMB; break;
	case 2: type = UfoType::FIREMAX; break;
	default:
		break;
	}
	auto Ufo = Ufo::create(type);
	int X_RAND;               //道具出现的随机范围
	X_RAND = rand() % (UFO_RAND_RANGE + 1) - UFO_RAND_RANGE;
	float minX = Ufo->getContentSize().width / 2 + X_RAND;
	float maxX = VISITABLE_SIZE.width - minX - X_RAND;
	float x = rand() % (int)(maxX - minX) + minX;
	Ufo->setPosition(x, VISITABLE_SIZE.height + Ufo->getContentSize().height / 2);
	switch (Ufo->getType())              //设置不同道具的颜色
	{
	case UfoType::FIREUP: Ufo->setColor(UFO_FIREUP_COLOR);
		break;
	case UfoType::FIREMAX: Ufo->setColor(UFO_FIREMAX_COLOR);
		break;
	case UfoType::BOMB:	Ufo->setColor(UFO_BOMB_COLOR);
		break;
	default:
		break;
	}
	this->addChild(Ufo, FORGROUND_ZORDER);
	this->m_Ufos.pushBack(Ufo);
	//通过顺序序列 让道具先下来再上去

	//log("RAND_X is %d", X_RAND);
	auto movedown = MoveTo::create(UFO_FIRSTDOWN_TIME, Vec2(Ufo->getPositionX(), (VISITABLE_SIZE.height - VISITABLE_SIZE.height / 2)));
	Ufo->setPositionX(Ufo->getPositionX() + X_RAND);
	auto moveup = MoveTo::create(UFO_UP_TIME, Vec2(Ufo->getPositionX(), (VISITABLE_SIZE.height + VISITABLE_SIZE.height / 4)));
	Ufo->setPositionX(Ufo->getPositionX() + X_RAND);
	auto Ufodown = MoveTo::create(UFO_SECONDDOWN_TIME, Vec2(Ufo->getPositionX(), (0 - Ufo->getContentSize().height)));
	auto seq = Sequence::create(movedown,moveup,Ufodown, RemoveSelf::create(), nullptr);
	Ufo->runAction(seq);
}

//void GameScene::createFireup(float) {
//	this->createUfo();
//}
//
//void GameScene::createBomb(float) {
//	this->createUfo();
//}


void GameScene::cycleBG() {
	//场景循环滚动
	auto bg1 = this->getChildByTag(BACKGROUND_TAG_1);
	auto bg2 = this->getChildByTag(BACKGROUND_TAG_2);

	bg1->setPositionY(bg1->getPositionY() - BACKGROUND_SPEED);
	bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);

	//（方法1）当第一张图片全部走完后直接将第一张图片重置，人眼看不出来，第二张图片定位到第一个图片的上方 -2（为了消除黑边）
	if (bg1->getPositionY() + bg1->getContentSize().height <= 0)
	{
		bg1->setPositionY(0);

	}
	/*方法2
	cyclePicture(1);
	cyclePicture(2);*/
	//------------------------------------------------------------------------
}
//（循环图片方法2）只需传入图片ID
//void GameScene::cyclePicture(int aimID)
//{
//	auto bg = this->getChildByTag(aimID);
//	bg->setPositionY(bg->getPositionY() - 4);
//	if (bg->getPositionY() + bg->getContentSize().height <= 0)
//	{
//		bg->setPositionY((bg->getContentSize().height));
//	}
//}
/*
	1,当炸弹数为0时，菜单项和标签都不显示
	2.当炸弹数为1时，只显示菜单项
	3.当炸弹数大于1时，显示菜单项和标签，且更新标签显示内容
*/
void GameScene::upDateBomb()   
{
	auto menu = this->getChildByTag(MENU_TAG);
	auto itemBomb = menu->getChildByTag(ITEM_BOMB_TAG);
	auto lblBomb = this->getChildByTag(LABEL_BOMB_TAG);
	if (Hero->m_bomb <= 0) {
		itemBomb->setVisible(false);
		lblBomb->setVisible(false);
	}
	else if (Hero->m_bomb == 1) {
		itemBomb->setVisible(true);
		lblBomb->setVisible(false);
	}
	else {
		itemBomb->setVisible(true);
		lblBomb->setVisible(true);
		dynamic_cast<Label *>(lblBomb)->setString(StringUtils::format("X%d",Hero->m_bomb));
	}
}

void GameScene::GameOver() {
	this->m_isOver = true;
	//1.执行爆炸动画
	for (auto node : this->getChildren())
	{
		node->stopAllActions();
	}
	//b.添加动画帧(从精灵帧缓存中根据名字查找出来的)
	auto ani_herodie = AnimationCache::getInstance()->getAnimation(HERO_DIE_ANIMATION);
	//seq中放顺序动作 参数最后要加null 或 nullptr 以表示最后动作
	auto seq = Sequence::create(Animate::create(ani_herodie), CallFunc::create([this]() {  //这个lambada表达式中 node表示的就是Enemy（后面要执行的node）
		//4.跳转场景
		auto scene = OverScene::createScene(this->m_totalScore);
		Director::getInstance()->replaceScene(TransitionFade::create(1, scene));																	 //this->removeChild(node);        //callfunN需要传参数 callfun不需要参数
	}), nullptr);

	Hero->runAction(seq);
	//3.停止所有计时器
	this->unscheduleAllSelectors();
	
	
}