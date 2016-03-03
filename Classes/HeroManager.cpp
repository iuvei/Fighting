#include "HeroManager.h"
#include "GameLogic.h"
#include "../CommonClass/Package.h"
#include "NetworkModule.h"

#include "Hero.h"
#include "GameAI.h"
#include "./hero/Bandit.h"
#include "./hero/Rudolf.h"
#include "./hero/bat.h"
#include "./hero/Davis.h"
#include "./hero/Deep.h"
#include "./hero/Dennis.h"
#include "./hero/Firen.h"
#include "./hero/Firzen.h"

#include "./hero/Freeze.h"
#include "./hero/Henry.h"
#include "./hero/Hunter.h"
#include "./hero/Jack.h"
#include "./hero/Jan.h"
#include "./hero/John.h"
#include "./hero/Julian.h"
#include "./hero/Justin.h"
#include "./hero/Knight.h"
#include "./hero/Louis.h"
#include "./hero/LouisEX.h"
#include "./hero/Mark.h"
#include "./hero/Monk.h"
#include "./hero/Sorcerer.h"
#include "./hero/Woody.h"

HeroManager* HeroManager::m_pInstance = 0;

// 全局的英雄信息数组
HeroInfoMap G_HeroInfo[] = 
{
	// 定义方式 ：
	// {id，名字}
	{BANDIT_ID,BANDIT_NAME},
	{BAT_ID,BAT_NAME},
	{DAVIS_ID,DAVIS_NAME},
	{DEEP_ID,DEEP_NAME},
	{DENNIS_ID,DENNIS_NAME},
	{FIREN_ID,FIREN_NAME},
	{FIRZEN_ID,FIRZEN_NAME},
	{FREEZE_ID,FREEZE_NAME},
	{HENRY_ID,HENRY_NAME},
	{HUNTER_ID,HUNTER_NAME},
	{JACK_ID,JACK_NAME},
	{JAN_ID,JAN_NAME},
	{JOHN_ID,JOHN_NAME},
	{JULIAN_ID,JULIAN_NAME},
	{JUSTIN_ID,JUSTIN_NAME},
	{KNIGHT_ID,KNIGHT_NAME},
	{LOUIS_ID,LOUIS_NAME},
	{LOUISEX_ID,LOUISEX_NAME},
	{MARK_ID,MARK_NAME},
	{MONK_ID,MONK_NAME},
	{RUDOLF_ID,RUDOLF_NAME},
	{SORCERER_ID,SORCERER_NAME},
	{WOODY_ID,WOODY_NAME},
};

// 根据英雄名字获取英雄ID
HERO_ID HeroManager::GetHeroIDByName(const char* pHeroName)
{
	int count = sizeof(G_HeroInfo)/sizeof(HeroInfoMap);
	for (int i = 0; i < count; ++i)
	{
		if (0 == strcmp(pHeroName,G_HeroInfo[i].name.c_str()))
		{
			return G_HeroInfo[i].id;
		}
	}

	return INVALID_ID;
}

const char* HeroManager::GetHeroNameByID(HERO_ID id)
{
	int count = sizeof(G_HeroInfo)/sizeof(HeroInfoMap);
	for (int i = 0; i < count; ++i)
	{
		if (id == G_HeroInfo[i].id)
		{
			return G_HeroInfo[i].name.c_str();
		}
	}

	return "";
}

int HeroManager::GetHeroDefineCount()
{
	return  sizeof(G_HeroInfo)/sizeof(HeroInfoMap);
}

HeroManager::HeroManager()
{
	m_pCurrentEnemy = 0;
	m_pCurrentHero = 0;

	m_CurrentEnemyID = INVALID_ID;
	m_CurrentHeroID = INVALID_ID;

	m_pGameAI = new GameAI;
}

HeroManager::~HeroManager()
{
	if (m_pGameAI != 0)
	{
		delete m_pGameAI;
		m_pGameAI = 0;
	}
}

HeroManager* HeroManager::GetInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new HeroManager;
	}

	return m_pInstance;
}

bool HeroManager::InitHero()
{
	if (GameLogic::GetInstance()->GetGameType() == INVALID_GAME_TYPE)
	{
		return false;
	}

	if (GameLogic::GetInstance()->GetGameType() == SINGLE_GAME_TYPE)
	{
		m_pCurrentHero = HeroManager::Create(m_CurrentHeroID);
		m_pCurrentHero->InitHero((HERO_ID)m_CurrentHeroID,directon_left,directon_right,true);

		m_pCurrentEnemy = HeroManager::Create(m_CurrentEnemyID);
		m_pCurrentEnemy->InitHero((HERO_ID)m_CurrentEnemyID,directon_right,directon_left,true);

		m_HeroVector.push_back(m_pCurrentHero);
		m_HeroVector.push_back(m_pCurrentEnemy);
	}
	else
	{
		if (GameLogic::GetInstance()->GetGameType() == CREATE_GAME_TYPE)// 创建者在左边
		{
			m_pCurrentHero = HeroManager::Create(m_CurrentHeroID);
			m_pCurrentHero->InitHero((HERO_ID)m_CurrentHeroID,directon_left,directon_right,false);

			m_pCurrentEnemy = HeroManager::Create(m_CurrentEnemyID);
			m_pCurrentEnemy->InitHero((HERO_ID)m_CurrentEnemyID,directon_right,directon_left,false);

			m_HeroVector.push_back(m_pCurrentHero);
			m_HeroVector.push_back(m_pCurrentEnemy);
		}
		else// 加入者在右边
		{
			m_pCurrentEnemy = HeroManager::Create(m_CurrentEnemyID);
			m_pCurrentEnemy->InitHero((HERO_ID)m_CurrentEnemyID,directon_left,directon_right,false);

			m_pCurrentHero = HeroManager::Create(m_CurrentHeroID);
			m_pCurrentHero->InitHero((HERO_ID)m_CurrentHeroID,directon_right,directon_left,false);

			m_HeroVector.push_back(m_pCurrentHero);
			m_HeroVector.push_back(m_pCurrentEnemy);
		}
	}

	return true;
}


/************************************************************************
*  根据英雄ID获取一个英雄
************************************************************************/
Hero* HeroManager::GetHero(HERO_ID id)
{
	for (int i = 0; i < m_HeroVector.size(); ++i)
	{
		if (id == m_HeroVector[i]->GetHeroID())
		{
			return m_HeroVector[i];
		}
	}

	return 0;
}

/************************************************************************
*  获取玩家当前选中的英雄
************************************************************************/
Hero* HeroManager::GetCurrentHero()
{
	return m_pCurrentHero;
}

/************************************************************************
*  获取当前的敌人
************************************************************************/
Hero* HeroManager::GetCurrentEnemy()
{
	return m_pCurrentEnemy;
}

/************************************************************************
*  选择英雄
************************************************************************/
void HeroManager::SetCurrentHero(HERO_ID h)
{
	m_CurrentHeroID = h;
}

/************************************************************************
*  选择敌人
************************************************************************/
void HeroManager::SetCurrentEnemy(HERO_ID id)
{
	m_CurrentEnemyID = id;
}

vector<Hero*>& HeroManager::GetHeroList()
{
	return m_HeroVector;
}


/************************************************************************
*  让英雄运动
************************************************************************/
void HeroManager::HeroMove(HERO_ID id,tagDirecton directon)
{
	if (GameLogic::GetInstance()->GetSuspendState() != NOT_SUSPEND)
	{
		return;
	}

	Hero* pHero = GetHero(id);

	if (0 == pHero)
	{
		return ;
	}

	if (pHero->GetHeroState() == DEATH_STATE)
	{
		return;
	}

	int step = GlobalValue::hero_move_step;

	Point currentPoint = pHero->getPosition();

	
	switch (directon)
	{
	case directon_right: // 向右
		{
			if (pHero->GetCurrentDirection() != directon_right || pHero->GetHeroState() != MOVE_STATE)
			{
				pHero->StopAnimation();	
				pHero->Run(directon_right);
			}

			pHero->setPosition(Point(currentPoint.x + step,currentPoint.y));

		}

		break;

	case directon_up: // 向上
		{
			pHero->Run(directon_up);
			pHero->setPosition(Point(currentPoint.x ,currentPoint.y + step));
		}

		break;

	case directon_left: // 向左
		{
			if (pHero->GetCurrentDirection() != directon_left || pHero->GetHeroState() != MOVE_STATE)
			{
				pHero->StopAnimation();
				pHero->Run(directon_left);
			}

			pHero->setPosition(Point(currentPoint.x - step,currentPoint.y));

		}

		break;

	case directon_down: // 向下
		{
			pHero->Run(directon_down);
			pHero->setPosition(Point(currentPoint.x ,currentPoint.y - step));
		}

		break;

	default:
		pHero->StopAnimation();
		break;

	}
	if (pHero == m_pCurrentHero)
	{
		SendHeroStateToServer(m_pCurrentHero);
	}

}



/************************************************************************
*  施放魔法
************************************************************************/
void  HeroManager::HeroMagicAttack(HERO_ID sender,HERO_ID revcver,tagDirecton d)
{
	if (GameLogic::GetInstance()->GetSuspendState() != NOT_SUSPEND)
	{
		return;
	}


	Hero* pHero = GetHero(sender);

	if (0 == pHero)
	{
		return ;
	}
	if (pHero->GetHeroState() == DEATH_STATE)
	{
		return;
	}

	pHero->MagicAttack();
	
	if (pHero == m_pCurrentHero)
	{
		SendHeroStateToServer(m_pCurrentHero);
	}

}

/************************************************************************
*  施放魔法
************************************************************************/
void  HeroManager::HeroAttack(HERO_ID sender,HERO_ID recver,tagDirecton d)
{
	if (GameLogic::GetInstance()->GetSuspendState() != NOT_SUSPEND)
	{
		return;
	}

	Hero* pHero = GetHero(sender);

	if (0 == pHero)
	{
		return ;
	}
	if (pHero->GetHeroState() == DEATH_STATE)
	{
		return;
	}

	pHero->Attack();
	
	if (pHero == m_pCurrentHero)
	{
		SendHeroStateToServer(m_pCurrentHero);
	}
}

/************************************************************************
*  发送英雄的状态信息到服务器
************************************************************************/
void HeroManager::SendHeroStateToServer(Hero* pHero)
{
	if (GameLogic::GetInstance()->IsGameRunning()== false || GameLogic::GetInstance()->GetGameType()== SINGLE_GAME_TYPE || pHero == 0)
	{
		return;
	}

	DataPackage pack;
	pack.SetBlood(pHero->GetBloodValue());
	pack.SetFaceTo(pHero->GetFaceDirection());
	pack.SetMoveDirection(pHero->GetCurrentDirection());
	pack.SetState(pHero->GetHeroState());
	pack.SetHeroID(pHero->GetHeroID());
	pack.SetMagic(pHero->GetMagicValue());
	pack.SetPosition(pHero->getPositionX(),pHero->getPositionY());
	GameLogic::GetInstance()->GetNetworkModule()->SendDataPack(pack);
}


// 从服务器中实时获取状态
void	HeroManager::UpdateHero()
{
	if (GameLogic::GetInstance()->IsGameRunning() == false)
	{
		return;
	}

	if (m_pCurrentHero->GetHeroState() == DEATH_STATE)
	{
		return;
	}

	static long sec = 0;

	++sec;

	// 定时回血和回魔
	if (sec == 100)
	{
		sec = 0;

		// ====================更新自己的英雄=================
		if (m_pCurrentHero->GetBloodValue() < 100)
		{
			m_pCurrentHero->SetBloodValue(m_pCurrentHero->GetBloodValue() + GlobalValue::blood_regain_value);
		}

		if (m_pCurrentHero->GetMagicValue() < 100)
		{
			m_pCurrentHero->SetMagicValue(m_pCurrentHero->GetMagicValue() + GlobalValue::magic_regain_value);
		}

		// ====================更新敌人的英雄=================

		if (m_pCurrentEnemy->GetBloodValue() < 100)
		{
			m_pCurrentEnemy->SetBloodValue(m_pCurrentEnemy->GetBloodValue() + GlobalValue::blood_regain_value);
		}

		if (m_pCurrentEnemy->GetMagicValue() < 100)
		{
			m_pCurrentEnemy->SetMagicValue(m_pCurrentEnemy->GetMagicValue() + GlobalValue::magic_regain_value);
		}

	}

	// AI控制
	if (GameLogic::GetInstance()->GetGameType() == SINGLE_GAME_TYPE)
	{
		m_pGameAI->MainProcess(m_pCurrentEnemy);
	}
}

void HeroManager::Clean()
{
	m_pCurrentEnemy = 0;
	m_pCurrentHero = 0;

	m_CurrentEnemyID = INVALID_ID;
	m_CurrentHeroID = INVALID_ID;

	m_HeroVector.clear();
}



// 一个创建英雄的简化宏 begin
#ifndef CREATE_HERO_EX_BEGIN
#define CREATE_HERO_EX_BEGIN\
	switch (id) {
#endif 

// 一个创建英雄的简化宏 定义
#ifndef CREATE_HERO_EX
#define CREATE_HERO_EX(id,classname)\
	case id: pHero = classname::create(); break;
#endif 

// 一个创建英雄的简化宏 end
#ifndef CREATE_HERO_EX_END
#define CREATE_HERO_EX_END }
#endif 

/************************************************************************
*  工厂函数：创建英雄对象
* 增加新英雄的时候要在这里添加的自己的代码
************************************************************************/
Hero* HeroManager::Create(HERO_ID id)
{
	Hero* pHero = 0;

	// 以后新增英雄的时候往这里添加
	// 在这里添加英雄-begin
	CREATE_HERO_EX_BEGIN
	
	CREATE_HERO_EX(BANDIT_ID,Bandit);
	CREATE_HERO_EX(RUDOLF_ID,Rudolf);
	CREATE_HERO_EX(BAT_ID,Bat);
	CREATE_HERO_EX(DAVIS_ID,Davis);
	CREATE_HERO_EX(DEEP_ID,Deep);
	CREATE_HERO_EX(DENNIS_ID,Dennis);
	CREATE_HERO_EX(FIREN_ID,Firen);
	CREATE_HERO_EX(FIRZEN_ID,Firzen);

	CREATE_HERO_EX(FREEZE_ID,Freeze);
	CREATE_HERO_EX(HENRY_ID,Henry);
	CREATE_HERO_EX(HUNTER_ID,Hunter);
	CREATE_HERO_EX(JACK_ID,Jack);
	CREATE_HERO_EX(JAN_ID,Jan);

	CREATE_HERO_EX(JOHN_ID,John);
	CREATE_HERO_EX(JULIAN_ID,Julian);
	CREATE_HERO_EX(JUSTIN_ID,Justin);
	CREATE_HERO_EX(KNIGHT_ID,Knight);
	CREATE_HERO_EX(LOUIS_ID,Louis);

	CREATE_HERO_EX(LOUISEX_ID,LouisEX);
	CREATE_HERO_EX(MARK_ID,Mark);
	CREATE_HERO_EX(MONK_ID,Monk);
	CREATE_HERO_EX(SORCERER_ID,Sorcerer);
	CREATE_HERO_EX(WOODY_ID,Woody);

	CREATE_HERO_EX_END
	// 在这里添加英雄-end

	/*
	switch (id)
	{
	case BANDIT_ID:
		pHero = Bandit::create();
		break;
	}*/

	return pHero;
}
