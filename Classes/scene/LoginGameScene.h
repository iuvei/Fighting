#ifndef _LOGIN_GAME_SCENE_H_
#define _LOGIN_GAME_SCENE_H_

#include <cocos2d.h>
#include <cocos-ext.h>
#include "ui\CocosGUI.h"
#include <string>

#define LABEL_SIZE 20
#define BUTTON_SIZE   20

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;

const int Btn_Login_TAG = 10001;
const int Btn_Register_TAG = 10002;
const int Btn_Rem_Password_TAG = 10003;
const int Btn_Forget_Password_TAG = 10004;
const int Edit_UserName_TAG = 10005;
const int Edit_Password_TAG = 10006;
const int Btn_Close_TAG = 10007;
const int Btn_Cancel_TAG = 10008;

class LoginGameScene : public cocos2d::Layer
{
public:
	LoginGameScene();
	virtual ~LoginGameScene();
	static cocos2d::Scene *createScene();
	virtual bool init();
	//void menuCloseCallBack(cocos2d::Ref * pScender);

	CREATE_FUNC(LoginGameScene);
public:
	/*
	 * 设置游戏登录背景
   	 */
	void addBackGroundSprite();
	/*
	 * 创建游戏按钮点击处理函数
	 */
	void menuPressHandle(Object *pScender , Control::EventType event);
	void registerGame();
	void forgetPassword();
	void loginGame();
	/*
	 * 创建警告提示框
	 */
	//void showAlert();

private:
	Size visibleSize;
	cocos2d::ui::EditBox *userNameEdit;
	cocos2d::ui::EditBox *passWordEdit;
	cocos2d::ui::CheckBox *remPassword;
	cocos2d::ui::CheckBox *selfLogin;
};

#endif