#include "XControlButton.h"
#include "GameLogic.h"
#include "Hero.h"

XControlButton::XControlButton()
{
	m_pButton = 0;
	isTouch = false;
}

/************************************************************************
*   初始化按钮                                                                
************************************************************************/
void XControlButton::InitButton(const char* nomalButtonFile)
{
	InitButton(nomalButtonFile,0,0);
}

void XControlButton::InitButton(const char* nomalButtonFile,const char* pressedButtonFile)
{
	InitButton(nomalButtonFile,pressedButtonFile,0);
}

void XControlButton::InitButton(const char* nomalButtonFile,const char* pressedButtonFile,const char* disabledButtonFile)
{
	if (pressedButtonFile == 0)
	{
		if (disabledButtonFile == 0)
		{
			m_pButton = ui::Button::create(nomalButtonFile);
			m_strNomalButtonFile = nomalButtonFile;
		}
		else
		{
			m_pButton = ui::Button::create(nomalButtonFile,pressedButtonFile);
			m_strNomalButtonFile = nomalButtonFile;
			m_strPressedButtonFile = pressedButtonFile;
		}
	}
	else
	{
		if (disabledButtonFile == 0)
		{
			m_pButton = ui::Button::create(nomalButtonFile,pressedButtonFile);
			m_strNomalButtonFile = nomalButtonFile;
			m_strPressedButtonFile = pressedButtonFile;
		}
		else
		{
			m_pButton = ui::Button::create(nomalButtonFile,pressedButtonFile,disabledButtonFile);
			m_strNomalButtonFile = nomalButtonFile;
			m_strPressedButtonFile = pressedButtonFile;
			m_strDisabledButtonFile = disabledButtonFile;
		}
		
	}
	this->addChild(m_pButton);
	BindButtonEven();
}

Size XControlButton::GetSize()
{
	if (0 == m_pButton)
	{
		return Size(0,0);
	}

	return m_pButton->getContentSize();
}

void XControlButton::BindButtonEven()
{
	if (m_pButton == 0)
	{
		return;
	}

	m_pButton->addTouchEventListener(CC_CALLBACK_2(XControlButton::touchEvent, this));
}

/************************************************************************
*   按钮的事件                                                               
************************************************************************/
void XControlButton::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
	isTouch=true;

	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		touchBegin(pSender,type);
		break;

	case Widget::TouchEventType::MOVED:
		touchMoved(pSender,type);
		break;

	case Widget::TouchEventType::ENDED:
		touchEnded(pSender,type);
		break;

	case Widget::TouchEventType::CANCELED:
		touchCanceled(pSender,type);
		break;

	default:
		break;
	}

}

/************************************************************************
*   子类需要实现这些函数以达到处理的目的                                                                
************************************************************************/
void XControlButton::touchBegin(Ref *pSender, Widget::TouchEventType type)
{
}

void XControlButton::touchMoved(Ref *pSender, Widget::TouchEventType type)
{

}

void XControlButton::touchEnded(Ref *pSender, Widget::TouchEventType type)
{

}

void XControlButton::touchCanceled(Ref *pSender, Widget::TouchEventType type)
{

}