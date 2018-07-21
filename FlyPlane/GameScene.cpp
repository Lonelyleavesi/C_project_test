#include"GameScene.h"
#include"time.h"
#include"Constants.h"
#include"OverScene.h"

Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init() {
	//1.�ȵ��ø����init	
	if (!Scene::init())
	{
		return false;
	}
	srand((unsigned int)time(NULL));
	
	this->createBackGround();	//��������
	this-> createHero();		//������Ƿɻ���ͼ�������´�����Hero ������ �������е�Hero��Ա
	if(m_isOver)Hero->Move();				//�ɻ����ƶ�����
	this->createMnue();			//�����˵�
	this->UpdateGame();			//��Ϸ״̬ʵʱ������ �Լ��ɻ����ߵ����������ؼ�ʱ��
	
	return true;
}


void GameScene::UpdateGame() {
	//ÿһ֡����һ��Update����
	scheduleUpdate();
	//��ʱ�����ӵ�

	//��ʱ�������
	schedule(schedule_selector(GameScene::createSmallEnemy), CREATE_SMALL_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_SMALL_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createMiddleEnemy), CREATE_MIDDLE_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_MIDDLE_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createBigEnemy), CREATE_BIG_ENEMY_INTERVAL, CC_REPEAT_FOREVER, CREATE_BIG_ENEMY_DELAY);
	schedule(schedule_selector(GameScene::createUfo), UFO_INTERVAL, CC_REPEAT_FOREVER, UFO_DELAY);
	//schedule(schedule_selector(GameScene::createBomb), UFO_BOMB_INTERVAL, CC_REPEAT_FOREVER, UFO_BOMB_DELAY);
	//component.scheduleOnce(function() {
	//	// ����� this ָ�� component
	//	this.doSomething();
	//}, 2);
}
void GameScene::update(float delta) {
	//ѭ������
	this->cycleBG();
	//�����ӵ�����
	schedule(schedule_selector(GameScene::createBullet), Hero->m_Bullet_seq);
	Vector<Sprite *>removableBullets;
	for (auto bullet : m_Bullets)
	{
		if(Hero->m_isMax)	bullet->setPositionY(bullet->getPositionY() + MAX_BULLET_SPEED);
		else bullet->setPositionY(bullet->getPositionY() + BULLET_SPEED);
		if (bullet->getPositionY() >= VISITABLE_SIZE.height) {
			this->removeChild(bullet);
			//�����ڱ�������ʱ���޸ĳ�Ա�����������ԣ������ǰ���Ч�ӵ���ŵ���ʱ������������
			removableBullets.pushBack(bullet);
		}
	}
	//-------------------------------------------------------------------
	//�����л�
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
	
	//��ײ���
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
					//���µ÷�
					auto nodeScore = this->getChildByTag(LABEL_SCORE_TAG);  //��ȡ��node
					auto lblScore = dynamic_cast<Label *>(nodeScore);		//ǿ��ת��Ϊlbl����
					std::string strScore = StringUtils::format("%d", m_totalScore);  //ȡ�÷�����string
					lblScore->setString(strScore);

					if (enemy->getType() == EnemyType::BIG_ENEMY)
						this->createUfo(1);
					//log("%d", m_totalScore);
				};  //�л������ٵĶ��� 
				removableBullets.pushBack(bullet);
				this->removeChild(bullet);
			}
		}
		//-----���л�ײ��Ӣ�۵ĵл�-------------------------------------------
		if (Hero->getBoundingBox().intersectsRect(enemy->getBoundingBox()))
		{
			this->GameOver();
		}
	}
	//========================================================================================================
	//��������
	Vector<Ufo *>removableUFO;
	for (auto Ufo : m_Ufos)
	{

		if (Ufo->getPositionY() + Ufo->getContentSize().height / 2 <= 0)  //����Խ��
		{
			removableUFO.pushBack(Ufo);
			this->removeChild(Ufo);
		}
		if (Ufo->getBoundingBox().intersectsRect(Hero->getBoundingBox()))  //������ɻ���ײ
		{
			removableUFO.pushBack(Ufo);
			this->removeChild(Ufo);
			Hero->LevelUP(Ufo->getType());
			//log("get props,This Hero FireLEVEL is %d", Hero->m_fireLevel);
		}

	}
	this->upDateBomb();

	//�ӵ����������ٷ���removeable�еĶ���
	for (auto bullet : removableBullets)
	{
		m_Bullets.eraseObject(bullet);
	}
	removableBullets.clear();
	//�л�����������
	for (auto Enemy : removableenemy)
	{
		m_Enemys.eraseObject(Enemy);
	}
	removableenemy.clear();
	//���ٵ���
	for (auto Ufo : removableUFO)
	{
		m_Ufos.eraseObject(Ufo);
	}
	removableUFO.clear();
};
//=====================================================================================================================
void GameScene::createBackGround()
{
	//����
	//auto bg1 = Sprite::create("shoot_background");
	auto bg1 = Sprite::createWithSpriteFrameName("background.png");
	bg1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);//�ƶ�ê�㵽���½� 
	//bg1->setPosition(origin.x+size.width/2,origin.y+bg1->getContentSize().height/2);  //������Ӿ���߶ȵ�һ��ʹ���½Ƕ���
	//bg1->setPosition(origin + size / 2);//��������������,
	//���������
	bg1->getTexture()->setAliasTexParameters();
	this->addChild(bg1, BACKGROUND_ZORDER, BACKGROUND_TAG_1);  //��bg1���뵽 ������� ����id =1  ������Ȩ��Ϊ-1��ԽСԽ�ף�


	auto bg2 = Sprite::createWithSpriteFrameName("background.png");  //bg2���ڱ�������ʱ��bg1ѭ��
	bg2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);//�ƶ�ê�㵽���½�
	bg2->setPositionY(VISITABLE_ORIGIN.y + bg1->getContentSize().height);  //�Ȱѵڶ���ͼƬ�ƶ�����һ��ͼƬ���Ϸ�
																 //���������
	bg2->getTexture()->setAliasTexParameters();
	this->addChild(bg2, BACKGROUND_ZORDER, BACKGROUND_TAG_2);
}

void GameScene::createMnue() {
	//UI ��ʾ��ֵ
	auto lblScore = Label::createWithBMFont("font.fnt", StringUtils::format("%d", this->m_totalScore));
	lblScore->setPosition(10, VISITABLE_SIZE.height - 10);
	lblScore->setColor(UI_SCORE_COLOR);
	lblScore->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->addChild(lblScore, UI_ZORDER, LABEL_SCORE_TAG);
	//---------------------------------------------------------------------------------------
	//ը�������˵�����
	auto spBomb = Sprite::createWithSpriteFrameName("bomb.png");     //����һ������
	auto itemBome = MenuItemSprite::create(spBomb, spBomb, [this, lblScore](Ref*) {    //ͨ�����鴴���¼�
		for (auto enemy : this->m_Enemys)
		{
			enemy->gethurt(NUM_BOMB_DAMAGE);
			this->m_totalScore += enemy->getScore();  //�����ըɱ�л��ӻ��־�ʹ����һ��
		}
		//this->m_Enemys.clear();      //�����ը��ɱ���ел�
		lblScore->setString(StringUtils::format("%d", this->m_totalScore)); //�����ըɱ�ּӻ��־�ʹ����һ��
		this->Hero->LevelDown(UfoType::BOMB);
		this->upDateBomb();
	});
	itemBome->setPosition(itemBome->getContentSize());

	//----------------------------------------------------------------------------------------------
	//��ͣ�˵�  
	auto spPauseNormal = Sprite::createWithSpriteFrameName("game_pause_nor.png");  //��ȡ��2����ͣ�˵�Ԫ��
	auto spPauseSelected = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
	auto spResumeNormal = Sprite::createWithSpriteFrameName("game_resume_nor.png");  //�����ָ��˵�Ԫ��
	auto spResumeSelected = Sprite::createWithSpriteFrameName("game_resume_pressed.png");
	auto itemPause = MenuItemSprite::create(spPauseNormal, spPauseSelected);   //
	auto itemResume = MenuItemSprite::create(spResumeNormal, spResumeSelected);

	auto toggle = MenuItemToggle::createWithCallback([this, itemBome](Ref *sender) {
		//��ȡ��ǰѡ������±꣨��0��ʼ�� ������ ��1���� ��ͣ ;0�ǻָ�
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
	//����Ԫ�ؼ���˵�
	auto menu = Menu::create();          // ��ը�� ��ͣ���뵽�˵� �ò˵�����
	menu->addChild(itemBome, 1, ITEM_BOMB_TAG);
	menu->addChild(toggle);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, UI_ZORDER, MENU_TAG);               //�ڽ��˵����뵽������ Ĭ������
	//
	//ը��������
	auto lblBomb = Label::createWithBMFont("font.fnt", StringUtils::format("X%d", Hero->m_bomb)); //���ը�������� X %d
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

void GameScene::createBullet(float)//����һ���ӵ������뵽����
{
	if (Hero->m_isMax) {
		this->createSimpleBullet(-13);
		this->createSimpleBullet(13);
		Hero->LevelDown(UfoType::FIREMAX);
	}
	else if (Hero->m_fireLevel == 1) {  //һ���ӵ�
		this->createSimpleBullet(0);
	}
	else if (Hero->m_fireLevel ==2) { 	//˫���ӵ�
		this->createSimpleBullet(-13);
		this->createSimpleBullet(13);
		Hero->LevelDown(UfoType::FIREUP);
	}
	else if (Hero->m_fireLevel == 3) { 	//�����ӵ�
		this->createSimpleBullet(-15);
		this->createSimpleBullet(15);
		this->createSimpleBullet(0);
		Hero->LevelDown(UfoType::FIREUP);
	}
}

void GameScene::createSimpleBullet(float OriginX)//����һ���ӵ������뵽����
{
	auto bullet = Sprite::createWithSpriteFrameName("bullet1.png");
	if (Hero->m_isMax)
	{
		bullet->setColor(MAX_BULLET_COLOR);
		bullet->setPosition(Hero->getPositionX() + OriginX, Hero->getPositionY());
		this->addChild(bullet, FORGROUND_ZORDER);
		//���½��ӵ����뼯��
		this->m_Bullets.pushBack(bullet);
	}
	else {	
		bullet->setColor(BULLET_COLOR);
		bullet->setPosition(Hero->getPositionX() + OriginX, Hero->getPositionY());
		this->addChild(bullet, FORGROUND_ZORDER);
		//���½��ӵ����뼯��
		this->m_Bullets.pushBack(bullet);
	}
}

void GameScene::createEnemy(const EnemyType& type)//����һ���л������뵽����
{
	//auto Enemy = Sprite::createWithSpriteFrameName("enemy1.png");
	auto Enemy = Enemy::create(type);   //����ö��ʱ�õ�classö�� �������������ʱ��Ҫ ǰ��EnemyType::
	float minX = Enemy->getContentSize().width / 2;
	float maxX = VISITABLE_SIZE.width - minX;
	//�����%��max - min + 1 �� + min
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
	int X_RAND;               //���߳��ֵ������Χ
	X_RAND = rand() % (UFO_RAND_RANGE + 1) - UFO_RAND_RANGE;
	float minX = Ufo->getContentSize().width / 2 + X_RAND;
	float maxX = VISITABLE_SIZE.width - minX - X_RAND;
	float x = rand() % (int)(maxX - minX) + minX;
	Ufo->setPosition(x, VISITABLE_SIZE.height + Ufo->getContentSize().height / 2);
	switch (Ufo->getType())              //���ò�ͬ���ߵ���ɫ
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
	//ͨ��˳������ �õ�������������ȥ

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
	//����ѭ������
	auto bg1 = this->getChildByTag(BACKGROUND_TAG_1);
	auto bg2 = this->getChildByTag(BACKGROUND_TAG_2);

	bg1->setPositionY(bg1->getPositionY() - BACKGROUND_SPEED);
	bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);

	//������1������һ��ͼƬȫ�������ֱ�ӽ���һ��ͼƬ���ã����ۿ����������ڶ���ͼƬ��λ����һ��ͼƬ���Ϸ� -2��Ϊ�������ڱߣ�
	if (bg1->getPositionY() + bg1->getContentSize().height <= 0)
	{
		bg1->setPositionY(0);

	}
	/*����2
	cyclePicture(1);
	cyclePicture(2);*/
	//------------------------------------------------------------------------
}
//��ѭ��ͼƬ����2��ֻ�贫��ͼƬID
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
	1,��ը����Ϊ0ʱ���˵���ͱ�ǩ������ʾ
	2.��ը����Ϊ1ʱ��ֻ��ʾ�˵���
	3.��ը��������1ʱ����ʾ�˵���ͱ�ǩ���Ҹ��±�ǩ��ʾ����
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
	//1.ִ�б�ը����
	for (auto node : this->getChildren())
	{
		node->stopAllActions();
	}
	//b.��Ӷ���֡(�Ӿ���֡�����и������ֲ��ҳ�����)
	auto ani_herodie = AnimationCache::getInstance()->getAnimation(HERO_DIE_ANIMATION);
	//seq�з�˳���� �������Ҫ��null �� nullptr �Ա�ʾ�����
	auto seq = Sequence::create(Animate::create(ani_herodie), CallFunc::create([this]() {  //���lambada���ʽ�� node��ʾ�ľ���Enemy������Ҫִ�е�node��
		//4.��ת����
		auto scene = OverScene::createScene(this->m_totalScore);
		Director::getInstance()->replaceScene(TransitionFade::create(1, scene));																	 //this->removeChild(node);        //callfunN��Ҫ������ callfun����Ҫ����
	}), nullptr);

	Hero->runAction(seq);
	//3.ֹͣ���м�ʱ��
	this->unscheduleAllSelectors();
	
	
}