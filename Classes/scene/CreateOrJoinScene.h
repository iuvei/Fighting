#ifndef _CREATEORJOIN_SCENE_H_
#define _CREATEORJOIN_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
USING_NS_CC;

#include "GameDefine.h"

class CreateOrJoinScene:public cocos2d::Layer
{
public:
	CreateOrJoinScene();

	virtual ~CreateOrJoinScene();

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(CreateOrJoinScene);
public:

	//������Ϸ������
	static void			jumpToMainScene(float t);
	//����������Ϸ��ѡ��˵�
	void			CreateMenu();
	//����������Ϸ����
	void			LoadBackGround();
	void			CreateGame();
	void			JoinGame();

	void			ExitGame();
};

#endif