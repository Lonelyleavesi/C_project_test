#include"Enemy.h"
#include"Constants.h"
bool Enemy::initWithSpriteFrameType(const EnemyType& type) {
	//�ó�Ա�����������ͣ��Ա��ס֮�������ʱ����
	this->m_type = type;
	std::string frameName = "enemy1.png";
	this->m_speed = SMALL_ENEMY_SPEED;
	this->m_hp = SMALL_ENEMY_HP;
	this->m_score = SMALL_ENEMY_SCORE;
	switch (m_type)
	{
	case EnemyType::MIDDLE_ENEMY:	frameName = StringUtils::format("enemy2.png");
		m_speed = MIDDLE_ENEMY_SPEED;
		m_hp = MIDDLE_ENEMY_HP;
		m_score = MIDDLE_ENEMY_SCORE;
		break;
	case EnemyType::BIG_ENEMY:		frameName = StringUtils::format("enemy3_n1.png");
		m_speed = BIG_ENEMY_SPEED;
		m_hp = BIG_ENEMY_HP;
		m_score = BIG_ENEMY_SCORE;
		break;
	default:
		break;
	}
	//���ﲻҪ���ó� creatWithSpriteFrameName!!
	if (!Sprite::initWithSpriteFrameName(frameName))
	{
		return false;
	}
	//��ɻ�����
	if (this->m_type == EnemyType::BIG_ENEMY) {

		auto bigEnemy_ani = AnimationCache::getInstance()->getAnimation(ENEMY3_FLY_ANIMATION);
		this->runAction(Animate::create(bigEnemy_ani));
	}
	return true;
}

Enemy* Enemy::create(const EnemyType& type) {
	auto enemy = new Enemy();
	if (enemy && enemy->initWithSpriteFrameType(type))
	{
		enemy->autorelease(); //�����Զ�������  
		return enemy;
	}

	delete enemy;
	enemy = nullptr;
	return nullptr;
}

void Enemy::down() {
	// 1.��������
	//a.������������
	auto ani = Animation::create();
	//b.��Ӷ���֡(�Ӿ���֡�����и������ֲ��ҳ�����)
		switch (m_type)
		{
		case EnemyType::SMALL_ENEMY:ani = AnimationCache::getInstance()->getAnimation(ENEMY1_DIE_ANIMATION);
									break;

		case EnemyType::MIDDLE_ENEMY:ani = AnimationCache::getInstance()->getAnimation(ENEMY2_DIE_ANIMATION);
									break; 

		case EnemyType::BIG_ENEMY: ani = AnimationCache::getInstance()->getAnimation(ENEMY3_DIE_ANIMATION);
									break;
		default:
			break;
		}

	
	//2.��������װΪ����
	//(ͨ��˳����)
	auto animate = Animate::create(ani);
	//seq�з�˳���� �������Ҫ��null �� nullptr �Ա�ʾ�����
	//����1 ͨ��removeChild ���Node��enemy��
	//auto seq = Sequence::create(animate, CallFuncN::create([this](Node *node) {  //���lambada���ʽ�� node��ʾ�ľ���Enemy������Ҫִ�е�node��
	//	this->removeChild(node);        //callfunN��Ҫ������ callfun����Ҫ����
	//}), nullptr);

	//����2 ͨ���ں����е���node�������������
	//auto seq = Sequence::create(animate, CallFuncN::create([](Node *node) {  //���lambada���ʽ�� node��ʾ�ľ���Enemy������Ҫִ�е�node��
	//	node->removeFromParentAndCleanup(true);        //callfunN��Ҫ������ callfun����Ҫ����
	//}), nullptr);

	//����3  RemoveSelf ��ʾͨ��������������
	auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);

	//ִ���¶���֮ǰֹͣ��������
	this->stopAllActions();
	//3.�������ж���
	this->runAction(seq);
}

//�л������е�ʱ��Ķ���
void Enemy::getinghit()
{
	auto Type = this->m_type;
	auto ani = Animation::create();
	//b.��Ӷ���֡(�Ӿ���֡�����и������ֲ��ҳ�����)

	switch (m_type)
	{								//���ͷɻ���Ҫ���� ���ͷɻ� ��Ϊ�Դ����� ���Լ�����
	case EnemyType::MIDDLE_ENEMY:	ani = AnimationCache::getInstance()->getAnimation(ENEMY2_HURT_ANIMATION);
		break;
	case EnemyType::BIG_ENEMY:		ani = AnimationCache::getInstance()->getAnimation(ENEMY3_HURT_ANIMATION);

		break;
	default:
		break;
	}
	//2.��������װΪ����
	this->runAction(Animate::create(ani));
}

void Enemy::move() {
	this->setPositionY(this->getPositionY() + this->m_speed);
}



//���˺��� Ĭ������attackֵ������
bool Enemy::gethurt(const int& attackint) {
	//�ж��Ƿ��Ѿ�����
	this->m_hp = m_hp - attackint;
	this->getinghit();
	if (this->m_hp <= 0) {
		this->down();
		return true;
	}
	return FALSE;
	/*
	@return ��Ѫ���Ƿ�����
	*/
}

int Enemy::getHP() {   //Ϊ�˱��ڽ�������ؿ��ѶȰ� ����һ������ ��궨���
	return m_hp;
}
EnemyType Enemy::getType()
{
	return this->m_type;
}