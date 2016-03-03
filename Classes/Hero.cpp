#include "Hero.h"
#include "GameDefine.h"
#include "./widget/XControlButton.h"
#include "GameLogic.h"
#include "./scene/MainScene.h"
#include "./widget/ProgressView.h"
#include "../CommonClass/Util.h"
#include "GameAI.h"
#include "Magic.h"
#include "HeroManager.h"
#include "MagicManager.h"

USING_NS_CC;

/************************************************************************
*  ���캯��
************************************************************************/
Hero::Hero(void)
{
	m_pSprite = 0;
	m_pHeadButton = 0;
	m_pSequence = 0;
	m_pLifebloodProgress = 0;
	m_pMagicProgress = 0;
	m_nPreDirection = directon_stay;
	m_nCurrentDirection = directon_stay;
	m_nFaceTo = directon_right;
	m_fBlood = 100;
	m_bUseAI = false;
	m_fMagic = 100;
	m_State = INVALID_STATE;
}

/************************************************************************
*  ��������
************************************************************************/
Hero::~Hero(void)
{
}


/************************************************************************
*  ��ʼ������
*  ������
*  @id ����Ҫ��ʼ����Ӣ�۵�ID
*	@hero_direction��Ӣ�۵ķ�λ������Ļ����߻����ұߣ�
*	@face_to��Ӣ�۵��沿���򣨳����ǳ��ң�
*	@bAI����Ӣ���Ƿ�����˹����ܣ����˹����ܵ�Ӣ�۲���Ҫ��ҿ��ƣ�
*  ����ֵ��null
************************************************************************/
void Hero::InitHero(HERO_ID id,tagDirecton hero_direction,tagDirecton  face_to,bool bAI)
{
	switch (id)
	{
	case BANDIT_ID:
		m_strHeroName = BANDIT_NAME;
		break;
	case BAT_ID:
		m_strHeroName = BAT_NAME;
		break;
	case DAVIS_ID:
		m_strHeroName = DAVIS_NAME;
		break;
	case DEEP_ID:
		m_strHeroName = DEEP_NAME;
		break;
	case DENNIS_ID:
		m_strHeroName = DENNIS_NAME;
		break;
	case FIREN_ID:
		m_strHeroName = FIREN_NAME;
		break;
	case FIRZEN_ID:
		m_strHeroName = FIRZEN_NAME;
		break;
	case FREEZE_ID:
		m_strHeroName = FREEZE_NAME;
		break;
	case HENRY_ID:
		m_strHeroName = HENRY_NAME;
		break;
	case HUNTER_ID:
		m_strHeroName = HUNTER_NAME;
		break;
	case JACK_ID:
		m_strHeroName = JACK_NAME;
		break;
	case JAN_ID:
		m_strHeroName = JAN_NAME;
		break;
	case JOHN_ID:
		m_strHeroName = JOHN_NAME;
		break;
	case JULIAN_ID:
		m_strHeroName = JULIAN_NAME;
		break;
	case JUSTIN_ID:
		m_strHeroName = JUSTIN_NAME;
		break;
	case KNIGHT_ID:
		m_strHeroName = KNIGHT_NAME;
		break;
	case LOUIS_ID:
		m_strHeroName = LOUIS_NAME;
		break;
	case LOUISEX_ID:
		m_strHeroName = LOUISEX_NAME;
		break;
	case MARK_ID:
		m_strHeroName = MARK_NAME;
		break;
	case MONK_ID:
		m_strHeroName = MONK_NAME;
		break;
	case RUDOLF_ID:
		m_strHeroName = RUDOLF_NAME;
		break;
	case SORCERER_ID:
		m_strHeroName = SORCERER_NAME;
		break;
	case WOODY_ID:
		m_strHeroName = WOODY_NAME;
		break;
	default:
		break;
	}

	// �������еĶ����ļ�������
	SetAllAnimationFile();

	m_HeroID = id;

	// ���ĳ���
	m_nFaceTo = face_to;

	// �Ƿ����AI
	m_bUseAI = bAI;

	// Ӣ��״̬
	m_State = STATIC_STATE;

	if (m_nFaceTo == directon_left)
	{
		SetFacetoLeftStaticAnimation();
	}
	else if(m_nFaceTo == directon_right)
	{
		SetFacetoRightStaticAnimation();
	}

	// ����Ӣ��վ����λ��
	if (hero_direction == directon_left)
	{
		Size size = Director::getInstance()->getVisibleSize();

		Size spriteSize = m_pSprite->getContentSize();

		this->setPosition(Point(0 + spriteSize.width ,240));

		setPosition(Point(0 + spriteSize.width ,240));
	}
	else if(hero_direction == directon_right)
	{
		Size size = Director::getInstance()->getVisibleSize();

		Size spriteSize = m_pSprite->getContentSize();

		this->setPosition(Point(size.width - spriteSize.width,240));

		setPosition(Point(size.width - spriteSize.width,240));
	}

	// ����ͷ�����ϽǺ����Ͻǵ�ͷ������ʹ�ð�ť�ؼ�ʵ�֣�Ҳ�����������ؼ�ʵ�֣�
	CreateHeadButton(hero_direction);

	// ����Ѫ��
	CreateBloodView(hero_direction);

	// ����ħ����
	CreateMagicView(hero_direction);

	GameLogic::GetInstance()->GetMainScene()->addChild(this);
}

/************************************************************************
*  ��������Ĵ�ͷ��ť
*  ������
*  @d ������ͷ��ķ�λ����߻����ұߣ�
*  ����ֵ��null
************************************************************************/
void Hero::CreateHeadButton(tagDirecton d)
{
	// ����ͷ��
	Size size = Director::getInstance()->getVisibleSize();

	m_pHeadButton = XControlButton::create();

	m_pHeadButton->InitButton(m_strHeroBigHeadFileName.c_str());

	// ����Ϊ�������Ķ���
	GameLogic::GetInstance()->GetMainScene()->addChild(m_pHeadButton,2);

	Size headSize = m_pHeadButton->GetSize();

	if (directon_left == d)
	{
		m_pHeadButton->setPosition(Point( headSize.width/2,size.height - headSize.height / 2));
	}
	else if(directon_right == d)
	{
		m_pHeadButton->setPosition(Point(size.width - headSize.width/2,size.height - headSize.height / 2));
	}
}

/************************************************************************
*  ����Ѫ��
*  ������
*  @d������Ѫ���ĵط�����߻����ұߣ�
*  ����ֵ��null
************************************************************************/
void Hero::CreateBloodView(tagDirecton d)
{
	// ����Ѫ��
	Size size = Director::getInstance()->getVisibleSize();

	Size headSize = m_pHeadButton->GetSize();

	m_pLifebloodProgress = ProgressView::create();

	m_pLifebloodProgress->InitProgress("LFR/other/xue_back.png","LFR/other/xue_fore.png",100,100);

	Size bloodSize = m_pLifebloodProgress->GetSize();

	m_pLifebloodProgress->setScale(1.5);  

	bloodSize = m_pLifebloodProgress->GetSize();

	if (directon_left == d)
	{
		m_pLifebloodProgress->setAnchorPoint(Point(0,0));

		m_pLifebloodProgress->setPosition(Point(0+bloodSize.width / 2 + 20,size.height - headSize.height - bloodSize.height));  
	}
	else if(directon_right == d)
	{
		m_pLifebloodProgress->setAnchorPoint(Point(0,0));

		m_pLifebloodProgress->setPosition(Point(size.width - headSize.width/2,size.height - headSize.height));  
	}

	GameLogic::GetInstance()->GetMainScene()->addChild(m_pLifebloodProgress, 2);
}

/************************************************************************
*  ����ħ����
*  ������
*  @d������ħ�����ĵط�����߻����ұߣ�
*  ����ֵ��null
************************************************************************/
void Hero::CreateMagicView(tagDirecton d)
{
	// ����Ѫ��
	Size size = Director::getInstance()->getVisibleSize();

	Size headSize = m_pHeadButton->GetSize();

	m_pMagicProgress = ProgressView::create();

	m_pMagicProgress->InitProgress("LFR/other/magic_back.png","LFR/other/magic_fore.png",100,100);

	Size bloodSize = m_pMagicProgress->GetSize();

	m_pMagicProgress->setScale(1.5);  

	bloodSize = m_pLifebloodProgress->GetSize();

	Size magicSize = m_pMagicProgress->GetSize();

	if (directon_left == d)
	{
		m_pMagicProgress->setAnchorPoint(Point(0,0));

		m_pMagicProgress->setPosition(Point(0+bloodSize.width / 2  + 20,size.height - headSize.height - bloodSize.height - magicSize.height));  
	}
	else if(directon_right == d)
	{
		m_pMagicProgress->setAnchorPoint(Point(0,0));

		m_pMagicProgress->setPosition(Point(size.width - headSize.width/2,size.height - headSize.height - magicSize.height));  
	}

	GameLogic::GetInstance()->GetMainScene()->addChild(m_pMagicProgress, 2);
}

/************************************************************************
*  �����˹�����
************************************************************************/
void Hero::UseAI(bool b)
{
	m_bUseAI = b;
}

/************************************************************************
*  �Ƿ�����˹�����
************************************************************************/
bool Hero::UseAI()
{
	return m_bUseAI;
}

/************************************************************************
*  Ӣ���ƶ�
************************************************************************/
void Hero::Run(tagDirecton directon)
{
	m_nPreDirection = m_nCurrentDirection;

	m_nCurrentDirection = directon;

	int step = GlobalValue::hero_move_step;

	Point currentPoint = this->getPosition();

	m_State = MOVE_STATE;

	// ���ö�������
	switch (directon)
	{
	case directon_left:
		{
			m_nFaceTo = directon_left;

			SetFacetoLeftMoveAnimation();
		}
		break;
	case directon_right:
		{
			m_nFaceTo = directon_right;

			SetFacetoRightMoveAnimation();
		}
		break;
	default: // ���ϻ������ƶ�
		{
			if (m_nFaceTo == directon_left && GetHeroState() != MOVE_STATE)
			{
				SetFacetoLeftMoveAnimation();
			}
			else if(m_nFaceTo == directon_right && GetHeroState() != MOVE_STATE)
			{
				SetFacetoRightMoveAnimation();
			}
			else if (GetHeroState() != MOVE_STATE)
			{
				m_nFaceTo = directon_right;

				SetFacetoRightMoveAnimation();
			}
		}
		break;
	}
	// ����Ӣ���ƶ�

	
}

/************************************************************************
*  ֹͣ����
************************************************************************/
void Hero::StopAnimation()
{
	if (m_State == STATIC_STATE)
	{
		return;
	}

	m_State = STATIC_STATE;


	m_pSprite->stopAction(m_pSequence);

	m_pSprite->stopAllActions();

	//�ָ�����ԭ���ĳ�ʼ����ͼ 
	this->removeChild(m_pSprite,TRUE);//��ԭ���ľ���ɾ����

	if (m_nFaceTo == directon_left)
	{
		SetFacetoLeftStaticAnimation();
	}
	else
	{
		SetFacetoRightStaticAnimation();
	}
}

/************************************************************************
*  ��ȡӢ�۵�ǰ�˶��ķ���
************************************************************************/
tagDirecton Hero::GetCurrentDirection()
{
	return m_nCurrentDirection;
}

/************************************************************************
*  ��ȡӢ��ǰһ������ʱ���˶�����
************************************************************************/
tagDirecton Hero::GetPreDirection()
{
	return m_nPreDirection;
}

/************************************************************************
*  ��ȡӢ�۵��沿����
*  ����ֵ���沿����
************************************************************************/
tagDirecton Hero::GetFaceDirection()
{
	return m_nFaceTo;
}

/************************************************************************
*  ����Ӣ�۵��沿����
*  ����ֵ��void
************************************************************************/
void Hero::SetFaceDirection(tagDirecton directon)
{
	m_nFaceTo = directon;

	switch (m_nFaceTo)
	{
	case directon_left:
		SetFacetoLeftStaticAnimation();
		break;
	case directon_right:
		SetFacetoRightStaticAnimation();
		break;
	default:
		break;
	}
}

/************************************************************************
*  ����Ӣ�۵�Ѫ��
*  ����ֵ��null
************************************************************************/
void Hero::SetBloodValue(float f)
{
	if (f < 0.0)
	{
		m_fBlood = 0;
	}
	else if(f > 100)
	{
		m_fBlood = 100;
	}
	else
	{
		m_fBlood = f;
	}

	m_pLifebloodProgress->SetCurrentProgress(m_fBlood);

	if (m_fBlood <= 0.0 && m_State != DEATH_STATE)
	{
		Die();
		GameLogic::GetInstance()->StopGame();
	}
}

/************************************************************************
*  ����Ӣ�۵�ħ��ֵ
*  ����ֵ��null
************************************************************************/
void Hero::SetMagicValue(float f)
{
	if (f < 0.0)
	{
		m_fMagic = 0;
	}
	else
	{
		m_fMagic = f;
	}

	m_pMagicProgress->SetCurrentProgress(m_fMagic);
}

/************************************************************************
*  ��ȡӢ�۵�Ѫ��
*  ����ֵ��Ѫ��
************************************************************************/
float Hero::GetBloodValue()
{
	return m_fBlood;
}

/************************************************************************
*  ��ȡӢ�۵�ħ��ֵ
*  ����ֵ��Ѫ��
************************************************************************/
float Hero::GetMagicValue()
{
	return m_fMagic;
}

/************************************************************************
*  ��ȡӢ�۵�ID
*  ����ֵ��ID
************************************************************************/
HERO_ID Hero::GetHeroID()
{
	return m_HeroID;
}

/************************************************************************
*  ��ȡӢ�۵�����
*  ����ֵ��Ӣ�۵�����
************************************************************************/
const char* Hero::GetHeroName()
{
	return m_strHeroName.c_str();
}

/************************************************************************
*  Ӣ������
*  ����ֵ��null
************************************************************************/
void Hero::Die()
{
	m_pSprite->stopAllActions();

	SetMagicValue(0);

	m_State = DEATH_STATE;

	if (m_nFaceTo == directon_left)
	{
		SetFacetoLeftDieAnimation();
	}
	else
	{
		SetFacetoRightDieAnimation();
	}
	GameLogic::GetInstance()->JumpToScene(RESULT_SCENE);
}

/************************************************************************
*  ħ������
*  ����ֵ��null
************************************************************************/
void Hero::MagicAttack()
{
	// ħ��ֵ����
	if (m_fMagic < GlobalValue::magic_cost)
	{
		return;
	}

	StopAnimation();

	// ħ������״̬
	m_State = MAGICATTACK_STATE;

	// ��ȡħ������������
	Sequence* seq = NULL;

	if (m_nFaceTo == directon_left)
	{
		SetFacetoLeftMagicAttackAnimation(seq);
	}
	else
	{
		SetFacetoRightMagicAttackAnimation(seq);
	}

	// ����ħ��ֵ
	SetMagicValue(m_fMagic - GlobalValue::magic_cost);

	// ����һ��ħ��
	auto mag = Magic::create();

	// ���뵽ħ��������
	MagicManager::GetInstance()->InsertMagic(mag);

	// ʩ��ħ��
	mag->PlayMagic(m_HeroID,seq);

}

/************************************************************************
*  ������
*  ����ֵ��null
************************************************************************/
void Hero::Attack()
{
	StopAnimation();

	m_State = ATTACK_STATE;

	// �Է�Ӣ��
	Hero* opponent = 0;

	// ��ȡӢ���б�
	vector<Hero*> allHero = HeroManager::GetInstance()->GetHeroList();

	// �����ĸ��Է��ĸ�Ӣ�������Ĺ�����Χ�ڣ������ܵ��˺�
	for (int i = 0; i < allHero.size(); i++)
	{
		opponent = allHero[i];
		if (opponent == 0)
		{
			continue;
		}

		if (opponent->GetHeroID() == m_HeroID)
		{
			continue;
		}

		// �����Ƿ���Ч
		bool bAttackValid = false;

		float x = getPosition().x - opponent->getPosition().x;

		float y = getPosition().y - opponent->getPosition().y;

		// ����Ӣ��֮��ľ���
		float distance = sqrt(pow(x,2) + pow(y,2));

		// �������Լ�����ߣ�����Ӣ���沿������ô��������Ч��
		if (x >= 0 && m_nFaceTo == directon_left) 
		{
			bAttackValid = true;
		}
		// �������Լ����ұߣ�����Ӣ���沿������ô��������Ч��
		else if(x <= 0 && m_nFaceTo == directon_right)
		{
			bAttackValid = true;
		}
		// ���������������Ч��
		else
		{
			// ���ﷵ�ص�ԭ���ǣ�Ӣ�۱�������ŶӶ��ֽ��й�������Ч��������Ч
			bAttackValid = false;
		}

		// ����Ч�Ĺ�������֮��
		if (bAttackValid && distance <= GlobalValue::attack_damage_distance)
		{
			float probability = Util::GetProbability();

			// ��
			/*if (probability <= GlobalValue::parry_probability)
			{
			Parry();
			}
			else*/
			{
				tagDirecton faceto = opponent->GetFaceDirection();

				// ���ű�����ʱ��Ķ���
				if (faceto == directon_left)
				{
					opponent->SetFacetoLeftBeHitAnimation();
				}
				else
				{
					opponent->SetFacetoRightBeHitAnimation();
				}

				// ����Ѫ��
				opponent->SetBloodValue(opponent->GetBloodValue() - GlobalValue::attack_damage);

				// ���ű����к�����վ�����Ķ���
				if (opponent->GetBloodValue() > 0)
				{
					if (faceto == directon_left)
					{
						opponent->SetFacetoLeftGetUpAnimation();
					}
					else
					{
						opponent->SetFacetoRightGetUpAnimation();
					}
				}
			}

		}

		if (GetFaceDirection() == directon_left)
		{

			SetFacetoLeftAttackAnimation();
		}
		else
		{

			SetFacetoRightAttackAnimation();
		}
	}


}

/************************************************************************
*  ��
*  ����ֵ��null
************************************************************************/
void Hero::Parry()
{
	StopAnimation();

	// ���Ÿ񵲵Ķ���
	if (GetFaceDirection() == directon_left)
	{
		SetFacetoLeftParryAnimation();
	}
	else
	{
		SetFacetoRightParryAnimation();
	}
}

/************************************************************************
*  ��������Ϻ����
*  ����ֵ��null
************************************************************************/
void Hero::AttackEnd()
{
	StopAnimation();

	m_State = STATIC_STATE;

	this->removeChild(m_pSprite,TRUE);//��ԭ���ľ���ɾ����

	if (GetFaceDirection() == directon_left)
	{
		SetFacetoLeftStaticAnimation();
	}
	else
	{
		SetFacetoRightStaticAnimation();
	}
}

/************************************************************************
*  �ƶ���Ϻ����
*  ����ֵ��null
************************************************************************/
void Hero::RunEnd()
{
	StopAnimation();

	m_State = STATIC_STATE;

	if (GetFaceDirection() == directon_left)
	{
		m_nFaceTo = directon_left;

		SetFacetoLeftStaticAnimation();
	}
	else
	{
		m_nFaceTo = directon_right;

		SetFacetoRightStaticAnimation();
	}
}

/************************************************************************
*  ����ħ�������
*  ����ֵ��null
************************************************************************/
void Hero::MagicAttackEnd()
{
	StopAnimation();

	m_State = MAGICATTACK_STATE;

	if (GetFaceDirection() == directon_left)
	{
		m_nFaceTo = directon_right;

		SetFacetoRightStaticAnimation();
	}
	else
	{
		m_nFaceTo = directon_left;

		SetFacetoLeftStaticAnimation();
	}
}

/************************************************************************
*  ��ȡӢ�۵�״̬
************************************************************************/
HERO_STATE Hero::GetHeroState()
{
	return m_State;
}

/************************************************************************
*  ����Ӣ�۵�״̬
************************************************************************/
void Hero::SetHeroState(HERO_STATE state)
{
	m_State = state;
}
