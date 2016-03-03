#include "Magic.h"
#include "GameLogic.h"
#include "Hero.h"
#include "HeroManager.h"

USING_NS_CC;
////////////////////////
Magic::Magic()
{
	m_pMagic = 0;
	m_bIsValid = false;
	m_Direction = directon_stay;
	m_nHeroID = INVALID_ID;
	m_nMagicDistance = 100;
	m_nMagicDamage = GlobalValue::magic_damage;
	
}

Magic::~Magic()
{

}

bool Magic::init()
{
	return true;
}

/************************************************************************
*  创建魔法动画（显示）
************************************************************************/
void Magic::PlayMagic(HERO_ID id,cocos2d::Sequence* seq)
{
	m_nHeroID = id;
	Hero* pHero = HeroManager::GetInstance()->GetHero(id);
	Point p = pHero->getPosition();
	m_Direction = pHero->GetFaceDirection();

	auto magic = Sprite::create();
	this->addChild(magic);
	this->setPosition(p);
	auto sequence = Sequence::create(seq,CallFunc::create(CC_CALLBACK_0(Magic::PlayOver,this)),NULL);
	magic->runAction(seq);
	m_bIsValid = true;
}

/************************************************************************
*  创建魔法动画（显示）
************************************************************************/
void Magic::PlayMagic(cocos2d::Sequence* seq,Point point)
{
	m_nHeroID = INVALID_ID;

	auto magic = Sprite::create();
	this->addChild(magic);
	this->setPosition(point);
	auto sequence = Sequence::create(seq,NULL);
	magic->runAction(seq);
	m_bIsValid = true;
}

/************************************************************************
*  创建魔法动画（显示）
************************************************************************/
void Magic::PlayMagic(const string& strFileName,Point point)
{
	m_nHeroID = INVALID_ID;

	auto magic = Sprite::create(strFileName.c_str());
	this->addChild(magic);
	this->setPosition(point);
	m_bIsValid = true;
}

/************************************************************************
*  魔法无效之后调用
************************************************************************/
void Magic::PlayOver()
{
	HeroManager::GetInstance()->GetHero(m_nHeroID)->MagicAttackEnd();
	m_bIsValid = false;
}

/************************************************************************
*  魔法是否还在施放（或者说是否还有效）
************************************************************************/
bool Magic::IsValid()
{
	return m_bIsValid;
}

/************************************************************************
*  该魔法属于哪个英雄
************************************************************************/
HERO_ID Magic::BelongTo()
{
	return m_nHeroID;
}


/************************************************************************
*  设置魔法施放的有效距离
*  返回值：null
************************************************************************/
void Magic::SetMagicDistance(int d)
{
	m_nMagicDistance = d;
}

/************************************************************************
*  获取魔法施放的有效距离
*  返回值：有效距离
************************************************************************/
int Magic::GetMagicDistance()
{
	return m_nMagicDistance;
}

/************************************************************************
*  获取获取施放的方向
************************************************************************/
tagDirecton Magic::GetDirection()
{
	return m_Direction;
}

/************************************************************************
*  获取获取施放的方向
************************************************************************/
void Magic::SetMagicDamage(int v)
{
	m_nMagicDamage = v;
}

/************************************************************************
*  获取获取施放的方向
************************************************************************/
int Magic::GetMagicDamage()
{
	return m_nMagicDamage;
}