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
*	ѡ����Ϸģʽ����
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
	//�����Ϸģʽѡ�񱳾�ͼƬ
	void addBackGroundSprite();

	//�����Ϸģʽѡ��˵�
	void addChooseGameMenu();

	////��ť���������
	void menuPressHandle(Object *pScender , Control::EventType event);

	////�˳���Ϸ
	//void quitgame();
private:
	//�������ӻ���С
	Size visibleSize;
};
#endif