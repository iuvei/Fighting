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
	void					registerEvent();													// ע���¼�
	
	bool virtual		touchBegan(Touch*touch,Event*event);			// ��������
	void virtual		touchMoved(Touch *touch,Event *event);			// �����ƶ�����
	void virtual		touchEnded(Touch *touch,Event *event);			// ������������
	void virtual		touchCancelled(Touch*touch,Event *event);

#ifdef _USE_PC_ // ʹ�õ��Ե�ʱ������ü��̽��п���
	void					onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void					onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void					onMouseUp(EventMouse* e);
#endif // _USE_PC_
	

	HRocker*					GeHRocker();															// ��ȡ��Ϸҡ��
	void							CreateHRocker(const char *rockerImageName,const char *rockerBGImageName,cocos2d::Point position);			// ����ҡ��
	void							LoadBackground();													// ���ر���ͼƬ
	void							LoadMenu();																// ���ز˵�

	void							CreateAttackButton();												// ������Ϸ��ť����ħ���İ�ť��
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