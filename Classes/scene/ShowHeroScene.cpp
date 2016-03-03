#include "ShowHeroScene.h"
#include "GameDefine.h"
#include "./widget/HeroListView.h"
#include "GameLogic.h"
#include "CreateOrJoinScene.h"
#include "GameDefine.h"
#include "MainScene.h"
#include "GameLogic.h"
#include "NetworkModule.h"
USING_NS_CC;

#include "ShowHeroScene.h"
#include "HeroManager.h"

USING_NS_CC;





ShowHeroScene::ShowHeroScene()
{
	m_bJoinGame = false;
	m_bIsCreator = false;

	m_nCurrentEnemyID = INVALID_ID;
	m_nCurrentHeroID	= INVALID_ID;

#ifdef _TEST_
	Util::Log("%s","测试");
	m_nCurrentHeroID = HeroManager::GetHeroIDByName(GlobalValue::current_hero_name.c_str());
	m_nCurrentEnemyID = HeroManager::GetHeroIDByName(GlobalValue::current_enemy_name.c_str());
#endif
}

ShowHeroScene::~ShowHeroScene()
{

}

Scene* ShowHeroScene::createScene()
{
	auto scene = Scene::create();

	auto layer = ShowHeroScene::create();

	scene->addChild(layer);

	return scene;
}

bool ShowHeroScene::init()
{

	if ( !Layer::init() )
	{
		return false;
	}

	//LoadBackGround();

	CreateMenu();

	if (/*!GameLogic::GetInstance()->IsSingleGame()  && !GameLogic::GetInstance()->IsCreator()*/GameLogic::GetInstance()->GetGameType() == JOIN_GAME_TYPE)
	{
		GameLogic::GetInstance()->GetNetworkModule()->RequestAllGameInfo();
		CreateGameRoundMenu(GameLogic::GetInstance()->GetNetworkModule()->GetGameRoundList());
	}

	m_pHeroListView = HeroListView::create();
	m_pHeroListView->InitView(Point(150,50),Size(120,120),HeroManager::GetHeroDefineCount(),true,false);
	addChild(m_pHeroListView);

	// 创建计时器
	this->scheduleUpdate();

	return true;
}


void ShowHeroScene::CreateMenu()
{
	auto strings = FileUtils::getInstance()->getValueMapFromFile("fonts/menu.xml"); 
	std::string strBeginGame  = strings["BeginGame"].asString(); 
	std::string strCreateGame = strings["CreateGame"].asString(); 
	std::string strJoinGame = strings["JoinGame"].asString(); 
	std::string strExitGame = strings["ExitGame"].asString();  

	if (/*GameLogic::GetInstance()->IsSingleGame()*/GameLogic::GetInstance()->GetGameType() == SINGLE_GAME_TYPE)
	{
		MenuItemLabel* beginGameItem = MenuItemLabel::create(LabelTTF::create(strBeginGame, "Arial", 40), 
			[&](Ref* sender)
		{
			BeginGame();
		}
		);
		beginGameItem->setPosition(Point(500,100));
		auto menu = Menu::create(beginGameItem,NULL);
		menu->setPosition(Point(0, 0));
		addChild(menu);
	}
	else
	{
		if (/*GameLogic::GetInstance()->IsCreator()*/GameLogic::GetInstance()->GetGameType() == CREATE_GAME_TYPE)
		{
			MenuItemLabel* createGameItem = MenuItemLabel::create(LabelTTF::create(strCreateGame, "Arial", 40), 
				[&](Ref* sender)
			{
				CreateGame();
			}
			);
			createGameItem->setPosition(Point(500,100));
			auto menu = Menu::create(createGameItem,NULL);
			menu->setPosition(Point(0, 0));
			addChild(menu);
		}
		else
		{
			MenuItemLabel* joinGameItem = MenuItemLabel::create(LabelTTF::create(strJoinGame, "Arial", 40), 
				[&](Ref* sender)
			{
				JoinGame();
			}
			);
			joinGameItem->setPosition(Point(500,100));
			auto menu = Menu::create(joinGameItem,NULL);
			menu->setPosition(Point(0, 0));
			addChild(menu);
		}
	}
}


void ShowHeroScene::LoadBackGround()
{
	auto sprite = Sprite::create("LFR/other/SelectHeroScenebg.png");
	sprite->setAnchorPoint(Point(0,0));
	sprite->setPosition(Point(0,0));
	this->addChild(sprite, 0);
}

void ShowHeroScene::BeginGame()
{
	m_nCurrentHeroID = (HERO_ID)m_pHeroListView->GetHeroID();
	if (m_nCurrentHeroID == INVALID_ID)
	{
		return;
	}
	
	GameLogic::GetInstance()->SetGameType(SINGLE_GAME_TYPE);
	HeroManager::GetInstance()->SetCurrentHero(m_nCurrentHeroID);
	HeroManager::GetInstance()->SetCurrentEnemy(m_nCurrentEnemyID);

	GameLogic::GetInstance()->JumpToScene(MAIN_SCENE);
}

void ShowHeroScene::CreateGame()
{

	bool bOK;

	m_nCurrentHeroID = (HERO_ID)m_pHeroListView->GetHeroID();

	if (m_nCurrentHeroID == INVALID_ID)
	{
		return;
	}
	

	GameLogic::GetInstance()->SetGameType(CREATE_GAME_TYPE);

	bOK = GameLogic::GetInstance()->GetNetworkModule()->CreateGame("MyGame",m_nCurrentHeroID);

	if (!bOK)
	{
		Util::Log("%s","创建游戏失败");
		exit(0);
	}

	HeroManager::GetInstance()->SetCurrentHero(m_nCurrentHeroID);
	
	m_bIsCreator = true;
}

void ShowHeroScene::JoinGame()
{

	bool bOK ;

	m_nCurrentHeroID = (HERO_ID)m_pHeroListView->GetHeroID();

	if (m_nCurrentHeroID == INVALID_ID)
	{
		return;
	}

	GameLogic::GetInstance()->SetGameType(JOIN_GAME_TYPE);
	
	GameRoundInfo* pRound = GameLogic::GetInstance()->GetNetworkModule()->GetFirstGame();

	if (pRound == 0)
	{
		Util::Log("%s","获取游戏信息失败");
		exit(0);
	}


	bOK = GameLogic::GetInstance()->GetNetworkModule()->JoinGame(pRound->m_nGameIndex,m_nCurrentHeroID);
	if (!bOK)
	{
		Util::Log("%s","加入游戏失败");
		exit(0);
	}
	
	HeroManager::GetInstance()->SetCurrentHero(m_nCurrentHeroID);
	HeroManager::GetInstance()->SetCurrentEnemy((HERO_ID)pRound->m_nCreateHeroID);

	m_bJoinGame = true;
}

void ShowHeroScene::SettingGame()
{

}

void ShowHeroScene::ExitGame()
{
	Director::getInstance()->end();
}

void ShowHeroScene::update(float delta)
{
	if (m_bIsCreator)
	{
		ControlPackage pack;
		GameLogic::GetInstance()->GetNetworkModule()->ReadControlPack(&pack,true);

		HeroManager::GetInstance()->SetCurrentEnemy((HERO_ID)pack.GetUserHeroID());
		if (pack.GetPackageCode() == code_join_game)
		{
			GameLogic::GetInstance()->JumpToScene(MAIN_SCENE);
		}
	}
	else
	{
		if (m_bJoinGame)
		{
			GameLogic::GetInstance()->JumpToScene(MAIN_SCENE);
		}
	}
}

void ShowHeroScene::CreateGameRoundMenu(vector<GameRoundInfo>& gameRounfInfo)
{
	

	for (int i = 0 ; i < gameRounfInfo.size(); ++i)
	{
		std::string strInfo = "GameIndex:";
		strInfo += Util::ConvertToStr(gameRounfInfo[i].m_nGameIndex);
		strInfo += "   PeerID:";
		strInfo+= Util::ConvertToStr(gameRounfInfo[i].m_nCreateHeroID);
		MenuItemLabel* joinGameItem = MenuItemLabel::create(LabelTTF::create(strInfo, "Arial", 40), 
			[&](Ref* sender)
		{
			//JoinGame();
		}
		);
		joinGameItem->setPosition(Point(500,150+5*i));
		auto menu = Menu::create(joinGameItem,NULL);
		menu->setPosition(Point(0, 0));
		addChild(menu);
	}
	
}