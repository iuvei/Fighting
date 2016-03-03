#ifndef _RESULT_SCENE_H_
#define _RESULT_SCENE_H_

#include "cocos2d.h"

/************************************************************************
*   ¿ªÊ¼³¡¾°                                                                  
************************************************************************/
class ResultScene:public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(ResultScene);

	void CreateMenu();
};

#endif //_MAIN_SCENE_H_