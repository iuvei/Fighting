#ifndef _CHOOSE_GAME_SCENE_H_
#define _CHOOSE_GAME_SCENE_H_
#include "MainScene.h"
#include <cocos2d.h>
#include <cocos-ext.h>
#include "ui\CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define FONT_MENU     "Arial"
#define MENU_SIZE  40

const int Btn_Single_Game_TAG = 10001;
const int Btn_Net_Game_TAG = 10002;
const int Btn_Help_Game_TAG = 10003;
const int Btn_Quit_Game_TAG = 10004;
/******************************************
*	选择游戏模式场景
*******************************************/
class ChooseGameScene:public cocos2d::Layer
{
public:
	ChooseGameScene();
	virtual ~ChooseGameScene();
	static cocos2d::Scene *createScene();
	virtual bool init();
	//void menuCloseCallBack(cocos2d::Ref * pScender);

	CREATE_FUNC(ChooseGameScene);

public:
	//添加游戏模式选择背景图片
	void addBackGroundSprite();

	//添加游戏模式选择菜单
	void addChooseGameMenu();

	////按钮点击处理函数
	void menuPressHandle(Object *pScender , Control::EventType event);

	////退出游戏
	//void quitgame();
private:
	//场景可视化大小
	Size visibleSize;
};
#endif