#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "./scene/ChooseGameScene.h"
#include "GameLogic.h"
#include "GameDefine.h"
#include "Hero.h"
#include "./scene/MainScene.h"


#include "../CommonClass/Util.h"


// add by mobinsheng for 添加网络功能 begin
#include "NetworkModule.h"
// add by mobinsheng for 添加网络功能 end

#include "HeroManager.h"
#include "MagicManager.h"

#include "./scene/ResultScene.h"
#include "./scene/ShowHeroScene.h"
#include "./scene/CreateOrJoinScene.h"

#include "cocos-ext.h"
USING_NS_CC_EXT;

USING_NS_CC;

#include "../CommonClass/GameRound.h"

GameLogic* GameLogic::m_pGameLogicInstance = NULL;

/************************************************************************
*  获取游戏逻辑对象
************************************************************************/
GameLogic* GameLogic::GetInstance()
{
	if (0 == m_pGameLogicInstance)
	{
		m_pGameLogicInstance = new GameLogic();
	}

	return m_pGameLogicInstance;
}

/************************************************************************
*  构造函数
************************************************************************/
GameLogic::GameLogic()
{
	m_bInit							= false;

	m_pMainScene				= 0;

	m_pNetwork					= 0;

	if (m_pNetwork == 0)
	{
		m_pNetwork = new NetworkMoudle();
	}



	m_Player						= new Player;

	m_nGameType				= INVALID_GAME_TYPE;

	m_nSuspendState			= NOT_SUSPEND;


	LoadConfig();
}

/************************************************************************
*  析构函数
************************************************************************/
GameLogic::~GameLogic()
{
	Clean();
}

void GameLogic::StartGame(MainScene* s)
{
	if (m_bInit)
	{
		return;
	}

	m_pMainScene = s;

	// 初始化英雄人物
	if(!HeroManager::GetInstance()->InitHero())
	{
		return; 
	}

	// 设置玩家的英雄的ID
	m_Player->SetHeroID(HeroManager::GetInstance()->GetCurrentHero()->GetHeroID());

	if (GetGameType() == SINGLE_GAME_TYPE)
	{
		m_Player->SetUserID("Player");
	}

	if (GetGameType() == CREATE_GAME_TYPE)
	{
		m_Player->SetIsCreator(true);
	}

	if (GetGameType() == JOIN_GAME_TYPE)
	{
		m_Player->SetIsCreator(false);
	}

	m_bInit = true;

	// 设置运行标志
	IsGameRunning(true);

	MagicManager::GetInstance()->ShowLamp();

}

/************************************************************************
*  游戏循环，由主场景的计时器周期性调用
************************************************************************/

void GameLogic::LoopGame()
{
	// 如果游戏没有运行，则不进行处理
	if (!GameLogic::GetInstance()->IsGameRunning())
	{
		return;
	}

	// 无效游戏
	if (GetGameType() == INVALID_GAME_TYPE)
	{
		return;
	}

	static int nRandomMagicInterval = 0;
	//
	++nRandomMagicInterval;
	if (nRandomMagicInterval == 500)
	{
		int xPos = Util::GetRandomNum(100,700);
		int yPos = Util::GetRandomNum(xPos,100 +xPos);
		MagicManager::GetInstance()->RandomMagic(xPos,yPos);
		MagicManager::GetInstance()->RandomMoney(xPos/2+50,yPos/2-50);
		nRandomMagicInterval = 0;
	}

	if (nRandomMagicInterval % 100 == 0)
	{
		MagicManager::GetInstance()->TowerShoot();
	}


	MagicManager::GetInstance()->UpdateMagic();

	// 魔法与英雄人物之间的碰撞检测
	MagicManager::GetInstance()->MagicCollisionDetection();

	// 单机游戏需要更新
	if (GetGameType() == SINGLE_GAME_TYPE)
	{
		HeroManager::GetInstance()->UpdateHero();
		return;
	}

	if (HeroManager::GetInstance()->GetCurrentHero() == NULL || HeroManager::GetInstance()->GetCurrentEnemy() == NULL)
	{
		return;
	}

	// 下面是联网模式下的代码

	// 英雄移动、攻击，技能攻击这三种动作，在按键按下的时候发送数据包
	if (HeroManager::GetInstance()->GetCurrentHero()->GetHeroState() == ATTACK_STATE 
		|| HeroManager::GetInstance()->GetCurrentHero()->GetHeroState() == MAGICATTACK_STATE
		|| HeroManager::GetInstance()->GetCurrentHero()->GetHeroState() == MOVE_STATE
		)
	{
	}
	else
	{
		// 其他的时候由主动发送数据包
		HeroManager::GetInstance()->SendHeroStateToServer(HeroManager::GetInstance()->GetCurrentHero());
	}

	ControlPackage ctlPack;
	DataPackage dataPack;
	PackageHead head;

	// 读取包头部
	bool bOK = GameLogic::GetInstance()->GetNetworkModule()->ReadPackageHead(head,true);

	if (bOK == false)
	{
		return;
	}

	// 如果在游戏进行中收到控制包，那么只有两种情况：
	// 1、对方暂停了游戏
	// 2、对方继续运行游戏
	// 3、对方离开了游戏
	if (head.GetPackageType() == package_control)
	{
		GameLogic::GetInstance()->GetNetworkModule()->ReadControlPackBody(head,&ctlPack);

		// 对方暂停游戏
		if (ctlPack.GetPackageCode() == code_suspend_game)
		{
			GameLogic::GetInstance()->SetSuspendState(OPPONENT_SUSPEND);
		}

		// 对方继续执行游戏
		if (ctlPack.GetPackageCode() == code_continue_game)
		{
			GameLogic::GetInstance()->SetSuspendState(NOT_SUSPEND);
		}

		// 对方离开游戏
		if (ctlPack.GetPackageCode() == code_leave)
		{
			HeroManager::GetInstance()->GetCurrentEnemy()->SetHeroState(DEATH_STATE);
			GameLogic::GetInstance()->OpponentStopGame();
			return;
		}
	}
	else if (head.GetPackageType() == package_data)
	{
		GameLogic::GetInstance()->GetNetworkModule()->ReadDataPackBody(head,&dataPack);
	}
	else
	{
		return ;
	}

	if (dataPack.m_PackageHead.GetPackageType() == package_unknown)
	{
		return;
	}

	// 对方死亡
	if (dataPack.GetState() == DEATH_STATE )
	{
		HeroManager::GetInstance()->GetCurrentEnemy()->SetBloodValue(0);
		return;
	}

	// 对方移动
	if (dataPack.GetState() == MOVE_STATE)
	{
		HeroManager::GetInstance()->HeroMove(HeroManager::GetInstance()->GetCurrentEnemy()->GetHeroID(),(tagDirecton)dataPack.GetMoveDirection());
	}
	// 对方静止
	if (dataPack.GetState() == STATIC_STATE)
	{
		HeroManager::GetInstance()->GetCurrentEnemy()->StopAnimation();
	}

	// 对方攻击
	if (dataPack.GetState() == ATTACK_STATE)
	{
		if (HeroManager::GetInstance()->GetCurrentEnemy()->GetHeroState() != ATTACK_STATE)
		{
			HeroManager::GetInstance()->GetCurrentEnemy()->StopAnimation();
		}
		HeroManager::GetInstance()->GetCurrentEnemy()->Attack();
	}

	// 对方魔法攻击
	if (dataPack.GetState() == MAGICATTACK_STATE)
	{
		if (HeroManager::GetInstance()->GetCurrentEnemy()->GetHeroState() != MAGICATTACK_STATE)
		{
			HeroManager::GetInstance()->GetCurrentEnemy()->StopAnimation();
		}

		HeroManager::GetInstance()->GetCurrentEnemy()->MagicAttack();
	}
}

/************************************************************************
*  清理函数
************************************************************************/
void GameLogic::Clean()
{
	if (GetGameType() != SINGLE_GAME_TYPE && GetNetworkModule()->IsConnect())
	{
		m_pNetwork->StopGame();
	}
	
	// 魔法管理器清理
	MagicManager::GetInstance()->Clean();

	// 英雄管理器清理
	HeroManager::GetInstance()->Clean();

	// 游戏运行标志
	m_bIsGameRunning = false;

	m_bInit = false;

	// 游类型
	m_nGameType = INVALID_GAME_TYPE;

}

// add by mobinsheng for 添加网络功能 begin
// 登陆服务器
bool GameLogic::ConnectToServer(const char* ip,int nPort)
{
	bool ret = m_pNetwork->Connect(ip,nPort);

	return ret;
}

// 获取网络模块
NetworkMoudle* GameLogic::GetNetworkModule()
{
	return m_pNetwork;
}


/************************************************************************
*  加载配置文件
************************************************************************/
void GameLogic::LoadConfig()
{
	rapidjson::Document json_file;

	char* pBytes = 0;

	ssize_t  size = 0;

	pBytes = (char*)FileUtils::sharedFileUtils()->getFileData(CONFIG_FILE,"r",&size);

	if (NULL == pBytes || strcmp(pBytes,"") == 0)
	{
		return;
	}

	string str(pBytes,size);

	//CC_SAFE_DELETE_ARRAY(pBytes);
	delete pBytes;

	json_file.Parse<0>(str.c_str());

	if (json_file.HasParseError())
	{
		return;
	}

	if (!json_file.IsObject())
	{
		return;
	}

	GlobalValue::server_ip								= json_file["server_ip"].GetString();
	GlobalValue::server_port							= json_file["server_port"].GetInt();
	GlobalValue::hero_move_step					=  json_file["hero_move_step"].GetInt();
	GlobalValue::parry_probability					=  json_file["parry_probability"].GetDouble();
	GlobalValue::magic_distance						=  json_file["magic_distance"].GetInt();
	GlobalValue::use_ai									= json_file["use_ai"].GetBool();
	GlobalValue::ai_check_distance					= json_file["ai_check_distance"].GetInt();
	GlobalValue::ai_attack_distance					= json_file["ai_attack_distance"].GetInt();
	GlobalValue::magic_move_step					= json_file["magic_move_step"].GetInt();
	GlobalValue::attack_damage						= json_file["attack_damage"].GetInt();
	GlobalValue::attack_damage_distance		= json_file["attack_damage_distance"].GetInt();
	GlobalValue::magic_damage						= json_file["magic_damage"].GetInt();
	GlobalValue::magic_damage_distance		= json_file["magic_damage_distance"].GetInt();
	GlobalValue::magic_cost							= json_file["magic_cost"].GetInt();
	GlobalValue::blood_regain_value				= json_file["blood_regain_value"].GetInt();
	GlobalValue::magic_regain_value				= json_file["magic_regain_value"].GetInt();

#ifdef _TEST_
	GlobalValue::current_hero_name				= json_file["current_hero"].GetString();
	GlobalValue::current_enemy_name			= json_file["current_enemy"].GetString();
#endif

	rapidjson::Value& ai_probability = json_file["ai_probability"];
	if (!ai_probability.IsObject())
	{
		return;
	}
	GlobalValue::nomal_ai_probability.attack_probability = ai_probability["attack_probability"].GetDouble();
	GlobalValue::nomal_ai_probability.magic_probability = ai_probability["magic_probability"].GetDouble();
	GlobalValue::nomal_ai_probability.move_probability = ai_probability["move_probability"].GetDouble();
	GlobalValue::nomal_ai_probability.static_probability = ai_probability["static_probability"].GetDouble();

	rapidjson::Value& attack_ai_probability = json_file["attack_ai_probability"];

	if (!attack_ai_probability.IsObject())
	{
		return;
	}
	GlobalValue::attack_ai_probability.attack_probability = attack_ai_probability["attack_probability"].GetDouble();
	GlobalValue::attack_ai_probability.magic_probability = attack_ai_probability["magic_probability"].GetDouble();
	GlobalValue::attack_ai_probability.move_probability = attack_ai_probability["move_probability"].GetDouble();
	GlobalValue::attack_ai_probability.static_probability = attack_ai_probability["static_probability"].GetDouble();

	return;
}

/************************************************************************
*  获取主场景（即游戏进行的场景）
************************************************************************/
MainScene* GameLogic::GetMainScene()
{
	return m_pMainScene;
}

/************************************************************************
*  设置游戏运行标志
************************************************************************/
void GameLogic::IsGameRunning(bool v)
{
	m_bIsGameRunning = v;
}

/************************************************************************
*  获取游戏运行标志
************************************************************************/
bool GameLogic::IsGameRunning()
{
	return m_bIsGameRunning;
}

/************************************************************************
*  获取游戏的暂停状态（没有暂停，自己暂停、对方暂停）
************************************************************************/
int GameLogic::GetSuspendState()
{
	return m_nSuspendState;
}

/************************************************************************
*  设置游戏的暂停状态
************************************************************************/
void GameLogic::SetSuspendState(int state)
{
	m_nSuspendState = state;
}

/************************************************************************
*  暂停游戏
* 注意不在此函数内设置暂停状态，因为不知道是对方请求暂停还是自己玩家
* 请求暂停，所以暂停的时候，暂停状态在别的地方设置
************************************************************************/
void GameLogic::SuspendGame()
{
	GetNetworkModule()->SuspendGame();
}

/************************************************************************
*  继续游戏
************************************************************************/
void GameLogic::ContinueGame()
{
	GetNetworkModule()->ContinueGame();
	m_nSuspendState = NOT_SUSPEND;
}

/************************************************************************
*  对方停止了游戏
************************************************************************/
void GameLogic::OpponentStopGame()
{
	JumpToScene(RESULT_SCENE);
	Clean();
}

/************************************************************************
*  自己玩家停止了游戏
************************************************************************/
void GameLogic::StopGame()
{
	JumpToScene(RESULT_SCENE);
	Clean();
}


/************************************************************************
*  获取玩家对象
************************************************************************/
Player* GameLogic::GetPlayer()
{
	return m_Player;
}

/************************************************************************
*  获取游戏类型
************************************************************************/
int GameLogic::GetGameType()
{
	return m_nGameType;
}

/************************************************************************
*  设置游戏类型
************************************************************************/
void GameLogic::SetGameType(int type)
{
	m_nGameType = type;
}

/************************************************************************
*  程序退出
************************************************************************/
void GameLogic::ExitApp()
{
	if (GetGameType() == SINGLE_GAME_TYPE)
	{
	}
	else
	{
		StopGame();
		GetNetworkModule()->Logout();
		GetNetworkModule()->Close();
	}

	Director::getInstance()->end();
}

/************************************************************************
*  英雄的行为
************************************************************************/
void GameLogic::HeroAction(int action_type,int direction)
{
	Hero* pCurrentHero = HeroManager::GetInstance()->GetCurrentHero();
	Hero* pCurrentEnemy = HeroManager::GetInstance()->GetCurrentEnemy();

	if (pCurrentEnemy == NULL || pCurrentHero == NULL)
	{
		return;
	}

	HERO_ACTION action = (HERO_ACTION)action_type;

	switch (action)
	{
	case STATIC_ACTION:
		{
			if (pCurrentHero->GetHeroState() == DEATH_STATE)
			{
				return;
			}
			pCurrentHero->StopAnimation();
		}
		break;
	case MOVE_ACTION:
		HeroManager::GetInstance()->HeroMove(pCurrentHero->GetHeroID(),(tagDirecton)direction);
		break;
	case ATTACK_ACTION:
		{
			HeroManager::GetInstance()->HeroAttack(pCurrentHero->GetHeroID(),pCurrentEnemy->GetHeroID(),pCurrentHero->GetFaceDirection());
		}
		break;
	case MAGICATTACK_ACTION:
		{
			HeroManager::GetInstance()->HeroMagicAttack(pCurrentHero->GetHeroID(),pCurrentEnemy->GetHeroID(),pCurrentHero->GetFaceDirection());
		}
		break;
	default:
		break;
	}
}

/************************************************************************
*  场景之间的转换
************************************************************************/
void GameLogic::JumpToScene(SCENE_ID id)
{
	switch (id)
	{
	case INVALID_SCENE:
		break;
	case START_SCENE:
		break;
	case CHOOSEGAME_SCENE:
		{
			auto scene = ChooseGameScene::createScene();
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(0.5,scene));
		}
		break;
	case MAIN_SCENE:
		{
			auto scene = MainScene::createScene();
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(0.5,scene));
		}
		break;
	case RESULT_SCENE:
		{
			auto scene = ResultScene::createScene();
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(0.5,scene));
		}
		break;
	case SHOW_HERO_SCENE:
		{
			auto scene = ShowHeroScene::createScene();
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(0.5,scene));
		}
		break;
	case CREATEORJOIN_SCENE:
		{
			auto scene = CreateOrJoinScene::createScene();
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(0.5,scene));
		}
		break;
	default:
		break;
	}

}