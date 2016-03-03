#include "ChooseGameScene.h"
#include "MainScene.h"
#include "GameLogic.h"
#include "LoginGameScene.h"
#include <cocos2d.h>
#include <cocos-ext.h>
#include "ui\CocosGUI.h"
#include "HeroManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

ChooseGameScene::ChooseGameScene()
{}

ChooseGameScene::~ChooseGameScene()
{}

Scene *ChooseGameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = ChooseGameScene::create();

	scene->addChild(layer);
	return scene;
}

bool ChooseGameScene::init()
{
	if(!Layer::init())
	{
		return false;
	}//if

	this->visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	/*
	 * 设置游戏模式选择背景
   	 */
	addBackGroundSprite();
	/*
	 * 创建游戏模式选择菜单
	 */
	addChooseGameMenu();
	return true;
}

void ChooseGameScene::addBackGroundSprite()
{
	cocos2d::ui::Scale9Sprite *backGroundSprite = cocos2d::ui::Scale9Sprite::create("LFR/login/chooseGameImage.png");
	backGroundSprite->setPreferredSize(visibleSize);
	backGroundSprite->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2));
	this->addChild(backGroundSprite,0);
}

void ChooseGameScene::addChooseGameMenu()
{
	//添加单击游戏菜单的press效果图片,所有按钮公用
	cocos2d::ui::Scale9Sprite *btnPress = cocos2d::ui::Scale9Sprite::create("LFR/login/menuImage_p.png");
	
	auto strings = FileUtils::getInstance()->getValueMapFromFile("fonts/chooseGameInfo.xml"); 
	std::string singleGameName = strings["SINGLE_GAME"].asString();
	std::string netGameName = strings["NET_GAME"].asString();
	std::string helpGameName = strings["HELP_GAME"].asString();
	std::string quitGameName = strings["QUIT_GAME"].asString();
	
	//添加单机游戏菜单
	cocos2d::ui::Scale9Sprite *singleGameMenu = cocos2d::ui::Scale9Sprite::create("LFR/login/menuImage.png");
	LabelTTF *singleGameLabel = LabelTTF::create(singleGameName, FONT_MENU, MENU_SIZE);
	cocos2d::extension::ControlButton *singleGameBtn = cocos2d::extension::ControlButton::create(singleGameLabel , singleGameMenu);
	Size btn_Size  = Size(singleGameBtn->getContentSize().width * 1.5 , singleGameBtn->getContentSize().height * 1.5);
	//设置菜单按下的效果
	singleGameBtn->setBackgroundSpriteForState(btnPress,Control::State::SELECTED);
	singleGameBtn->setPreferredSize(btn_Size);
	singleGameBtn->setPosition(Vec2(visibleSize.width/2 - 300 , visibleSize.height/2 + 200));
	//设置Tag
	singleGameBtn->setTag(Btn_Single_Game_TAG);
	//设置点击回调方法
	singleGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(ChooseGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	this->addChild(singleGameBtn , 1);

	//添加网络游戏菜单
	cocos2d::ui::Scale9Sprite *netGameMenu = cocos2d::ui::Scale9Sprite::create("LFR/login/menuImage.png");
	LabelTTF *netGameLabel = LabelTTF::create(netGameName, FONT_MENU, MENU_SIZE);
	cocos2d::extension::ControlButton *netGameBtn = cocos2d::extension::ControlButton::create(netGameLabel , netGameMenu);
	//添加单击游戏菜单的press效果图片
	netGameBtn->setPreferredSize(btn_Size);
	netGameBtn->setPosition(Vec2(visibleSize.width/2 - 300 , visibleSize.height/2 + 100));
	//设置Tag
	netGameBtn->setTag(Btn_Net_Game_TAG);
	//设置点击回调方法
	netGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(ChooseGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	this->addChild(netGameBtn , 2);


	//添加游戏帮助菜单
	cocos2d::ui::Scale9Sprite *helpGameMenu = cocos2d::ui::Scale9Sprite::create("LFR/login/menuImage.png");
	LabelTTF *helpGameLabel = LabelTTF::create(helpGameName, FONT_MENU, MENU_SIZE);
	cocos2d::extension::ControlButton *helpGameBtn = cocos2d::extension::ControlButton::create(helpGameLabel , helpGameMenu);
	////添加单击游戏菜单的press效果图片
	helpGameBtn->setPreferredSize(btn_Size);
	helpGameBtn->setPosition(Vec2(visibleSize.width/2 - 300 , visibleSize.height/2));
	//设置Tag
	helpGameBtn->setTag(Btn_Help_Game_TAG);
	//设置点击回调方法
	helpGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(ChooseGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	this->addChild(helpGameBtn , 3);

	//添加退出游戏菜单
	cocos2d::ui::Scale9Sprite *quitGameMenu = cocos2d::ui::Scale9Sprite::create("LFR/login/menuImage.png");
	LabelTTF *quitGameLabel = LabelTTF::create(quitGameName, FONT_MENU, MENU_SIZE);
	cocos2d::extension::ControlButton *quitGameBtn = cocos2d::extension::ControlButton::create(quitGameLabel , quitGameMenu);
	////添加单击游戏菜单的press效果图片
	quitGameBtn->setPreferredSize(btn_Size);
	quitGameBtn->setPosition(Vec2(visibleSize.width/2 - 300 , visibleSize.height/2 - 100));
	//设置Tag
	quitGameBtn->setTag(Btn_Quit_Game_TAG);
	//设置点击回调方法
	quitGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(ChooseGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	this->addChild(quitGameBtn , 4);

}

void ChooseGameScene::menuPressHandle(Object *pScender , Control::EventType event)
{
	cocos2d::extension::ControlButton *btn = (cocos2d::extension::ControlButton *)pScender;
	int tag = btn->getTag();
	
	log(tag);

	switch (tag)
	{
		case Btn_Single_Game_TAG:
		{
			log("here is running!");
			HeroManager::GetInstance()->SetCurrentHero(BANDIT_ID);
			HeroManager::GetInstance()->SetCurrentEnemy(RUDOLF_ID);
			GameLogic::GetInstance()->SetGameType(SINGLE_GAME_TYPE);
			/*auto scene = MainScene::createScene();
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(1,scene));
			log("SingleGame Pressed");*/
			GameLogic::GetInstance()->JumpToScene(SHOW_HERO_SCENE);
			break;
		}
		case Btn_Net_Game_TAG:
		{
			auto scene = LoginGameScene::createScene();
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(1,scene));
			log("NetGame pressed");
			break;
		}
		case Btn_Help_Game_TAG:
		{
			log("HelpGame pressed");
			break;
		}
		case Btn_Quit_Game_TAG:
		{
			log("QuitGame pressed");
			Director::getInstance()->end();
			break;
		}
		default:
			log("non Button pressed");
			break;
	}//switch
}