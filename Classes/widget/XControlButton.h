#ifndef _CONTROL_BUTTON_H_
#define _CONTROL_BUTTON_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace std;
USING_NS_CC;
using namespace cocos2d::ui;

/************************************************************************
*   ���ư�ť                                                                   
************************************************************************/
class XControlButton:public cocos2d::Node
{
public:
	XControlButton();

	CREATE_FUNC(XControlButton);

	void								InitButton(const char* nomalButtonFile);										// ����ͼƬ��ʼ����ť
	void								InitButton(const char* nomalButtonFile,const char* pressedButtonFile);
	void								InitButton(const char* nomalButtonFile,const char* pressedButtonFile,const char* disabledButtonFile);

	Size								GetSize();																						// ��ȡ��ť�ĳߴ�

	//================������Ҫʵ����Щ�麯�� begin======================
	virtual void					touchBegin(Ref *pSender, Widget::TouchEventType type);				// �����¼�
	virtual void					touchMoved(Ref *pSender, Widget::TouchEventType type);
	virtual void					touchEnded(Ref *pSender, Widget::TouchEventType type);
	virtual void					touchCanceled(Ref *pSender, Widget::TouchEventType type);
	//================������Ҫʵ����Щ�麯�� end=======================
private:
	void								BindButtonEven();																			// ע���¼�

	void								touchEvent(Ref *pSender, Widget::TouchEventType type);


private:
	ui::Button*					m_pButton;																					// ʵ�ʵİ�ť
	string							m_strPressedButtonFile;																// ��ť����ʱ��ʾ��ͼƬ
	string							m_strNomalButtonFile;																	// ����ʱ��ͼƬ
	string							m_strDisabledButtonFile;																// ��ťʧЧʱ��ͼƬ
	bool								isTouch;
};
#endif