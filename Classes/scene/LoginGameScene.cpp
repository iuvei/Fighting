#include "LoginGameScene.h"
#include "ChooseGameScene.h"
#include "CreateOrJoinScene.h"
#include "RegisterGameScene.h"
#include "../CommonClass/Util.h"
#include "GameLogic.h"
#include <cocos2d.h>
#include <cocos-ext.h>
#include "ui\CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;

LoginGameScene::LoginGameScene()
{}

LoginGameScene::~LoginGameScene()
{}

Scene *LoginGameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoginGameScene::create();

	scene->addChild(layer);
	return scene;
}

bool LoginGameScene::init()
{
	if(!Layer::init())
	{
		return false;
	}//if

	this->visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	/*
	 * ������Ϸģʽѡ�񱳾�
   	 */
	addBackGroundSprite();
	/*
	 * ������Ϸģʽѡ��˵�
	 */
	//addChooseGameMenu();
	return true;
}

void LoginGameScene::addBackGroundSprite()
{
	//��¼��������屳��
	Scale9Sprite *backGroundSprite = Scale9Sprite::create("LFR/login/loginGameImage.png");	
	backGroundSprite->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2));
	this->addChild(backGroundSprite);

	//��ȡ��¼�������ǩ
	auto strings = FileUtils::getInstance()->getValueMapFromFile("fonts/loginInfo.xml"); 
	std::string loginName = strings["LOGIN_GAME"].asString();
	std::string cancelName = strings["CANCEL_GAME"].asString();
	std::string registerName = strings["REGISTER_GAME"].asString();
	std::string forgetPasswordName = strings["FORGET_PASSWORD"].asString();
	std::string selfLoginName = strings["SELF_LOGIN"].asString();
	std::string remPasswordName = strings["REM_PASSWORD"].asString();
	std::string inputUsername = strings["INPUT_USERNAME"].asString();
	std::string inputPassword = strings["INPUT_PASSWORD"].asString();

	//��¼Sprite
	Scale9Sprite *loginSprite= Scale9Sprite::create("LFR/login/menuImage.png");
	Scale9Sprite *loginPress = Scale9Sprite::create("LFR/login/menuImage_p.png");
	LabelTTF *loginLabel = LabelTTF::create(loginName, FONT_MENU, BUTTON_SIZE);
	ControlButton *loginGameBtn = ControlButton::create(loginLabel , loginSprite);
	Size buttonSize = Size(loginSprite->getContentSize().width ,loginSprite->getContentSize().height + 5 );
	//���ð��µ�Ч��
	loginGameBtn->setBackgroundSpriteForState(loginPress,Control::State::SELECTED);
	loginGameBtn->setPreferredSize(buttonSize);
	loginGameBtn->setPosition(Vec2(visibleSize.width/2 - 50, visibleSize.height/2 - 150));
	loginGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(LoginGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	loginGameBtn->setTag(Btn_Login_TAG);
	this->addChild(loginGameBtn);

	//���ȡ����¼
	Scale9Sprite *cancelSprite= Scale9Sprite::create("LFR/login/menuImage.png");
	Scale9Sprite *cancelPress = Scale9Sprite::create("LFR/login/menuImage_p.png");
	LabelTTF *cancelLabel = LabelTTF::create(cancelName, FONT_MENU, BUTTON_SIZE);
	ControlButton *cancelGameBtn = ControlButton::create(cancelLabel , cancelSprite);
	cancelGameBtn->setPreferredSize(buttonSize);
	cancelGameBtn->setPosition(Vec2(visibleSize.width/2 + 50, visibleSize.height/2 - 150));
	//���ð��µ�Ч��
	cancelGameBtn->setBackgroundSpriteForState(cancelPress,Control::State::SELECTED);
	//����Tag
	cancelGameBtn->setTag(Btn_Cancel_TAG);
	//���õ���ص�����
	cancelGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(LoginGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	this->addChild(cancelGameBtn);

	//ȡ����¼������ѡ��ҳ�Ĺرհ�ť
	Scale9Sprite *closeSprite = Scale9Sprite::create();
	ControlButton *closeGameBtn = ControlButton::create(closeSprite);
	closeGameBtn->setPreferredSize(Size(buttonSize.width /2 + 5 , buttonSize.height + 3 ));
	closeGameBtn->setPosition(Vec2(visibleSize.width/2 + 208 , visibleSize.height/2 + 190));
	closeGameBtn->setTag(Btn_Close_TAG);
	closeGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(LoginGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	this->addChild(closeGameBtn);


	//�û����ı�ǩ
	Scale9Sprite *userNameLabel = Scale9Sprite::create("LFR/login/userNameImage.png");
	userNameLabel->setPosition(Vec2(visibleSize.width/2 - 130 , visibleSize.height/2 - 20));
	Size labelSize = Size(userNameLabel->getContentSize().width , userNameLabel->getContentSize().height);
	this->addChild(userNameLabel);

	//�û����������
	Scale9Sprite *userNameText = Scale9Sprite::create("LFR/login/textBoxImage.png");
	Size textSize = Size(userNameText->getContentSize().width , userNameText->getContentSize().height);
	userNameEdit = EditBox::create(textSize,userNameText);
	userNameEdit->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2 - 20));
	userNameEdit->setTag(Edit_UserName_TAG);
	userNameEdit->setPlaceHolder(inputUsername.c_str());
	userNameEdit->setFontColor(Color3B(255 , 0 , 0));
	this->addChild(userNameEdit);

	//�����ǩ
	Scale9Sprite *passWordLabel = Scale9Sprite::create("LFR/login/passWordImage.png");
	passWordLabel->setPosition(Vec2(visibleSize.width/2 - 130 , visibleSize.height/2 - 70 ));
	passWordLabel->setPreferredSize(labelSize);
	this->addChild(passWordLabel);

	//���������
	Scale9Sprite *passWordText = Scale9Sprite::create("LFR/login/textBoxImage.png");
	passWordText->setPreferredSize(textSize);
	passWordEdit = EditBox::create(textSize,passWordText);
	passWordEdit->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2 - 70));
	passWordEdit->setTag(Edit_Password_TAG); //������������Ϊ���ķ�ʽ
	passWordEdit->setPlaceHolder(inputPassword.c_str());
	passWordEdit->setInputFlag(EditBox::InputFlag::PASSWORD);
	passWordEdit->setFontColor(Color3B(255 , 0 , 0));
	this->addChild(passWordEdit);

	//ע���˺ŵ�label
	LabelTTF * registerLabel = LabelTTF::create(registerName,FONT_MENU,LABEL_SIZE);
	registerLabel->setFontFillColor(Color3B(255 , 0 , 0));
	MenuItemLabel* registerGameItem = MenuItemLabel::create(registerLabel, [&](Ref* sender)
		{
			registerGame();
		}
	);
	Menu *registerMenu = Menu::create(registerGameItem , NULL);
	registerMenu->setPosition(Vec2(visibleSize.width/2 + 150 , visibleSize.height/2 - 20));
	this->addChild(registerMenu);

	//���������label
	LabelTTF * forgetPasswordLabel = LabelTTF::create(forgetPasswordName,FONT_MENU,LABEL_SIZE);
	forgetPasswordLabel->setFontFillColor(Color3B(255 , 0 , 0));
	MenuItemLabel* forgetPasswordItem = MenuItemLabel::create(forgetPasswordLabel, [&](Ref* sender)
		{
			forgetPassword();
		}
	);
	Menu *forgetPasswordMenu = Menu::create(forgetPasswordItem , NULL);
	forgetPasswordMenu->setPosition(Vec2(visibleSize.width/2 + 150 , visibleSize.height/2 - 70));
	this->addChild(forgetPasswordMenu);

	//��ס���� 
	LabelTTF *remPasswordLabel = LabelTTF::create(remPasswordName, FONT_MENU, LABEL_SIZE);
	remPasswordLabel->setPosition(Vec2(visibleSize.width/2 - 40, visibleSize.height/2 - 110));
	remPasswordLabel->setFontFillColor(Color3B(0, 0 , 0));
	this->addChild(remPasswordLabel);
	remPassword = cocos2d::ui::CheckBox::create();
	remPassword->setSelectedState(false);
	remPassword->setPosition(Vec2(visibleSize.width/2 - 60, visibleSize.height/2 - 110));
	this->addChild(remPassword);

	//�Զ���¼
	LabelTTF *selfLoginLabel = LabelTTF::create(selfLoginName, FONT_MENU, LABEL_SIZE);
	selfLoginLabel->setPosition(Vec2(visibleSize.width/2 + 70, visibleSize.height/2 - 110));
	selfLoginLabel->setFontFillColor(Color3B(0, 0 , 0));
	selfLogin = cocos2d::ui::CheckBox::create();
	selfLogin->setSelectedState(false);
	selfLogin->setPosition(Vec2(visibleSize.width/2 + 50, visibleSize.height/2 - 110));
	this->addChild(selfLoginLabel);
	this->addChild(selfLogin);
}

void LoginGameScene::menuPressHandle(Object *pScender , Control::EventType event)
{
	cocos2d::extension::ControlButton *btn = (cocos2d::extension::ControlButton *)pScender;
	int tag = btn->getTag();
	
	log(tag);

	switch (tag)
	{
	case Btn_Login_TAG:
		{
			log("login is running!");
			loginGame();
			break;
		}
	case Btn_Close_TAG:
		{
			auto scene = ChooseGameScene::createScene();
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(1,scene));
			break;
		}
	case Btn_Cancel_TAG:
		{
			auto scene = ChooseGameScene::createScene();
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(1,scene));
			break;
		}	
		default:
			log("non Button pressed");
			break;
	}//switch
}

void LoginGameScene::registerGame()
{
	log("register pressing!");
	auto scene = RegisterGameScene::createScene();
	Director::getInstance()->replaceScene(TransitionProgressOutIn::create(1,scene));
}

void LoginGameScene::forgetPassword()
{
	log("forget password pressing!");
}

void LoginGameScene::loginGame()
{
	//�������ӷ�����
	bool bOK = GameLogic::GetInstance()->ConnectToServer(GlobalValue::server_ip.c_str(),GlobalValue::server_port);
	if (bOK == false)
	{
		Util::Log("%s","���ӷ�����ʧ��");
		//���ӷ�����ʧ�ܣ����ص�¼����
		auto scene = LoginGameScene::createScene();
		Director::getInstance()->replaceScene(TransitionProgressOutIn::create(1,scene));	
	}
	//��¼��Ϣ����֤



	//��¼��֤�ɹ�������������Ϸ����
	//GameLogic::GetInstance()->IsSingleGame(false);
	auto scene = CreateOrJoinScene::createScene();
	Director::getInstance()->replaceScene(TransitionProgressOutIn::create(1,scene));
}