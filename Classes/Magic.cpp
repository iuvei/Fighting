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
*  ����ħ����������ʾ��
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
*  ����ħ����������ʾ��
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
*  ����ħ����������ʾ��
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
*  ħ����Ч֮�����
************************************************************************/
void Magic::PlayOver()
{
	HeroManager::GetInstance()->GetHero(m_nHeroID)->MagicAttackEnd();
	m_bIsValid = false;
}

/************************************************************************
*  ħ���Ƿ���ʩ�ţ�����˵�Ƿ���Ч��
************************************************************************/
bool Magic::IsValid()
{
	return m_bIsValid;
}

/************************************************************************
*  ��ħ�������ĸ�Ӣ��
************************************************************************/
HERO_ID Magic::BelongTo()
{
	return m_nHeroID;
}


/************************************************************************
*  ����ħ��ʩ�ŵ���Ч����
*  ����ֵ��null
************************************************************************/
void Magic::SetMagicDistance(int d)
{
	m_nMagicDistance = d;
}

/************************************************************************
*  ��ȡħ��ʩ�ŵ���Ч����
*  ����ֵ����Ч����
************************************************************************/
int Magic::GetMagicDistance()
{
	return m_nMagicDistance;
}

/************************************************************************
*  ��ȡ��ȡʩ�ŵķ���
************************************************************************/
tagDirecton Magic::GetDirection()
{
	return m_Direction;
}

/************************************************************************
*  ��ȡ��ȡʩ�ŵķ���
************************************************************************/
void Magic::SetMagicDamage(int v)
{
	m_nMagicDamage = v;
}

/************************************************************************
*  ��ȡ��ȡʩ�ŵķ���
************************************************************************/
int Magic::GetMagicDamage()
{
	return m_nMagicDamage;
}