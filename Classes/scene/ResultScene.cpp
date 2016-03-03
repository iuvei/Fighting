#include "ResultScene.h"
#include "GameDefine.h"
#include "GameLogic.h"
#include "Hero.h"
#include "HeroManager.h"

USING_NS_CC;



Scene* ResultScene::createScene()
{
	auto scene = Scene::create();

	auto layer = ResultScene::create();

	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool ResultScene::init()
{

	if ( !Layer::init() )
	{
		return false;
	}


	/*Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto strings = FileUtils::getInstance()->getValueMapFromFile("fonts/game_result.xml"); 
	std::string strVictory  = strings["VICTORY"].asString(); 
	std::string strFailure = strings["FAILURE"].asString(); 

	Label* label = 0;

	if (HeroManager::GetInstance()->GetCurrentEnemy()->GetHeroState() == DEATH_STATE)
	{
		label = Label::createWithTTF(strVictory, "fonts/Marker Felt.ttf", 48);
	}
	else
	{
		label = Label::createWithTTF(strFailure, "fonts/Marker Felt.ttf", 48);
	}

	label->setPosition(Vec2(origin.x + visibleSize.width/2,
		320 ));

	label->setScale(2.0);

	this->addChild(label, 1);*/


	CreateMenu();

	return true;
}



void ResultScene::CreateMenu()
{
	auto strings = FileUtils::getInstance()->getValueMapFromFile("fonts/strings.xml"); 
	std::string strRestartGame  = strings["RestartGame"].asString(); 
	std::string strExitGame = strings["ExitGame"].asString();  

	MenuItemLabel* restartGameItem = MenuItemLabel::create(LabelTTF::create(strRestartGame, "Arial", 40), 
		[&](Ref* sender)
	{
		GameLogic::GetInstance()->JumpToScene(CHOOSEGAME_SCENE);
	}
	);


	MenuItemLabel* exitGameItem = MenuItemLabel::create(LabelTTF::create(strExitGame, "Arial", 40), 
		[&](Ref* sender)
	{
		Director::getInstance()->end();
	}
	);

	

	std::string strVictory  = strings["VICTORY"].asString(); 
	std::string strFailure = strings["FAILURE"].asString(); 

	MenuItemLabel* result =0;

	if (HeroManager::GetInstance()->GetCurrentEnemy()->GetHeroState() == DEATH_STATE)
	{

		result = MenuItemLabel::create(LabelTTF::create(strVictory, "Arial", 40), 
			[&](Ref* sender)
		{
		}
		);
	}
	else
	{
		result = MenuItemLabel::create(LabelTTF::create(strFailure, "Arial", 40), 
			[&](Ref* sender)
		{
		}
		);
	}
	restartGameItem->setPosition(Point(700,150));

	exitGameItem->setPosition(Point(200,150));

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	result->setPosition(Vec2(origin.x + visibleSize.width/2,
		320 ));

	auto menu = Menu::create(restartGameItem,exitGameItem,result,NULL);
	menu->setPosition(Point(0, 0));
	addChild(menu);
}
