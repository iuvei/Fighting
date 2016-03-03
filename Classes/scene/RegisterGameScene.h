#ifndef REGISTER_GAME_SCENE_H_
#define REGISTER_GAME_SCENE_H_

#include <cocos2d.h>
#include <cocos-ext.h>
#include "ui\CocosGUI.h"
#include <string>

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;

class RegisterGameScene : public cocos2d::Layer
{
public:
	RegisterGameScene();
	virtual ~RegisterGameScene();
	static cocos2d::Scene *createScene();
	virtual bool init();

	CREATE_FUNC(RegisterGameScene);

public:
	/**
     * ���ע����汳��
	 */
	void addBackGroundSprite();
		/*
	 * ������Ϸ��ť���������
	 */
	void menuPressHandle(Object *pScender , Control::EventType event);
private:
	Size visibleSize;
	cocos2d::ui::EditBox *userNameEdit;
	cocos2d::ui::EditBox *passWordEdit;
	cocos2d::ui::EditBox *passWordRepeatEdit;
};

#endif