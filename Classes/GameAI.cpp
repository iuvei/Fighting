#include "GameAI.h"
#include "Hero.h"
#include "../CommonClass/Util.h"
#include "HeroManager.h"

/************************************************************************
*  AI�����߼�
************************************************************************/
void GameAI::MainProcess(Hero* pHero)
{
	if (pHero == 0)
	{
		return;
	}

	m_pHero = pHero;

	// ���Ӣ���Ѿ���������ô���ٶ������д���
	if (pHero->GetHeroState() == DEATH_STATE)
	{
		return;
	}

	// ������AIѡ����ܽ��д���
	if (!GlobalValue::use_ai)
	{
		return;
	}

	// ����Լ��Ƿ񵽴�߽磬����Ѿ��ﵽ��ô����λ��
	CheckAndAdjustPosition();

	// �ñ�����ʾ�����Ƿ��Ѿ������Լ����Ӿ���
	bool bIsInCheckDomain = false;

	// �������Ƿ����Ӿ�֮�ڣ�������Ӿ�֮�ڣ���ô���и��١������ȶ���
	Action(bIsInCheckDomain);

	if (bIsInCheckDomain)
	{
		return ;
	}

	// ������ֲ����Ӿ�֮�ڣ��ͽ���Ѳ�ߡ�����ż��ܡ��������һ��

	// ��ȡ����
	int n = Util::GetRandomNum(1,100);
	float probability = n / 100.0;

	// �ø����Ǿ�ֹ����
	if (probability < GlobalValue::nomal_ai_probability.static_probability)
	{
		return;
	}

	// �ø����ǽ����ƶ�
	if (probability <= GlobalValue::nomal_ai_probability.move_probability
		&& probability >= GlobalValue::nomal_ai_probability.static_probability)
	{

		Run();

		return;
	}
	// �ø����ǽ��й���
	else if( probability >= GlobalValue::nomal_ai_probability.move_probability 
		&& probability < GlobalValue::nomal_ai_probability.attack_probability)
	{
		if (m_pHero->GetHeroState() != ATTACK_STATE)
		{
			m_pHero->Attack();
		}

		return;
	}
	// �ø����ǽ���ħ������
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
*  AIӢ���ƶ�
************************************************************************/
void GameAI::Run()
{
	// ��ȡAIӢ�۵ĵ�ǰλ��
	Point currentPoint = m_pHero->getPosition();

	// ��ȡӢ�۵���������
	Size winsize = Director::getInstance()->getVisibleSize();

	// ���Ӣ���Ѿ����ƶ�
	if (m_pHero->GetHeroState() == MOVE_STATE)
	{
		// ���Ӣ���Ƿ񵽴�߽粢����λ��
		CheckAndAdjustPosition();

		// �ƶ�Ӣ��
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

	// ���Ӣ��û�����ƶ�����ô����������������ƶ�
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
*  �Ӿ���
************************************************************************/
void GameAI::Action(bool& bIsInCheckDomain)
{
	// ��ȡ���֣����ھ���AI��Ӣ��ͨ���ǵ��ˣ��������Ķ��־������Ӣ��
	Hero* opponent = HeroManager::GetInstance()->GetCurrentHero();

	// ��������Ѿ���������ô�����д�������Ҳ��ʾ���ֲ����Ӿ�֮��
	if (opponent->GetHeroState() == DEATH_STATE)
	{
		bIsInCheckDomain  = false;
		return;
	}

	float x = m_pHero->getPosition().x - opponent->getPosition().x;

	float y = m_pHero->getPosition().y - opponent->getPosition().y;

	// �����Լ��Ͷ���֮��ľ���
	float distance = sqrt(pow(x,2) + pow(y,2));

	// ��������Ӿ�֮�ڣ������д���
	if (distance > GlobalValue::ai_check_distance)
	{
		bIsInCheckDomain  = false;
		return;
	}

	// ���Ӿ�֮��
	bIsInCheckDomain = true;

	// ֻ���Ӿ�֮�ڣ���û�е��﹥����Χ�����и���
	if (distance <= GlobalValue::ai_check_distance && distance > GlobalValue::ai_attack_distance)
	{
		Follow();
		return;
	}

	// �����˹�����Χ

	// ���Ӣ���Ѿ��ڹ���״̬����ô�Ͳ���������
	if (m_pHero->GetHeroState() == ATTACK_STATE || m_pHero->GetHeroState() == MAGICATTACK_STATE)
	{
		return;
	}

	// �����ʱӢ�۵�״̬�ǷǾ�ֹ�ģ���ô��Ӣ��վ��ԭ��
	if (m_pHero->GetHeroState() != STATIC_STATE)
	{
		m_pHero->StopAnimation();
	}

	//  ���ݸ���ѡ����������
	float probability = Util::GetProbability();

	// ��ֹ����
	if (probability < GlobalValue::attack_ai_probability.static_probability)
	{
		return;
	}
	// ���ٸ���
	else if(probability >= GlobalValue::attack_ai_probability.static_probability && 
		probability < GlobalValue::attack_ai_probability.move_probability)
	{
		Follow();
	}
	// ��������
	else if (probability >= GlobalValue::attack_ai_probability.move_probability &&
		probability < GlobalValue::attack_ai_probability.attack_probability)
	{
		m_pHero->Attack();
	}
	// ħ����������
	else
	{
		m_pHero->MagicAttack();
	}
	return ;

}

/************************************************************************
*  ���Ӣ���Ƿ��Ѿ�����߽磬����Ӧ�ĵ���λ��
************************************************************************/
void GameAI::CheckAndAdjustPosition()
{
	// Ӣ�۵�ǰ��λ��
	Point currentPoint = m_pHero->getPosition();

	// ��ȡ��Ļ�Ĵ�С
	Size winsize = Director::getInstance()->getVisibleSize();

	// ���Ӣ�۲����ƶ�״̬�ģ������д���
	if (m_pHero->GetHeroState() == MOVE_STATE)
	{
		// �����߽�
		if (currentPoint.x >= winsize.width)
		{
			// ������Ӧ�ĵ���
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
		// �����߽�
		else if(currentPoint.x <= 0)
		{
			// ������Ӧ�ĵ���
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
*  ���ٶ���
************************************************************************/
void GameAI::Follow()
{
	// ���֣������Ӣ�ۣ�
	Hero* opponent = HeroManager::GetInstance()->GetCurrentHero();

	float x = m_pHero->getPosition().x - opponent->getPosition().x;

	float y = m_pHero->getPosition().y - opponent->getPosition().y;

	float distance = sqrt(pow(x,2) + pow(y,2));

	Point currentPoint = m_pHero->getPosition();

	// ���ٶ��ֵ�ʱ���ٶ�Ӧ�ñȶ�����
	int step = GlobalValue::hero_move_step / 2;

	// distance ��100��300֮��

	if (x > 0) //�������Լ������
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
	else if(x < 0) // �������Լ����ұ�
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

	// �����ʱӢ�۵�״̬�������ƶ��Ļ�����ô�ƶ���Ϊ����Ӣ�۵Ķ�������
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

	if (y > 0)  //�������Լ����±�
	{
		m_pHero->setPosition(Point(currentPoint.x ,currentPoint.y -  step));
	}
	else if(y < 0) // �������Լ����Ϸ�
	{
		m_pHero->setPosition(Point(currentPoint.x ,currentPoint.y +  step));
	}
}