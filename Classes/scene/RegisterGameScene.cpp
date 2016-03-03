#include "LoginGameScene.h"
#include "RegisterGameScene.h"
#include "ChooseGameScene.h"
#include <cocos2d.h>
#include <cocos-ext.h>
#include "ui\CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;

RegisterGameScene::RegisterGameScene()
{}

RegisterGameScene::~RegisterGameScene()
{}


Scene *RegisterGameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = RegisterGameScene::create();

	scene->addChild(layer);
	return scene;
}

bool RegisterGameScene::init()
{
	if(!Layer::init())
	{
		return false;
	}//if

	this->visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	/*
	 * 设置游戏注册背景
   	 */
	addBackGroundSprite();

	return true;
}

void RegisterGameScene::addBackGroundSprite()
{
	//注册界面的整体背景
	Scale9Sprite *backGroundSprite = Scale9Sprite::create("LFR/register/registerBackground.png");	
	backGroundSprite->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2));
	this->addChild(backGroundSprite);

	//读取注册界面各标签
	auto strings = FileUtils::getInstance()->getValueMapFromFile("fonts/registerInfo.xml"); 
	std::string registerName = strings["REGISTER_GAME"].asString();
	std::string cancelName = strings["CANCEL_GAME"].asString();
	std::string userName = strings["USER_NAME"].asString();
	std::string passwordName = strings["PASSWORD"].asString();
	std::string passwordRepeatName = strings["PASSWORDREPEAT"].asString();
	std::string sexName = strings["SEX"].asString();
	std::string inputUsername = strings["INPUT_USERNAME"].asString();
	std::string inputPassword = strings["INPUT_PASSWORD"].asString();
	std::string inputPasswordAgain = strings["INPUT_PASSWORD_AGAIN"].asString();

	//注册按钮实现
	Scale9Sprite *registerSprite= Scale9Sprite::create("LFR/login/menuImage.png");
	Scale9Sprite *registerPress = Scale9Sprite::create("LFR/login/menuImage_p.png");
	LabelTTF *registerLabel = LabelTTF::create(registerName, FONT_MENU, BUTTON_SIZE);
	ControlButton *registerGameBtn = ControlButton::create(registerLabel , registerSprite);
	Size buttonSize = Size(registerSprite->getContentSize().width ,registerSprite->getContentSize().height + 3 );
	//设置按下的效果
	registerGameBtn->setBackgroundSpriteForState(registerPress,Control::State::SELECTED);
	registerGameBtn->setPreferredSize(buttonSize);
	registerGameBtn->setPosition(Vec2(visibleSize.width/2 - 50, visibleSize.height/2 - 200));
	registerGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(RegisterGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	registerGameBtn->setTag(Btn_Register_TAG);
	this->addChild(registerGameBtn);

	//添加取消注册
	Scale9Sprite *cancelSprite= Scale9Sprite::create("LFR/login/menuImage.png");
	Scale9Sprite *cancelPress = Scale9Sprite::create("LFR/login/menuImage_p.png");
	LabelTTF *cancelLabel = LabelTTF::create(cancelName, FONT_MENU, BUTTON_SIZE);
	ControlButton *cancelGameBtn = ControlButton::create(cancelLabel , cancelSprite);
	cancelGameBtn->setPreferredSize(buttonSize);
	cancelGameBtn->setPosition(Vec2(visibleSize.width/2 + 50, visibleSize.height/2 - 200));
	//设置按下的效果
	cancelGameBtn->setBackgroundSpriteForState(cancelPress,Control::State::SELECTED);
	//设置Tag
	cancelGameBtn->setTag(Btn_Cancel_TAG);
	//设置点击回调方法
	cancelGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(RegisterGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	this->addChild(cancelGameBtn);


	//返回选择页的关闭按钮
	Scale9Sprite *closeSprite = Scale9Sprite::create();
	ControlButton *closeGameBtn = ControlButton::create(closeSprite);
	closeGameBtn->setPreferredSize(Size(buttonSize.width /2 + 6 , buttonSize.height + 6 ));
	closeGameBtn->setPosition(Vec2(visibleSize.width/2 + 208 , visibleSize.height/2 + 235));
	closeGameBtn->setTag(Btn_Close_TAG);
	closeGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(RegisterGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	this->addChild(closeGameBtn);

	//用户名的标签
	LabelTTF *userNameLabel = LabelTTF::create(userName, FONT_MENU, LABEL_SIZE);
	userNameLabel->setPosition(Vec2(visibleSize.width/2 - 140, visibleSize.height/2 + 30));
	userNameLabel->setFontFillColor(Color3B(0 , 0 , 0));
	this->addChild(userNameLabel);
	//用户名的输入框
	Scale9Sprite *userNameText = Scale9Sprite::create("LFR/register/textBoxImage.png");
	Size textSize = Size(userNameText->getContentSize().width , userNameText->getContentSize().height);
	userNameEdit = EditBox::create(textSize,userNameText);
	userNameEdit->setPosition(Vec2(visibleSize.width/2 + 20 , visibleSize.height/2 + 30));
	userNameEdit->setTag(Edit_UserName_TAG);
	userNameEdit->setPlaceHolder(inputUsername.c_str());
	userNameEdit->setFontColor(Color3B(255 , 255 , 0));
	this->addChild(userNameEdit);
	//性别
	LabelTTF *SexNameLabel = LabelTTF::create(sexName, FONT_MENU, LABEL_SIZE);
	SexNameLabel->setPosition(Vec2(visibleSize.width/2 - 140, visibleSize.height/2 - 20));
	SexNameLabel->setFontFillColor(Color3B(0 , 0 , 0));
	this->addChild(SexNameLabel);


	//密码标签
	LabelTTF *passWordLabel = LabelTTF::create(passwordName, FONT_MENU, LABEL_SIZE);
	passWordLabel->setPosition(Vec2(visibleSize.width/2 - 140, visibleSize.height/2 - 70));
	passWordLabel->setFontFillColor(Color3B(0 , 0 , 0));
	this->addChild(passWordLabel);
	//密码输入框
	Scale9Sprite *passWordText = Scale9Sprite::create("LFR/register/textBoxImage.png");
	passWordText->setPreferredSize(textSize);
	passWordEdit = EditBox::create(textSize,passWordText);
	passWordEdit->setPosition(Vec2(visibleSize.width/2 + 20, visibleSize.height/2 - 70));
	passWordEdit->setTag(Edit_Password_TAG); //设置密码输入为密文方式
	passWordEdit->setPlaceHolder(inputPassword.c_str());
	passWordEdit->setInputFlag(EditBox::InputFlag::PASSWORD);
	passWordEdit->setFontColor(Color3B(255 , 255 , 0));
	this->addChild(passWordEdit);

	//确认密码标签
	LabelTTF *passWordRepeatLabel = LabelTTF::create(passwordRepeatName, FONT_MENU, LABEL_SIZE);
	passWordRepeatLabel->setPosition(Vec2(visibleSize.width/2 - 140, visibleSize.height/2 - 120));
	passWordRepeatLabel->setFontFillColor(Color3B(0 , 0 , 0));
	this->addChild(passWordRepeatLabel);
	//确认密码输入框
	Scale9Sprite *passWordRepeatText = Scale9Sprite::create("LFR/register/textBoxImage.png");
	passWordRepeatText->setPreferredSize(textSize);
	passWordRepeatEdit = EditBox::create(textSize,passWordRepeatText);
	passWordRepeatEdit->setPosition(Vec2(visibleSize.width/2 + 20, visibleSize.height/2 - 120));
	passWordRepeatEdit->setTag(Edit_Password_TAG); //设置密码输入为密文方式
	passWordRepeatEdit->setPlaceHolder(inputPasswordAgain.c_str());
	passWordRepeatEdit->setInputFlag(EditBox::InputFlag::PASSWORD);
	passWordRepeatEdit->setFontColor(Color3B(255 , 255 , 0));
	this->addChild(passWordRepeatEdit);


}


void RegisterGameScene::menuPressHandle(Object *pScender , Control::EventType event)
{
	cocos2d::extension::ControlButton *btn = (cocos2d::extension::ControlButton *)pScender;
	int tag = btn->getTag();
	
	log(tag);

	switch (tag)
	{
	case Btn_Register_TAG:
		{
			log("Register is running!");
			
			break;
		}
	case Btn_Close_TAG:
		{
			auto scene = LoginGameScene::createScene();
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(1,scene));
			break;
		}
	case Btn_Cancel_TAG:
		{
			auto scene = LoginGameScene::createScene();
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(1,scene));
			break;
		}	
		default:
			log("non Button pressed");
			break;
	}//switch
}
