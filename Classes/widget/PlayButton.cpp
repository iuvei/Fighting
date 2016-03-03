#include "PlayButton.h"
#include "GameDefine.h"
#include "GameLogic.h"
#include "Hero.h"
#include "HeroManager.h"

PlayButton::PlayButton()
{
	//m_pCallback = 0;
}
/*
void PlayButton::SetCallFun(FUN callback)
{
	m_pCallback = callback;
}*/

void PlayButton::SetActionType(int action_type)
{
	m_nActionType = action_type;
}

void PlayButton::touchBegin(Ref *pSender, Widget::TouchEventType type)
{
	Hero* pHero = HeroManager::GetInstance()->GetCurrentHero();

	GameLogic::GetInstance()->HeroAction(m_nActionType,pHero->GetFaceDirection());

	//(HeroManager::GetInstance()->*m_pCallback)(HeroManager::GetInstance()->GetCurrentHero()->GetHeroID(),HeroManager::GetInstance()->GetCurrentEnemy()->GetHeroID(),d);
}