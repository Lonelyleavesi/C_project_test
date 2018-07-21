#include"Enemy.h"
#include"Constants.h"
bool Enemy::initWithSpriteFrameType(const EnemyType& type) {
	//用成员变量保存类型，以便记住之后可以随时访问
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
	//这里不要调用成 creatWithSpriteFrameName!!
	if (!Sprite::initWithSpriteFrameName(frameName))
	{
		return false;
	}
	//大飞机动画
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
		enemy->autorelease(); //放入自动计数池  
		return enemy;
	}

	delete enemy;
	enemy = nullptr;
	return nullptr;
}

void Enemy::down() {
	// 1.创建动画
	//a.创建动画对象
	auto ani = Animation::create();
	//b.添加动画帧(从精灵帧缓存中根据名字查找出来的)
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

	
	//2.将动画封装为动作
	//(通过顺序动作)
	auto animate = Animate::create(ani);
	//seq中放顺序动作 参数最后要加null 或 nullptr 以表示最后动作
	//方法1 通过removeChild 清除Node（enemy）
	//auto seq = Sequence::create(animate, CallFuncN::create([this](Node *node) {  //这个lambada表达式中 node表示的就是Enemy（后面要执行的node）
	//	this->removeChild(node);        //callfunN需要传参数 callfun不需要参数
	//}), nullptr);

	//方法2 通过在函数中调用node的自身清除函数
	//auto seq = Sequence::create(animate, CallFuncN::create([](Node *node) {  //这个lambada表达式中 node表示的就是Enemy（后面要执行的node）
	//	node->removeFromParentAndCleanup(true);        //callfunN需要传参数 callfun不需要参数
	//}), nullptr);

	//方法3  RemoveSelf 表示通过动作自我销毁
	auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);

	//执行新动作之前停止其他动作
	this->stopAllActions();
	//3.精灵运行动作
	this->runAction(seq);
}

//敌机被击中的时候的动画
void Enemy::getinghit()
{
	auto Type = this->m_type;
	auto ani = Animation::create();
	//b.添加动画帧(从精灵帧缓存中根据名字查找出来的)

	switch (m_type)
	{								//中型飞机需要重置 大型飞机 因为自带动画 会自己重置
	case EnemyType::MIDDLE_ENEMY:	ani = AnimationCache::getInstance()->getAnimation(ENEMY2_HURT_ANIMATION);
		break;
	case EnemyType::BIG_ENEMY:		ani = AnimationCache::getInstance()->getAnimation(ENEMY3_HURT_ANIMATION);

		break;
	default:
		break;
	}
	//2.将动画封装为动作
	this->runAction(Animate::create(ani));
}

void Enemy::move() {
	this->setPositionY(this->getPositionY() + this->m_speed);
}



//受伤函数 默认受伤attack值的生命
bool Enemy::gethurt(const int& attackint) {
	//判断是否已经死亡
	this->m_hp = m_hp - attackint;
	this->getinghit();
	if (this->m_hp <= 0) {
		this->down();
		return true;
	}
	return FALSE;
	/*
	@return 减血后是否死亡
	*/
}

int Enemy::getHP() {   //为了便于将生命与关卡难度绑定 设置一个函数 与宏定义绑定
	return m_hp;
}
EnemyType Enemy::getType()
{
	return this->m_type;
}