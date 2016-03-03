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
*  构造函数
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
*  析构函数
************************************************************************/
Hero::~Hero(void)
{
}


/************************************************************************
*  初始化函数
*  参数：
*  @id ：需要初始化的英雄的ID
*	@hero_direction：英雄的方位（在屏幕的左边还是右边）
*	@face_to：英雄的面部朝向（朝左还是朝右）
*	@bAI：该英雄是否具有人工智能（有人工智能的英雄不需要玩家控制）
*  返回值：null
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

	// 设置所有的动画文件变量名
	SetAllAnimationFile();

	m_HeroID = id;

	// 脸的朝向
	m_nFaceTo = face_to;

	// 是否具有AI
	m_bUseAI = bAI;

	// 英雄状态
	m_State = STATIC_STATE;

	if (m_nFaceTo == directon_left)
	{
		SetFacetoLeftStaticAnimation();
	}
	else if(m_nFaceTo == directon_right)
	{
		SetFacetoRightStaticAnimation();
	}

	// 设置英雄站立的位置
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

	// 创建头像（左上角和右上角的头像：这里使用按钮控件实现，也可以用其他控件实现）
	CreateHeadButton(hero_direction);

	// 创建血条
	CreateBloodView(hero_direction);

	// 创建魔法条
	CreateMagicView(hero_direction);

	GameLogic::GetInstance()->GetMainScene()->addChild(this);
}

/************************************************************************
*  创建人物的大头像按钮
*  参数：
*  @d ：创建头像的方位（左边或者右边）
*  返回值：null
************************************************************************/
void Hero::CreateHeadButton(tagDirecton d)
{
	// 创建头像
	Size size = Director::getInstance()->getVisibleSize();

	m_pHeadButton = XControlButton::create();

	m_pHeadButton->InitButton(m_strHeroBigHeadFileName.c_str());

	// 设置为主场景的儿子
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
*  创建血条
*  参数：
*  @d：创建血条的地方（左边或者右边）
*  返回值：null
************************************************************************/
void Hero::CreateBloodView(tagDirecton d)
{
	// 创建血条
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
*  创建魔法条
*  参数：
*  @d：创建魔法条的地方（左边或者右边）
*  返回值：null
************************************************************************/
void Hero::CreateMagicView(tagDirecton d)
{
	// 创建血条
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
*  开启人工智能
************************************************************************/
void Hero::UseAI(bool b)
{
	m_bUseAI = b;
}

/************************************************************************
*  是否具有人工智能
************************************************************************/
bool Hero::UseAI()
{
	return m_bUseAI;
}

/************************************************************************
*  英雄移动
************************************************************************/
void Hero::Run(tagDirecton directon)
{
	m_nPreDirection = m_nCurrentDirection;

	m_nCurrentDirection = directon;

	int step = GlobalValue::hero_move_step;

	Point currentPoint = this->getPosition();

	m_State = MOVE_STATE;

	// 设置动画播放
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
	default: // 向上或向下移动
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
	// 设置英雄移动

	
}

/************************************************************************
*  停止动作
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

	//恢复精灵原来的初始化贴图 
	this->removeChild(m_pSprite,TRUE);//把原来的精灵删除掉

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
*  获取英雄当前运动的方向
************************************************************************/
tagDirecton Hero::GetCurrentDirection()
{
	return m_nCurrentDirection;
}

/************************************************************************
*  获取英雄前一个动作时的运动方向
************************************************************************/
tagDirecton Hero::GetPreDirection()
{
	return m_nPreDirection;
}

/************************************************************************
*  获取英雄的面部朝向
*  返回值：面部朝向
************************************************************************/
tagDirecton Hero::GetFaceDirection()
{
	return m_nFaceTo;
}

/************************************************************************
*  设置英雄的面部朝向
*  返回值：void
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
*  设置英雄的血量
*  返回值：null
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
*  设置英雄的魔法值
*  返回值：null
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
*  获取英雄的血量
*  返回值：血量
************************************************************************/
float Hero::GetBloodValue()
{
	return m_fBlood;
}

/************************************************************************
*  获取英雄的魔法值
*  返回值：血量
************************************************************************/
float Hero::GetMagicValue()
{
	return m_fMagic;
}

/************************************************************************
*  获取英雄的ID
*  返回值：ID
************************************************************************/
HERO_ID Hero::GetHeroID()
{
	return m_HeroID;
}

/************************************************************************
*  获取英雄的名字
*  返回值：英雄的名字
************************************************************************/
const char* Hero::GetHeroName()
{
	return m_strHeroName.c_str();
}

/************************************************************************
*  英雄死亡
*  返回值：null
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
*  魔法攻击
*  返回值：null
************************************************************************/
void Hero::MagicAttack()
{
	// 魔法值不够
	if (m_fMagic < GlobalValue::magic_cost)
	{
		return;
	}

	StopAnimation();

	// 魔法攻击状态
	m_State = MAGICATTACK_STATE;

	// 获取魔法动画的序列
	Sequence* seq = NULL;

	if (m_nFaceTo == directon_left)
	{
		SetFacetoLeftMagicAttackAnimation(seq);
	}
	else
	{
		SetFacetoRightMagicAttackAnimation(seq);
	}

	// 设置魔法值
	SetMagicValue(m_fMagic - GlobalValue::magic_cost);

	// 创建一个魔法
	auto mag = Magic::create();

	// 加入到魔法队列中
	MagicManager::GetInstance()->InsertMagic(mag);

	// 施放魔法
	mag->PlayMagic(m_HeroID,seq);

}

/************************************************************************
*  物理攻击
*  返回值：null
************************************************************************/
void Hero::Attack()
{
	StopAnimation();

	m_State = ATTACK_STATE;

	// 对方英雄
	Hero* opponent = 0;

	// 获取英雄列表
	vector<Hero*> allHero = HeroManager::GetInstance()->GetHeroList();

	// 无论哪个对方哪个英雄在他的攻击范围内，都能受到伤害
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

		// 攻击是否有效
		bool bAttackValid = false;

		float x = getPosition().x - opponent->getPosition().x;

		float y = getPosition().y - opponent->getPosition().y;

		// 两个英雄之间的距离
		float distance = sqrt(pow(x,2) + pow(y,2));

		// 对手在自己的左边，而且英雄面部朝左，那么攻击是有效的
		if (x >= 0 && m_nFaceTo == directon_left) 
		{
			bAttackValid = true;
		}
		// 对手在自己的右边，而且英雄面部朝左，那么攻击是有效的
		else if(x <= 0 && m_nFaceTo == directon_right)
		{
			bAttackValid = true;
		}
		// 其他的情况都是无效的
		else
		{
			// 这里返回的原因是：英雄必须面对着队对手进行攻击才有效，否则无效
			bAttackValid = false;
		}

		// 在有效的攻击距离之间
		if (bAttackValid && distance <= GlobalValue::attack_damage_distance)
		{
			float probability = Util::GetProbability();

			// 格挡
			/*if (probability <= GlobalValue::parry_probability)
			{
			Parry();
			}
			else*/
			{
				tagDirecton faceto = opponent->GetFaceDirection();

				// 播放被击中时候的动画
				if (faceto == directon_left)
				{
					opponent->SetFacetoLeftBeHitAnimation();
				}
				else
				{
					opponent->SetFacetoRightBeHitAnimation();
				}

				// 设置血量
				opponent->SetBloodValue(opponent->GetBloodValue() - GlobalValue::attack_damage);

				// 播放被击中后重新站起来的动画
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
*  格挡
*  返回值：null
************************************************************************/
void Hero::Parry()
{
	StopAnimation();

	// 播放格挡的动画
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
*  物理攻击完毕后调用
*  返回值：null
************************************************************************/
void Hero::AttackEnd()
{
	StopAnimation();

	m_State = STATIC_STATE;

	this->removeChild(m_pSprite,TRUE);//把原来的精灵删除掉

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
*  移动完毕后调用
*  返回值：null
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
*  放完魔法后调用
*  返回值：null
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
*  获取英雄的状态
************************************************************************/
HERO_STATE Hero::GetHeroState()
{
	return m_State;
}

/************************************************************************
*  设置英雄的状态
************************************************************************/
void Hero::SetHeroState(HERO_STATE state)
{
	m_State = state;
}
