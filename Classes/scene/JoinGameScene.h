#ifndef _JOIN_GAME_SCENE_H_
#define _JOIN_GAME_SCENE_H_

#include <cocos2d.h>
#include <cocos-ext.h>
#include "ui\CocosGUI.h"
#include <iostream>
#include <vector>

#include "Hero.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class JoinGameScene : public cocos2d::Layer
{
public:
	JoinGameScene();
	virtual ~JoinGameScene();
	static cocos2d::Scene *createScene();
	virtual bool init();

	CREATE_FUNC(JoinGameScene);

public:
	//添加加入游戏背景图片
	void addBackGroundSprite();

private:
	Vector<Hero *> heroList;


};
#endif
