#include"OverScene.h"
#include"LoadingScene.h"
#include"GameScene.h"
#include"Constants.h"

Scene*	OverScene::createScene(int score)
{
	auto scene = new OverScene();
	if (scene && scene->init(score))
	{
		scene->autorelease();
		return scene;
	}
	delete scene;
	scene = nullptr;
	return nullptr;
}

bool OverScene::init(int score) {
	if (!Scene::init())
	{
		return FALSE;
	}
	//����ͼƬ
	auto bg = Sprite::createWithSpriteFrameName("gameover.png");
	bg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	bg->setPosition(0 , VISITABLE_SIZE.height);
	this->addChild(bg, BACKGROUND_ZORDER);
	
	//������ʷ���
	int highScore = UserDefault::getInstance()->getIntegerForKey(HIGH_SCORE_KEY,0);
	auto lblhighScore = Label::createWithBMFont("font.fnt", StringUtils::format("%d", highScore));
	lblhighScore->setPosition(VISITABLE_ORIGIN + Vec2(VISITABLE_SIZE.width / 2, VISITABLE_SIZE.height / 6 * 5));
	this->addChild(lblhighScore, UI_ZORDER);
	lblhighScore->setColor(Color3B::BLACK);
	lblhighScore->setBMFontSize(68);
	if (highScore < score)  //�����ǰ�ִ�����߷�
	{
		highScore = score;
		UserDefault::getInstance()->setIntegerForKey(HIGH_SCORE_KEY, highScore);
		auto seq = Sequence::create(
			ScaleTo::create(1, 5),	
			CallFuncN::create([highScore](Node *node) {
			dynamic_cast<Label*>(node)->setString(StringUtils::format("%d", highScore));
			}),
			ScaleTo::create(1,1),
			nullptr);
		lblhighScore->runAction(seq);
	}



	//���ճɼ�������ǩ
	auto lblScore = Label::createWithBMFont("font.fnt", StringUtils::format("%d", score));
	lblScore->setPosition(VISITABLE_ORIGIN + Vec2(VISITABLE_SIZE.width / 2, VISITABLE_SIZE.height / 3));
	this->addChild(lblScore, UI_ZORDER);
	lblScore->setColor(Color3B::BLACK);
	lblScore->setBMFontSize(68);

	this->reStartMnue();
	return true;
}

void OverScene::reStartMnue() {
	auto btn_reStart = Sprite::createWithSpriteFrameName("btn_finish.png");     //����һ������
	auto itemreStart = MenuItemSprite::create(btn_reStart, btn_reStart,[this](Ref*) {    //ͨ�����鴴���¼�
		auto scene = GameScene::createScene( );
		Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
	});
	itemreStart->setPosition(VISITABLE_SIZE.width - itemreStart->getContentSize().width/2, itemreStart->getContentSize().height / 2);
	auto menu = Menu::create();
	menu->addChild(itemreStart, UI_ZORDER, ITEM_RESTART_TAG);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, UI_ZORDER, REST_MENU_TAG);
}