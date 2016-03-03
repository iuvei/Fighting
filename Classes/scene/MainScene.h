#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"
#include "GameDefine.h"
#include "Hero.h"
USING_NS_CC;

class GameLogic;
class HRocker;

class MainScene:public cocos2d::Layer
{
public:
	MainScene();

	virtual ~MainScene();

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(MainScene);
public:
	void					registerEvent();													// 注册事件
	
	bool virtual		touchBegan(Touch*touch,Event*event);			// 触摸函数
	void virtual		touchMoved(Touch *touch,Event *event);			// 触摸移动函数
	void virtual		touchEnded(Touch *touch,Event *event);			// 触摸结束函数
	void virtual		touchCancelled(Touch*touch,Event *event);

#ifdef _USE_PC_ // 使用电脑的时候可以用键盘进行控制
	void					onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void					onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void					onMouseUp(EventMouse* e);
#endif // _USE_PC_
	

	HRocker*					GeHRocker();															// 获取游戏摇杆
	void							CreateHRocker(const char *rockerImageName,const char *rockerBGImageName,cocos2d::Point position);			// 创建摇杆
	void							LoadBackground();													// 加载背景图片
	void							LoadMenu();																// 加载菜单

	void							CreateAttackButton();												// 创建游戏按钮（放魔法的按钮）
	void							CreateMagicAttackButton();
	void							menuCloseCallback(cocos2d::Ref* pSender);
	void							jumpToMainScene(float t);
protected:
private:

	void					update(float delta);

	bool					m_bCanMove;

	int					m_nDirection;

};

#endif //_MAIN_SCENE_H_