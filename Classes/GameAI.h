#ifndef _GAME_AI_H_
#define _GAME_AI_H_

#include "cocos2d.h"
USING_NS_CC;

class Hero;


/************************************************************************
*  AI��
************************************************************************/
class GameAI:public cocos2d::Node
{
public:
	void					MainProcess(Hero* pHero);							// �������߼�

protected:
	void					Run();															// ����Ѳ���ƶ�

	void					Action(bool& bIsInCheckDomain);				// AI����Ϊ

	void					CheckAndAdjustPosition();							// ����Լ����ڵ�λ�ã�������λ��

	void					Follow();														// ���Ŷ����ƶ�
private:
	Hero*				m_pHero;
};

#endif