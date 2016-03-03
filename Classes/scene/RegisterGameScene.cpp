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
	 * ������Ϸע�ᱳ��
   	 */
	addBackGroundSprite();

	return true;
}

void RegisterGameScene::addBackGroundSprite()
{
	//ע���������屳��
	Scale9Sprite *backGroundSprite = Scale9Sprite::create("LFR/register/registerBackground.png");	
	backGroundSprite->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2));
	this->addChild(backGroundSprite);

	//��ȡע��������ǩ
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

	//ע�ᰴťʵ��
	Scale9Sprite *registerSprite= Scale9Sprite::create("LFR/login/menuImage.png");
	Scale9Sprite *registerPress = Scale9Sprite::create("LFR/login/menuImage_p.png");
	LabelTTF *registerLabel = LabelTTF::create(registerName, FONT_MENU, BUTTON_SIZE);
	ControlButton *registerGameBtn = ControlButton::create(registerLabel , registerSprite);
	Size buttonSize = Size(registerSprite->getContentSize().width ,registerSprite->getContentSize().height + 3 );
	//���ð��µ�Ч��
	registerGameBtn->setBackgroundSpriteForState(registerPress,Control::State::SELECTED);
	registerGameBtn->setPreferredSize(buttonSize);
	registerGameBtn->setPosition(Vec2(visibleSize.width/2 - 50, visibleSize.height/2 - 200));
	registerGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(RegisterGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	registerGameBtn->setTag(Btn_Register_TAG);
	this->addChild(registerGameBtn);

	//���ȡ��ע��
	Scale9Sprite *cancelSprite= Scale9Sprite::create("LFR/login/menuImage.png");
	Scale9Sprite *cancelPress = Scale9Sprite::create("LFR/login/menuImage_p.png");
	LabelTTF *cancelLabel = LabelTTF::create(cancelName, FONT_MENU, BUTTON_SIZE);
	ControlButton *cancelGameBtn = ControlButton::create(cancelLabel , cancelSprite);
	cancelGameBtn->setPreferredSize(buttonSize);
	cancelGameBtn->setPosition(Vec2(visibleSize.width/2 + 50, visibleSize.height/2 - 200));
	//���ð��µ�Ч��
	cancelGameBtn->setBackgroundSpriteForState(cancelPress,Control::State::SELECTED);
	//����Tag
	cancelGameBtn->setTag(Btn_Cancel_TAG);
	//���õ���ص�����
	cancelGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(RegisterGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	this->addChild(cancelGameBtn);


	//����ѡ��ҳ�Ĺرհ�ť
	Scale9Sprite *closeSprite = Scale9Sprite::create();
	ControlButton *closeGameBtn = ControlButton::create(closeSprite);
	closeGameBtn->setPreferredSize(Size(buttonSize.width /2 + 6 , buttonSize.height + 6 ));
	closeGameBtn->setPosition(Vec2(visibleSize.width/2 + 208 , visibleSize.height/2 + 235));
	closeGameBtn->setTag(Btn_Close_TAG);
	closeGameBtn->addTargetWithActionForControlEvents(this,cccontrol_selector(RegisterGameScene::menuPressHandle),
		Control::EventType::TOUCH_DOWN);
	this->addChild(closeGameBtn);

	//�û����ı�ǩ
	LabelTTF *userNameLabel = LabelTTF::create(userName, FONT_MENU, LABEL_SIZE);
	userNameLabel->setPosition(Vec2(visibleSize.width/2 - 140, visibleSize.height/2 + 30));
	userNameLabel->setFontFillColor(Color3B(0 , 0 , 0));
	this->addChild(userNameLabel);
	//�û����������
	Scale9Sprite *userNameText = Scale9Sprite::create("LFR/register/textBoxImage.png");
	Size textSize = Size(userNameText->getContentSize().width , userNameText->getContentSize().height);
	userNameEdit = EditBox::create(textSize,userNameText);
	userNameEdit->setPosition(Vec2(visibleSize.width/2 + 20 , visibleSize.height/2 + 30));
	userNameEdit->setTag(Edit_UserName_TAG);
	userNameEdit->setPlaceHolder(inputUsername.c_str());
	userNameEdit->setFontColor(Color3B(255 , 255 , 0));
	this->addChild(userNameEdit);
	//�Ա�
	LabelTTF *SexNameLabel = LabelTTF::create(sexName, FONT_MENU, LABEL_SIZE);
	SexNameLabel->setPosition(Vec2(visibleSize.width/2 - 140, visibleSize.height/2 - 20));
	SexNameLabel->setFontFillColor(Color3B(0 , 0 , 0));
	this->addChild(SexNameLabel);


	//�����ǩ
	LabelTTF *passWordLabel = LabelTTF::create(passwordName, FONT_MENU, LABEL_SIZE);
	passWordLabel->setPosition(Vec2(visibleSize.width/2 - 140, visibleSize.height/2 - 70));
	passWordLabel->setFontFillColor(Color3B(0 , 0 , 0));
	this->addChild(passWordLabel);
	//���������
	Scale9Sprite *passWordText = Scale9Sprite::create("LFR/register/textBoxImage.png");
	passWordText->setPreferredSize(textSize);
	passWordEdit = EditBox::create(textSize,passWordText);
	passWordEdit->setPosition(Vec2(visibleSize.width/2 + 20, visibleSize.height/2 - 70));
	passWordEdit->setTag(Edit_Password_TAG); //������������Ϊ���ķ�ʽ
	passWordEdit->setPlaceHolder(inputPassword.c_str());
	passWordEdit->setInputFlag(EditBox::InputFlag::PASSWORD);
	passWordEdit->setFontColor(Color3B(255 , 255 , 0));
	this->addChild(passWordEdit);

	//ȷ�������ǩ
	LabelTTF *passWordRepeatLabel = LabelTTF::create(passwordRepeatName, FONT_MENU, LABEL_SIZE);
	passWordRepeatLabel->setPosition(Vec2(visibleSize.width/2 - 140, visibleSize.height/2 - 120));
	passWordRepeatLabel->setFontFillColor(Color3B(0 , 0 , 0));
	this->addChild(passWordRepeatLabel);
	//ȷ�����������
	Scale9Sprite *passWordRepeatText = Scale9Sprite::create("LFR/register/textBoxImage.png");
	passWordRepeatText->setPreferredSize(textSize);
	passWordRepeatEdit = EditBox::create(textSize,passWordRepeatText);
	passWordRepeatEdit->setPosition(Vec2(visibleSize.width/2 + 20, visibleSize.height/2 - 120));
	passWordRepeatEdit->setTag(Edit_Password_TAG); //������������Ϊ���ķ�ʽ
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
