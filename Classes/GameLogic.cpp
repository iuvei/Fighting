#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "./scene/ChooseGameScene.h"
#include "GameLogic.h"
#include "GameDefine.h"
#include "Hero.h"
#include "./scene/MainScene.h"


#include "../CommonClass/Util.h"


// add by mobinsheng for ������繦�� begin
#include "NetworkModule.h"
// add by mobinsheng for ������繦�� end

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
*  ��ȡ��Ϸ�߼�����
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
*  ���캯��
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
*  ��������
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

	// ��ʼ��Ӣ������
	if(!HeroManager::GetInstance()->InitHero())
	{
		return; 
	}

	// ������ҵ�Ӣ�۵�ID
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

	// �������б�־
	IsGameRunning(true);

	MagicManager::GetInstance()->ShowLamp();

}

/************************************************************************
*  ��Ϸѭ�������������ļ�ʱ�������Ե���
************************************************************************/

void GameLogic::LoopGame()
{
	// �����Ϸû�����У��򲻽��д���
	if (!GameLogic::GetInstance()->IsGameRunning())
	{
		return;
	}

	// ��Ч��Ϸ
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

	// ħ����Ӣ������֮�����ײ���
	MagicManager::GetInstance()->MagicCollisionDetection();

	// ������Ϸ��Ҫ����
	if (GetGameType() == SINGLE_GAME_TYPE)
	{
		HeroManager::GetInstance()->UpdateHero();
		return;
	}

	if (HeroManager::GetInstance()->GetCurrentHero() == NULL || HeroManager::GetInstance()->GetCurrentEnemy() == NULL)
	{
		return;
	}

	// ����������ģʽ�µĴ���

	// Ӣ���ƶ������������ܹ��������ֶ������ڰ������µ�ʱ�������ݰ�
	if (HeroManager::GetInstance()->GetCurrentHero()->GetHeroState() == ATTACK_STATE 
		|| HeroManager::GetInstance()->GetCurrentHero()->GetHeroState() == MAGICATTACK_STATE
		|| HeroManager::GetInstance()->GetCurrentHero()->GetHeroState() == MOVE_STATE
		)
	{
	}
	else
	{
		// ������ʱ���������������ݰ�
		HeroManager::GetInstance()->SendHeroStateToServer(HeroManager::GetInstance()->GetCurrentHero());
	}

	ControlPackage ctlPack;
	DataPackage dataPack;
	PackageHead head;

	// ��ȡ��ͷ��
	bool bOK = GameLogic::GetInstance()->GetNetworkModule()->ReadPackageHead(head,true);

	if (bOK == false)
	{
		return;
	}

	// �������Ϸ�������յ����ư�����ôֻ�����������
	// 1���Է���ͣ����Ϸ
	// 2���Է�����������Ϸ
	// 3���Է��뿪����Ϸ
	if (head.GetPackageType() == package_control)
	{
		GameLogic::GetInstance()->GetNetworkModule()->ReadControlPackBody(head,&ctlPack);

		// �Է���ͣ��Ϸ
		if (ctlPack.GetPackageCode() == code_suspend_game)
		{
			GameLogic::GetInstance()->SetSuspendState(OPPONENT_SUSPEND);
		}

		// �Է�����ִ����Ϸ
		if (ctlPack.GetPackageCode() == code_continue_game)
		{
			GameLogic::GetInstance()->SetSuspendState(NOT_SUSPEND);
		}

		// �Է��뿪��Ϸ
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

	// �Է�����
	if (dataPack.GetState() == DEATH_STATE )
	{
		HeroManager::GetInstance()->GetCurrentEnemy()->SetBloodValue(0);
		return;
	}

	// �Է��ƶ�
	if (dataPack.GetState() == MOVE_STATE)
	{
		HeroManager::GetInstance()->HeroMove(HeroManager::GetInstance()->GetCurrentEnemy()->GetHeroID(),(tagDirecton)dataPack.GetMoveDirection());
	}
	// �Է���ֹ
	if (dataPack.GetState() == STATIC_STATE)
	{
		HeroManager::GetInstance()->GetCurrentEnemy()->StopAnimation();
	}

	// �Է�����
	if (dataPack.GetState() == ATTACK_STATE)
	{
		if (HeroManager::GetInstance()->GetCurrentEnemy()->GetHeroState() != ATTACK_STATE)
		{
			HeroManager::GetInstance()->GetCurrentEnemy()->StopAnimation();
		}
		HeroManager::GetInstance()->GetCurrentEnemy()->Attack();
	}

	// �Է�ħ������
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
*  ������
************************************************************************/
void GameLogic::Clean()
{
	if (GetGameType() != SINGLE_GAME_TYPE && GetNetworkModule()->IsConnect())
	{
		m_pNetwork->StopGame();
	}
	
	// ħ������������
	MagicManager::GetInstance()->Clean();

	// Ӣ�۹���������
	HeroManager::GetInstance()->Clean();

	// ��Ϸ���б�־
	m_bIsGameRunning = false;

	m_bInit = false;

	// ������
	m_nGameType = INVALID_GAME_TYPE;

}

// add by mobinsheng for ������繦�� begin
// ��½������
bool GameLogic::ConnectToServer(const char* ip,int nPort)
{
	bool ret = m_pNetwork->Connect(ip,nPort);

	return ret;
}

// ��ȡ����ģ��
NetworkMoudle* GameLogic::GetNetworkModule()
{
	return m_pNetwork;
}


/************************************************************************
*  ���������ļ�
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
*  ��ȡ������������Ϸ���еĳ�����
************************************************************************/
MainScene* GameLogic::GetMainScene()
{
	return m_pMainScene;
}

/************************************************************************
*  ������Ϸ���б�־
************************************************************************/
void GameLogic::IsGameRunning(bool v)
{
	m_bIsGameRunning = v;
}

/************************************************************************
*  ��ȡ��Ϸ���б�־
************************************************************************/
bool GameLogic::IsGameRunning()
{
	return m_bIsGameRunning;
}

/************************************************************************
*  ��ȡ��Ϸ����ͣ״̬��û����ͣ���Լ���ͣ���Է���ͣ��
************************************************************************/
int GameLogic::GetSuspendState()
{
	return m_nSuspendState;
}

/************************************************************************
*  ������Ϸ����ͣ״̬
************************************************************************/
void GameLogic::SetSuspendState(int state)
{
	m_nSuspendState = state;
}

/************************************************************************
*  ��ͣ��Ϸ
* ע�ⲻ�ڴ˺�����������ͣ״̬����Ϊ��֪���ǶԷ�������ͣ�����Լ����
* ������ͣ��������ͣ��ʱ����ͣ״̬�ڱ�ĵط�����
************************************************************************/
void GameLogic::SuspendGame()
{
	GetNetworkModule()->SuspendGame();
}

/************************************************************************
*  ������Ϸ
************************************************************************/
void GameLogic::ContinueGame()
{
	GetNetworkModule()->ContinueGame();
	m_nSuspendState = NOT_SUSPEND;
}

/************************************************************************
*  �Է�ֹͣ����Ϸ
************************************************************************/
void GameLogic::OpponentStopGame()
{
	JumpToScene(RESULT_SCENE);
	Clean();
}

/************************************************************************
*  �Լ����ֹͣ����Ϸ
************************************************************************/
void GameLogic::StopGame()
{
	JumpToScene(RESULT_SCENE);
	Clean();
}


/************************************************************************
*  ��ȡ��Ҷ���
************************************************************************/
Player* GameLogic::GetPlayer()
{
	return m_Player;
}

/************************************************************************
*  ��ȡ��Ϸ����
************************************************************************/
int GameLogic::GetGameType()
{
	return m_nGameType;
}

/************************************************************************
*  ������Ϸ����
************************************************************************/
void GameLogic::SetGameType(int type)
{
	m_nGameType = type;
}

/************************************************************************
*  �����˳�
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
*  Ӣ�۵���Ϊ
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
*  ����֮���ת��
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