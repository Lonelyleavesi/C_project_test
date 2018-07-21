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
	/*�ӻ�����ȡ�ö���*/
	auto  ani = AnimationCache::getInstance()->getAnimation(HERO_FLY_ANIMATION);
	//2.��������װΪ����
	auto animator = Animate::create(ani);

	//3.�������ж���
	this->runAction(animator);
	return true;
}

Hero* Hero::createHero( ) {
	auto hero = new Hero();
	if (hero && hero->initWithFrame( ))
	{
		hero->autorelease(); //�����Զ�������  
		return hero;
	}

	delete hero;
	hero = nullptr;
	return nullptr;
}

void Hero::Move() {
	//��Ӵ����¼����� (�ɻ��ƶ����¼�)
	//1,����һ���¼�����
	auto lintener = EventListenerTouchOneByOne::create();   //Touch��ʾ���� allatonceֻ����һ��  oneByone������ 
	//onTouchBegin������ʼ ���ַ���ֵ��ֻ�п�ʼ�ɹ��� ���к����¼���
	// onTouchMoved�����ƶ�ʱ onTouchEnded��������ʱ  onTouchCancelled����������ʱ����ʱ
	//2.�ֽ��¼��������߼�
	//a.������ʼʱ
	//lambada���ʽ��[]���ֿ��ƶ��ⲿ�����ķ��ʣ�����һ��һ���Ĵ���
	//Ҳ����дһ��[=]����ʾ�ⲿ���б���ֵ���ݽ��������Է��ʣ������ܽ����޸�
	//������дһ��[&]��ַ������ʾ�ⲿ���б����������ô��ݽ������������Է��ʻ����޸�

	lintener->onTouchBegan = [=](Touch* touch, Event* event) {     //"�����ڲ����庯��"  С���ű�ʾ�����б� �����ű�ʾ����ֵ
		Vec2 touchPos = touch->getLocation();					//[]���Բ��������� �Ӷ�ʹ�ô������п���ʹ��[]�еĲ���
		log("Touch begin...");
		log("touch position is x: %f , y: %f", touchPos.x, touchPos.y);

		this->m_offset = this->getPosition() - touchPos;  //����ɻ�˲�Ʒ���1 ͨ�������λ��ȡ�ú�heroê���������
		bool isContains = this->getBoundingBox().containsPoint(touchPos);   //�жϴ��������Ƿ��ڷɻ���
		//ȡ�þ���ĵ��"����"  �жϲ����ĵ��Ƿ��ھ��������
		//hero->setPosition(touchPos);
		//auto move = MoveTo::create(2.0f, touchPos); //moveby����� moveto �Ǿ���
		//hero->runAction(move);
		return isContains;
	};
	//b.�����������ƶ�ʱ
	lintener->onTouchMoved = [=](Touch* touch, Event* event) {
		Vec2 touchPos = touch->getLocation();
		//Vec2 deltapos = touch->getDelta();   //��һ�δ���������һ�δ�����֮���������
		this->setPosition(touch->getLocation() + m_offset);

		auto minX = this->getContentSize().width / 2;
		this->setPositionX(MAX(this->getPositionX(), 0));
		this->setPositionX(MIN(VISITABLE_SIZE.width - 0, this->getPositionX()));

		auto minY = this->getContentSize().height / 2;
		this->setPositionY(MAX(this->getPositionY(), 0));
		this->setPositionY(MIN(VISITABLE_SIZE.height - 2 * minY, this->getPositionY()));

		//hero->setPosition(hero->getPosition() + deltapos); //�ɻ�ÿ���ƶ����������ڵ�ǰλ�ü�������ƶ�������
		//log("TouchMoved...");
	};
	//c.��������ʱ
	lintener->onTouchEnded = [](Touch* touch, Event* event) {
		log("TouchEnded...");
	};
	//3.ע��������ַ�����
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(lintener, this);
	Listener = lintener;
	//�õ��¼��ַ���          ͨ�������滭���ȼ����ڵ����ȼ������ ��󻭵����ȵõ��¼�   
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
							if (m_maxBulletsCount >= 0) {   //������д�����ӵ�

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