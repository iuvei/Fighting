#ifndef _PLAY_BUTTON_H_
#define _PLAY_BUTTON_H_

#include "XControlButton.h"
#include "GameDefine.h"
#include "GameLogic.h"
#include "HeroManager.h"

//typedef void (HeroManager::*FUN)(int action_type, tagDirecton d);

/************************************************************************
*   游戏控制按钮                                                                  
************************************************************************/
class PlayButton:public XControlButton
{
public:
	PlayButton();
	CREATE_FUNC(PlayButton);

	//void								SetCallFun(FUN callback);																				// 设置调用函数
	void								SetActionType(int action_type);

	virtual void					touchBegin(Ref *pSender, Widget::TouchEventType type);

private:
	//FUN								m_pCallback;																								// 按下该按钮的时候，调用的函数
	int								m_nActionType;																							
};
#endif