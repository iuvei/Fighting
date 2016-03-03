#include "CreateOrJoinScene.h"
#include "GameDefine.h"
#include "MainScene.h"
#include "GameLogic.h"
#include "NetworkModule.h"
USING_NS_CC;

#include "ShowHeroScene.h"

CreateOrJoinScene::CreateOrJoinScene()
{
}

CreateOrJoinScene::~CreateOrJoinScene()
{

}

Scene* CreateOrJoinScene::createScene()
{
	auto scene = Scene::create();

	auto layer = CreateOrJoinScene::create();

	scene->addChild(layer);

	return scene;
}

bool CreateOrJoinScene::init()
{

	if ( !Layer::init() )
	{
		return false;
	}

	CreateMenu();


	return true;
}

void CreateOrJoinScene::LoadBackGround()
{
	auto sprite = Sprite::create("LFR/other/SelectHeroScenebg.png");
	sprite->setAnchorPoint(Point(0,0));
	sprite->setPosition(Point(0,0));
	this->addChild(sprite, 0);
}

void CreateOrJoinScene::CreateMenu()
{
	auto strings = FileUtils::getInstance()->getValueMapFromFile("fonts/menu.xml"); 
	std::string strCreateGame  = strings["CreateGame"].asString(); 
	std::string strJoinGame = strings["JoinGame"].asString(); 
	std::string strExitGame = strings["ExitGame"].asString();  

	//创建游戏的菜单选项
	MenuItemLabel* createGameItem = MenuItemLabel::create(LabelTTF::create(strCreateGame, "Arial", 40), 
		[&](Ref* sender)
	{
		CreateGame();
	}
	);

	//加入游戏的菜单选项
	MenuItemLabel* joinGameItem = MenuItemLabel::create(LabelTTF::create(strJoinGame, "Arial", 40), 
		[&](Ref* sender)
	{
		JoinGame();
	}
	);

	//退出游戏的菜单选项
	MenuItemLabel* exitGameItem = MenuItemLabel::create(LabelTTF::create(strExitGame, "Arial", 40), 
		[&](Ref* sender)
	{
		ExitGame();
	}
	);

	createGameItem->setPosition(Point(200,450));
	joinGameItem->setPosition(Point(200,350));
	exitGameItem->setPosition(Point(200,250));

	auto menu = Menu::create(createGameItem,joinGameItem,exitGameItem,NULL);
	menu->setPosition(Point(0, 0));
	addChild(menu);
}

void CreateOrJoinScene::jumpToMainScene(float t)
{
	auto scene = MainScene::createScene();
	Director::getInstance()->replaceScene(TransitionProgressOutIn::create(0.5,scene));
}

//用户点击创建一款游戏的处理函数
void CreateOrJoinScene::CreateGame()
{
	GameLogic::GetInstance()->SetGameType(CREATE_GAME_TYPE);
	GameLogic::GetInstance()->JumpToScene(SHOW_HERO_SCENE);//CREAT_GAME_SCENE
}

void CreateOrJoinScene::JoinGame()
{
	GameLogic::GetInstance()->SetGameType(JOIN_GAME_TYPE);

	GameLogic::GetInstance()->JumpToScene(SHOW_HERO_SCENE);//SHOW_HERO_SCENE
}


void CreateOrJoinScene::ExitGame()
{
	Director::getInstance()->end();
}

