#include "GameAI.h"
#include "Hero.h"
#include "../CommonClass/Util.h"
#include "HeroManager.h"

/************************************************************************
*  AI的主逻辑
************************************************************************/
void GameAI::MainProcess(Hero* pHero)
{
	if (pHero == 0)
	{
		return;
	}

	m_pHero = pHero;

	// 如果英雄已经死亡，那么不再对它进行处理
	if (pHero->GetHeroState() == DEATH_STATE)
	{
		return;
	}

	// 设置了AI选项才能进行处理
	if (!GlobalValue::use_ai)
	{
		return;
	}

	// 检测自己是否到达边界，如果已经达到那么调整位置
	CheckAndAdjustPosition();

	// 该变量表示对手是否已经进入自己的视距内
	bool bIsInCheckDomain = false;

	// 检测对手是否在视距之内，如果在视距之内，那么进行跟踪、攻击等动作
	Action(bIsInCheckDomain);

	if (bIsInCheckDomain)
	{
		return ;
	}

	// 如果对手不在视距之内，就进行巡逻、随机放技能、随机攻击一下

	// 获取概率
	int n = Util::GetRandomNum(1,100);
	float probability = n / 100.0;

	// 该概率是静止概率
	if (probability < GlobalValue::nomal_ai_probability.static_probability)
	{
		return;
	}

	// 该概率是进行移动
	if (probability <= GlobalValue::nomal_ai_probability.move_probability
		&& probability >= GlobalValue::nomal_ai_probability.static_probability)
	{

		Run();

		return;
	}
	// 该概率是进行攻击
	else if( probability >= GlobalValue::nomal_ai_probability.move_probability 
		&& probability < GlobalValue::nomal_ai_probability.attack_probability)
	{
		if (m_pHero->GetHeroState() != ATTACK_STATE)
		{
			m_pHero->Attack();
		}

		return;
	}
	// 该概率是进行魔法攻击
	else if(probability >= GlobalValue::nomal_ai_probability.attack_probability )
	{
		if(m_pHero->GetHeroState() != MAGICATTACK_STATE)
		{
			m_pHero->MagicAttack();
		}
		return;
	}
	else
	{
		return;
	}
}

/************************************************************************
*  AI英雄移动
************************************************************************/
void GameAI::Run()
{
	// 获取AI英雄的当前位置
	Point currentPoint = m_pHero->getPosition();

	// 获取英雄的脸部朝向
	Size winsize = Director::getInstance()->getVisibleSize();

	// 如果英雄已经在移动
	if (m_pHero->GetHeroState() == MOVE_STATE)
	{
		// 检测英雄是否到达边界并调整位置
		CheckAndAdjustPosition();

		// 移动英雄
		if (m_pHero->GetFaceDirection() == directon_left)
		{
			m_pHero->setPosition(currentPoint.x - GlobalValue::hero_move_step,currentPoint.y);
		}
		else
		{
			m_pHero->setPosition(currentPoint.x + GlobalValue::hero_move_step,currentPoint.y);
		}

		return;

	}

	// 如果英雄没有在移动，那么根据脸部朝向进行移动
	if (m_pHero->GetFaceDirection() == directon_left)
	{
		m_pHero->SetFacetoLeftMoveAnimation();
	}
	else
	{
		m_pHero->SetFacetoRightMoveAnimation();
	}
}

/************************************************************************
*  视距检测
************************************************************************/
void GameAI::Action(bool& bIsInCheckDomain)
{
	// 获取对手，由于具有AI的英雄通常是敌人，所以他的对手就是玩家英雄
	Hero* opponent = HeroManager::GetInstance()->GetCurrentHero();

	// 如果对手已经死亡，那么不进行处理，而且也表示队手不在视距之内
	if (opponent->GetHeroState() == DEATH_STATE)
	{
		bIsInCheckDomain  = false;
		return;
	}

	float x = m_pHero->getPosition().x - opponent->getPosition().x;

	float y = m_pHero->getPosition().y - opponent->getPosition().y;

	// 计算自己和对手之间的距离
	float distance = sqrt(pow(x,2) + pow(y,2));

	// 如果不在视距之内，不进行处理
	if (distance > GlobalValue::ai_check_distance)
	{
		bIsInCheckDomain  = false;
		return;
	}

	// 在视距之内
	bIsInCheckDomain = true;

	// 只在视距之内，还没有到达攻击范围，进行跟踪
	if (distance <= GlobalValue::ai_check_distance && distance > GlobalValue::ai_attack_distance)
	{
		Follow();
		return;
	}

	// 进入了攻击范围

	// 如果英雄已经在攻击状态，那么就不继续处理
	if (m_pHero->GetHeroState() == ATTACK_STATE || m_pHero->GetHeroState() == MAGICATTACK_STATE)
	{
		return;
	}

	// 如果此时英雄的状态是非静止的，那么让英雄站在原地
	if (m_pHero->GetHeroState() != STATIC_STATE)
	{
		m_pHero->StopAnimation();
	}

	//  根据概率选择动作的类型
	float probability = Util::GetProbability();

	// 静止概率
	if (probability < GlobalValue::attack_ai_probability.static_probability)
	{
		return;
	}
	// 跟踪概率
	else if(probability >= GlobalValue::attack_ai_probability.static_probability && 
		probability < GlobalValue::attack_ai_probability.move_probability)
	{
		Follow();
	}
	// 攻击概率
	else if (probability >= GlobalValue::attack_ai_probability.move_probability &&
		probability < GlobalValue::attack_ai_probability.attack_probability)
	{
		m_pHero->Attack();
	}
	// 魔法攻击概率
	else
	{
		m_pHero->MagicAttack();
	}
	return ;

}

/************************************************************************
*  检测英雄是否已经到达边界，并相应的调整位置
************************************************************************/
void GameAI::CheckAndAdjustPosition()
{
	// 英雄当前的位置
	Point currentPoint = m_pHero->getPosition();

	// 获取屏幕的大小
	Size winsize = Director::getInstance()->getVisibleSize();

	// 如果英雄不是移动状态的，不进行处理
	if (m_pHero->GetHeroState() == MOVE_STATE)
	{
		// 超出边界
		if (currentPoint.x >= winsize.width)
		{
			// 进行相应的调整
			m_pHero->setPosition(Point(winsize.width - 1,currentPoint.y));
			m_pHero->StopAnimation();
			if (m_pHero->GetFaceDirection() == directon_left)
			{
				m_pHero->m_nFaceTo =directon_right ;
				m_pHero->SetFacetoRightStaticAnimation();
			}
			else
			{
				m_pHero->m_nFaceTo = directon_left;
				m_pHero->SetFacetoLeftStaticAnimation();
			}
		}
		// 超出边界
		else if(currentPoint.x <= 0)
		{
			// 进行相应的调整
			m_pHero->StopAnimation();
			m_pHero->setPosition(Point(currentPoint.x + 2,currentPoint.y));
			if (m_pHero->GetFaceDirection() == directon_left)
			{
				m_pHero->m_nFaceTo =directon_right ;
				m_pHero->SetFacetoRightStaticAnimation();
			}
			else
			{
				m_pHero->m_nFaceTo = directon_left;
				m_pHero->SetFacetoLeftStaticAnimation();
			}
		}
		else
		{
		}
	}
}

/************************************************************************
*  跟踪对手
************************************************************************/
void GameAI::Follow()
{
	// 对手（即玩家英雄）
	Hero* opponent = HeroManager::GetInstance()->GetCurrentHero();

	float x = m_pHero->getPosition().x - opponent->getPosition().x;

	float y = m_pHero->getPosition().y - opponent->getPosition().y;

	float distance = sqrt(pow(x,2) + pow(y,2));

	Point currentPoint = m_pHero->getPosition();

	// 跟踪对手的时候，速度应该比对手慢
	int step = GlobalValue::hero_move_step / 2;

	// distance 在100到300之间

	if (x > 0) //队手在自己的左边
	{
		if (m_pHero->GetFaceDirection() != directon_left)
		{
			m_pHero->m_nFaceTo = directon_left;
			m_pHero->StopAnimation();
		}

		if (m_pHero->GetHeroState() != MOVE_STATE)
		{
			m_pHero->SetFacetoLeftMoveAnimation();
		}

		m_pHero->setPosition(Point(currentPoint.x - step,currentPoint.y) );
	}
	else if(x < 0) // 对手在自己的右边
	{
		if (m_pHero->GetFaceDirection() != directon_right)
		{
			m_pHero->m_nFaceTo = directon_right;
			m_pHero->StopAnimation();
		}

		if (m_pHero->GetHeroState() != MOVE_STATE)
		{
			m_pHero->SetFacetoRightMoveAnimation();
		}

		m_pHero->setPosition(Point(currentPoint.x + step,currentPoint.y ));
	}

	// 如果此时英雄的状态还不是移动的话，那么移动，为了是英雄的动作连贯
	if (m_pHero->GetHeroState() != MOVE_STATE)
	{
		if (m_pHero->GetFaceDirection() == directon_left)
		{
			m_pHero->SetFacetoLeftMoveAnimation();
		}
		else
		{
			m_pHero->SetFacetoRightMoveAnimation();
		}
	}

	if (y > 0)  //队手在自己的下边
	{
		m_pHero->setPosition(Point(currentPoint.x ,currentPoint.y -  step));
	}
	else if(y < 0) // 对手在自己的上方
	{
		m_pHero->setPosition(Point(currentPoint.x ,currentPoint.y +  step));
	}
}