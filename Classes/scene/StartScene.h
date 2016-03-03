#ifndef _START_SCENE_H_
#define _START_SCENE_H_

#include "cocos2d.h"

/************************************************************************
*   开始场景                                                                  
************************************************************************/
class StartScene:public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(StartScene);

	void jumpToMainScene(float t);									// 调到主场景
};

#endif //_MAIN_SCENE_H_