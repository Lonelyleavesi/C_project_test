#include"Hero.h"

bool Hero::initWithFrame( )
{
	std::string frameName = "hero1.png";
	this->m_hp = 1;
	this->m_fireLevel = 1;
	this->m_hurtLevel = 1;
	this->m_doubleBulletsCount = 0;
	this->m_ThreeBulletsCount = 0;
	this->m_maxBulletsCount = 0;
	this->m_isMax = FALSE;
	this->m_Bullet_seq = CREATE_BULLET_INERVAL;
	if (!Sprite::initWithSpriteFrameName(frameName))
	{
		return false;
	}
	/*从缓存中取得动画*/
	auto  ani = AnimationCache::getInstance()->getAnimation(HERO_FLY_ANIMATION);
	//2.将动画封装为动作
	auto animator = Animate::create(ani);

	//3.精灵运行动作
	this->runAction(animator);
	return true;
}

Hero* Hero::createHero( ) {
	auto hero = new Hero();
	if (hero && hero->initWithFrame( ))
	{
		hero->autorelease(); //放入自动计数池  
		return hero;
	}

	delete hero;
	hero = nullptr;
	return nullptr;
}

void Hero::Move() {
	//添加触摸事件处理 (飞机移动的事件)
	//1,创建一个事件监听
	auto lintener = EventListenerTouchOneByOne::create();   //Touch表示触摸 allatonce只处理一次  oneByone处理多次 
	//onTouchBegin触摸开始 （又返回值，只有开始成功了 才有后续事件）
	// onTouchMoved触摸移动时 onTouchEnded触摸结束时  onTouchCancelled触摸被其他时间打断时
	//2.分解事件，处理逻辑
	//a.触摸开始时
	//lambada表达式的[]部分控制对外部变量的访问，可以一个一个的传递
	//也可以写一个[=]，表示外部所有变量值传递进来，可以访问，但不能进行修改
	//还可以写一个[&]地址符，表示外部所有变量都按引用传递进来，不仅可以访问还能修改

	lintener->onTouchBegan = [=](Touch* touch, Event* event) {     //"函数内部定义函数"  小括号表示参数列表 大括号表示返回值
		Vec2 touchPos = touch->getLocation();					//[]可以捕获外界参数 从而使得大括号中可以使用[]中的参数
		log("Touch begin...");
		log("touch position is x: %f , y: %f", touchPos.x, touchPos.y);

		this->m_offset = this->getPosition() - touchPos;  //解决飞机瞬移方法1 通过鼠标点击位置取得和hero锚点的向量差
		bool isContains = this->getBoundingBox().containsPoint(touchPos);   //判断触摸区域是否在飞机上
		//取得精灵的点的"矩阵"  判断参数的点是否在精灵点里面
		//hero->setPosition(touchPos);
		//auto move = MoveTo::create(2.0f, touchPos); //moveby是相对 moveto 是绝对
		//hero->runAction(move);
		return isContains;
	};
	//b.持续触摸并移动时
	lintener->onTouchMoved = [=](Touch* touch, Event* event) {
		Vec2 touchPos = touch->getLocation();
		//Vec2 deltapos = touch->getDelta();   //上一次触摸点与这一次触摸点之间的向量差
		this->setPosition(touch->getLocation() + m_offset);

		auto minX = this->getContentSize().width / 2;
		this->setPositionX(MAX(this->getPositionX(), 0));
		this->setPositionX(MIN(VISITABLE_SIZE.width - 0, this->getPositionX()));

		auto minY = this->getContentSize().height / 2;
		this->setPositionY(MAX(this->getPositionY(), 0));
		this->setPositionY(MIN(VISITABLE_SIZE.height - 2 * minY, this->getPositionY()));

		//hero->setPosition(hero->getPosition() + deltapos); //飞机每次移动的向量等于当前位置加上鼠标移动的向量
		//log("TouchMoved...");
	};
	//c.触摸结束时
	lintener->onTouchEnded = [](Touch* touch, Event* event) {
		log("TouchEnded...");
	};
	//3.注册监听到分发器上
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(lintener, this);
	Listener = lintener;
	//得到事件分发器          通过场景绘画优先级（遮挡优先级）添加 最后画的最先得到事件   
}

void Hero::LevelUP(UfoType type) {
	switch (type)
	{
	case UfoType::FIREUP:  	if (m_fireLevel == 3) { this->m_ThreeBulletsCount += NUM_DOUBLEBULLET; };
							if (m_fireLevel == 2) {
							this->m_fireLevel ++;
							this->m_ThreeBulletsCount = NUM_DOUBLEBULLET + m_doubleBulletsCount;
							}
							if (m_fireLevel == 1) {
							this->m_fireLevel ++;
							this->m_doubleBulletsCount = NUM_DOUBLEBULLET;
							}

		break;
	case UfoType::BOMB:  if (m_bomb < 3)this->m_bomb = m_bomb + 1;
		break;
	case UfoType::FIREMAX: {
		m_maxBulletsCount += NUM_MAXBULLET;
		m_isMax = TRUE;
		m_Bullet_seq = MAX_BULLET_INERVAL;
	}
	default:
		break;
	}
}

void Hero::LevelDown(UfoType type) {
	switch (type)
	{
	case UfoType::FIREUP: {
							if (m_maxBulletsCount >= 0) {   //如果还有大火力子弹

								this->LevelDown(UfoType::FIREMAX);
							}
							if (m_ThreeBulletsCount >= 0)
							{
								this->m_ThreeBulletsCount  --;
							}
							else if (m_doubleBulletsCount >= 0)
							{
								this->m_doubleBulletsCount --;
							}
							else
							{
							  this->m_fireLevel = 1;
								 }
							break;
						   }
	case UfoType::BOMB:  this->m_bomb = m_bomb - 1;
								break;
	case UfoType::FIREMAX:  if (m_maxBulletsCount >= 0)
							{
								this->m_maxBulletsCount--;
							}
						   else {
							   this->m_isMax = FALSE;
							   this->m_Bullet_seq = CREATE_BULLET_INERVAL;
							   this->LevelDown(UfoType::FIREUP);
						   }

						   break;
	default:
		break;
	}
}

void Hero::TurnListener()
{
	if (Listener->isEnabled())
		Listener->setEnabled(FALSE);
	else
		Listener->setEnabled(TRUE);
};