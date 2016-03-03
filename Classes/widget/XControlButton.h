#ifndef _CONTROL_BUTTON_H_
#define _CONTROL_BUTTON_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace std;
USING_NS_CC;
using namespace cocos2d::ui;

/************************************************************************
*   控制按钮                                                                   
************************************************************************/
class XControlButton:public cocos2d::Node
{
public:
	XControlButton();

	CREATE_FUNC(XControlButton);

	void								InitButton(const char* nomalButtonFile);										// 根据图片初始化按钮
	void								InitButton(const char* nomalButtonFile,const char* pressedButtonFile);
	void								InitButton(const char* nomalButtonFile,const char* pressedButtonFile,const char* disabledButtonFile);

	Size								GetSize();																						// 获取按钮的尺寸

	//================子类需要实现这些虚函数 begin======================
	virtual void					touchBegin(Ref *pSender, Widget::TouchEventType type);				// 各种事件
	virtual void					touchMoved(Ref *pSender, Widget::TouchEventType type);
	virtual void					touchEnded(Ref *pSender, Widget::TouchEventType type);
	virtual void					touchCanceled(Ref *pSender, Widget::TouchEventType type);
	//================子类需要实现这些虚函数 end=======================
private:
	void								BindButtonEven();																			// 注册事件

	void								touchEvent(Ref *pSender, Widget::TouchEventType type);


private:
	ui::Button*					m_pButton;																					// 实际的按钮
	string							m_strPressedButtonFile;																// 按钮按下时显示的图片
	string							m_strNomalButtonFile;																	// 正常时的图片
	string							m_strDisabledButtonFile;																// 按钮失效时的图片
	bool								isTouch;
};
#endif