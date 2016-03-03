#ifndef _START_SCENE_H_
#define _START_SCENE_H_

#include "cocos2d.h"

/************************************************************************
*   ��ʼ����                                                                  
************************************************************************/
class StartScene:public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(StartScene);

	void jumpToMainScene(float t);									// ����������
};

#endif //_MAIN_SCENE_H_