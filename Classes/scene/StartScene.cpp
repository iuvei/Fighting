#include "StartScene.h"
#include "GameLogic.h"

USING_NS_CC;



Scene* StartScene::createScene()
{
	auto scene = Scene::create();

	auto layer = StartScene::create();

	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{

	if ( !Layer::init() )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("LFR/other/background.png");
	sprite->setAnchorPoint(Point(0,0));
	sprite->setPosition(Point(0,0));
	this->addChild(sprite, 0);

	auto label = Label::createWithTTF("Fighting", "fonts/Marker Felt.ttf", 48);

	label->setPosition(Vec2(origin.x + visibleSize.width/2,
		320 ));

	label->setScale(2.0);

	this->addChild(label, 1);

	this->scheduleOnce(schedule_selector(StartScene::jumpToMainScene),1);

	return true;
}


void StartScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


/************************************************************************
*   0.5秒后调到主场景                                                                  
************************************************************************/
void StartScene::jumpToMainScene(float t)
{
	GameLogic::GetInstance()->JumpToScene(CHOOSEGAME_SCENE);
}