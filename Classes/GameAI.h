#ifndef _GAME_AI_H_
#define _GAME_AI_H_

#include "cocos2d.h"
USING_NS_CC;

class Hero;


/************************************************************************
*  AI类
************************************************************************/
class GameAI:public cocos2d::Node
{
public:
	void					MainProcess(Hero* pHero);							// 主处理逻辑

protected:
	void					Run();															// 来回巡逻移动

	void					Action(bool& bIsInCheckDomain);				// AI的行为

	void					CheckAndAdjustPosition();							// 检查自己所在的位置，并调整位置

	void					Follow();														// 跟着对手移动
private:
	Hero*				m_pHero;
};

#endif