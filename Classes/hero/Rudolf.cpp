#include "Rudolf.h"

#include "cocos2d.h"
USING_NS_CC;

void Rudolf::SetAllAnimationFile()
{
	m_strHeroBigHeadFileName;
	m_strHeroBigHeadFileName = "LFR/rudolf/rudolf_f.png";


	m_strFacetoLeftStaticFileName = "LFR/rudolf/rudolf_0_mirror.png";//"LFR/bandit/bandit_s.png";
	m_strFacetoRightStaticFileName = "LFR/rudolf/rudolf_0.png";//"LFR/bandit/bandit_s.png";

	m_strFacetoLeftMoveFileName ="LFR/rudolf/rudolf_0_mirror.png";
	m_strFacetoRightMoveFileName = "LFR/rudolf/rudolf_0.png";

	m_strFacetoLeftAttackFileName = "LFR/rudolf/rudolf_2_mirror.png";
	m_strFacetoRightAttackFileName = "LFR/rudolf/rudolf_2.png";

	m_strFacetoLeftMagicAttackFileName;
	m_strFacetoRightMagicAttackFileName;

	m_strFacetoLeftParryFileName = "LFR/rudolf/rudolf_0_mirror.png";
	m_strFacetoRightParryFileName = "LFR/rudolf/rudolf_0.png";

	m_strFacetoLeftBeHitFileName;
	m_strFacetoRightBeHitFileName;

	m_strFacetoLeftGetUpFileName;
	m_strFacetoRightGetUpFileName;

	m_strFacetoLeftDieFileName = "LFR/rudolf/rudolf_0_mirror.png";
	m_strFacetoRightDieFileName ="LFR/rudolf/rudolf_0.png";
}

void Rudolf::SetFacetoLeftStaticAnimation()
{
	m_pSprite = Sprite::create(m_strFacetoLeftStaticFileName.c_str(),Rect(9 * 80,0,80,80));
	m_pSprite->setScale(2);
	this->addChild(m_pSprite,1,m_HeroID);
}

void Rudolf::SetFacetoRightStaticAnimation()
{
	// ´´½¨Ó¢ÐÛ¾«Áé

	m_pSprite = Sprite::create(m_strFacetoRightStaticFileName.c_str(),Rect(0,0,80,80));
	m_pSprite->setScale(2);
	this->addChild(m_pSprite,1,m_HeroID);
}

void Rudolf::SetFacetoLeftMoveAnimation()
{
	m_State = MOVE_STATE;
	DEFINE_ANIMATION_WITHOUT_CALLBACK(m_strFacetoLeftMoveFileName.c_str(),0,9,1,false,80,80,0.1,true,-2.5);
}

void Rudolf::SetFacetoRightMoveAnimation()
{
	m_State = MOVE_STATE;
	DEFINE_ANIMATION_WITHOUT_CALLBACK(m_strFacetoRightMoveFileName.c_str(),0,0,8,true,80,80,0.1,true,-2.5);
}

void Rudolf::SetFacetoLeftAttackAnimation()
{
	m_State = ATTACK_STATE;
	DEFINE_ANIMATION_WITH_CALLBACK(m_strFacetoLeftAttackFileName.c_str(),0,4,-1,false,152,94,0.1,false,0,Hero::AttackEnd);
}

void Rudolf::SetFacetoRightAttackAnimation()
{
	m_State = ATTACK_STATE;
	DEFINE_ANIMATION_WITH_CALLBACK(m_strFacetoRightAttackFileName.c_str(),0,0,5,true,152,94,0.1,false,0,Hero::AttackEnd);
}

void Rudolf::SetFacetoLeftMagicAttackAnimation(cocos2d::Sequence*& seq)
{
	m_State = MAGICATTACK_STATE;
	DEFINE_MOVE_MAGIC(seq,"LFR/freeze/freeze_ball_mirror.png",directon_left,GlobalValue::magic_distance,0,2,3,-1,false,82,83,0.1);

	
}

void Rudolf::SetFacetoRightMagicAttackAnimation(cocos2d::Sequence*& seq)
{
	m_State = MAGICATTACK_STATE;
	DEFINE_MOVE_MAGIC(seq,"LFR/freeze/freeze_ball.png",directon_right,GlobalValue::magic_distance,0,2,3,-1,false,82,83,0.1);
}

void Rudolf::SetFacetoLeftParryAnimation()
{
	DEFINE_ANIMATION_WITH_CALLBACK(m_strFacetoLeftParryFileName.c_str(),1,5,1,false,80,80,0.1,false,0,Hero::AttackEnd);
}

void Rudolf::SetFacetoRightParryAnimation()
{
	DEFINE_ANIMATION_WITH_CALLBACK(m_strFacetoRightParryFileName.c_str(),1,5,8,true,80,80,0.1,false,0,Hero::AttackEnd);
}

void Rudolf::SetFacetoLeftInjuredAnimation()
{
}

void Rudolf::SetFacetoRightInjuredAnimation()
{
}

void Rudolf::SetFacetoLeftBeHitAnimation()
{
}

void Rudolf::SetFacetoRightBeHitAnimation()
{
}

void Rudolf::SetFacetoLeftGetUpAnimation()
{
}

void Rudolf::SetFacetoRightGetUpAnimation()
{
}

void Rudolf::SetFacetoLeftDieAnimation()
{
	m_State = DEATH_STATE;
	DEFINE_ANIMATION_WITHOUT_CALLBACK(m_strFacetoRightDieFileName.c_str(),3,8,4,false,80,80,0.2,false,0);
}

void Rudolf::SetFacetoRightDieAnimation()
{
	m_State = DEATH_STATE;
	DEFINE_ANIMATION_WITHOUT_CALLBACK(m_strFacetoRightDieFileName.c_str(),3,1,5,true,80,80,0.2,false,0);
}