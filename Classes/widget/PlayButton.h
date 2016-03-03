#ifndef _PLAY_BUTTON_H_
#define _PLAY_BUTTON_H_

#include "XControlButton.h"
#include "GameDefine.h"
#include "GameLogic.h"
#include "HeroManager.h"

//typedef void (HeroManager::*FUN)(int action_type, tagDirecton d);

/************************************************************************
*   ��Ϸ���ư�ť                                                                  
************************************************************************/
class PlayButton:public XControlButton
{
public:
	PlayButton();
	CREATE_FUNC(PlayButton);

	//void								SetCallFun(FUN callback);																				// ���õ��ú���
	void								SetActionType(int action_type);

	virtual void					touchBegin(Ref *pSender, Widget::TouchEventType type);

private:
	//FUN								m_pCallback;																								// ���¸ð�ť��ʱ�򣬵��õĺ���
	int								m_nActionType;																							
};
#endif