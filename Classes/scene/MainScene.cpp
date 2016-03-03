#include "MainScene.h"
#include "GameDefine.h"
#include "GameLogic.h"
#include "./widget/HRocker.h"
USING_NS_CC;

//#define _US_PC_
#include "./widget/PlayButton.h"
#include "Hero.h"
#include "MagicManager.h"
#include "../../CommonClass/Util.h"



MainScene::MainScene()
{
	m_bCanMove = false;
	m_nDirection = directon_stay;
}

MainScene::~MainScene()
{
	this->unscheduleUpdate();  
	//this->unschedule(schedule_selector(HelloWorld::Move)); 
}

Scene* MainScene::createScene()
{
	auto scene = Scene::create();

	auto layer = MainScene::create();

	scene->addChild(layer);

	return scene;
}

bool MainScene::init()
{

	if ( !Layer::init() )
	{
		return false;
	}

	// ע���¼�(��ҪҪע���ˣ���Ϊ�¼���ҡ���д���)
	registerEvent();

	// ��Ϸ�߼���ʼ��
	
	LoadBackground();													// ���ر���ͼƬ
	LoadMenu();																// ���ز˵�

	CreateAttackButton();												// ������Ϸ��ť����ħ���İ�ť��
	CreateMagicAttackButton();
	//CreateHRocker("LFR/other/rocker.png","LFR/other/rockerBG.png",ccp(110,60));


	GameLogic::GetInstance()->StartGame(this);

	this->schedule(schedule_selector(MainScene::update),0.01f);
	//this->scheduleUpdate();

	return true;
}

/************************************************************************
*  ע���¼�
************************************************************************/
void MainScene::registerEvent()
{

#ifdef _USE_PC_ // ʹ�õ��Ե�ʱ��
	auto listenerKeyboard = EventListenerKeyboard::create();
	listenerKeyboard->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	listenerKeyboard->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeyboard, this);

	auto _mouseListener = EventListenerMouse::create();
	// ����Ҽ���Ϣ
	_mouseListener->onMouseUp = [=](Event *event)
	{
		EventMouse* e = (EventMouse*)event;
		onMouseUp(e);
	};
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener,this);//addEventListenerWithSceneGraphPriority(_mouseListener,this);
#else
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(false);

	listener->onTouchBegan = CC_CALLBACK_2(MainScene::touchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(MainScene::touchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(MainScene::touchEnded,this);
	listener->onTouchCancelled = CC_CALLBACK_2(MainScene::touchCancelled,this);
	_eventDispatcher->addEventListenerWithFixedPriority(listener,-255);
#endif // _US_PC_

	// ������ʱ��
	
	//
}

void MainScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}



/************************************************************************
*  ��ʼ������Ļ���ߵ������������û��ʵ�֣���Ϊ��Щ�¼�����ҡ��������д����
************************************************************************/
bool MainScene::touchBegan(Touch*touch,Event*event)
{
	return true;
}

void MainScene::touchMoved(Touch *touch,Event *event)
{
}

void MainScene::touchEnded(Touch *touch,Event *event)
{

}

void MainScene::touchCancelled(Touch*touch,Event*event)
{
}

/************************************************************************
*  ����Ҽ��Ĵ�����
************************************************************************/
void MainScene::onMouseUp(EventMouse* event)
{
}


void MainScene::update(float delta)
{
	if (!GameLogic::GetInstance()->IsGameRunning())
	{
		return;
	}

	if(m_bCanMove)
	{
		if (!GameLogic::GetInstance()->IsGameRunning())
		{
			return;
		}
		//HeroManager::GetInstance()->HeroMove(HeroManager::GetInstance()->GetCurrentHero()->GetHeroID(),(tagDirecton)m_nDirection);
		GameLogic::GetInstance()->HeroAction(MOVE_ACTION,m_nDirection);
	}

	GameLogic::GetInstance()->LoopGame();
}



/************************************************************************
*  ����ҡ��
************************************************************************/
void MainScene::CreateHRocker(const char *rockerImageName,const char *rockerBGImageName,Point position)
{
	auto Rocker = HRocker::createHRocker(rockerImageName,rockerBGImageName,position);//���е�һ��ͼƬΪҡ�˵İ�ť���ڶ���Ϊ����   "LFR/other/rocker.png","LFR/other/rockerBG.png",ccp(110,60)
	this->addChild(Rocker,2);
	Rocker->startRocker(true);
}

/************************************************************************
*  ������Ϸħ��play��ť
************************************************************************/
void MainScene::CreateMagicAttackButton()
{
	Size visibleSize = Director::getInstance()->getVisibleSize(); 
	auto button = PlayButton::create();
	button->InitButton("LFR/other/attack_play.png","LFR/other/attack_play.png");
	button->SetActionType(MAGICATTACK_ACTION);
	button->setPosition(Vec2(visibleSize.width-200,50));
	button->setScale(2);
	this->addChild(button, 1);  
}

/************************************************************************
*  ������Ϸ������play��ť
************************************************************************/
void MainScene::CreateAttackButton()
{
	Size visibleSize = Director::getInstance()->getVisibleSize(); 
	auto button = PlayButton::create();
	button->InitButton("LFR/other/attack_play.png","LFR/other/attack_play.png");
	button->SetActionType(ATTACK_ACTION);
	button->setPosition(Vec2(visibleSize.width-70,50));
	button->setScale(2);
	this->addChild(button, 1);  
}


/************************************************************************
*  ���ر���ͼƬ
************************************************************************/
void MainScene::LoadBackground()
{
	/*auto pUI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("DemoMap/DemoMap.ExportJson");
	pUI->setPosition(Point(0, 0));
	this->addChild(pUI);*/

	auto ary = FileUtils::getInstance()->getValueVectorFromFile("config/background.xml"); 

	for (int i = 0; i < ary.size();++i)
	{
		auto item = ary.at(i).asValueMap();
		std::string path = item["path"].asString();
		int xPos = item["x"].asInt();
		int yPos = item["y"].asInt();
		double scale = item["scale"].asDouble();

		auto sprite = Sprite::create(path.c_str());
		sprite->setAnchorPoint(Point(0,0));
		sprite->setPosition(Point(xPos,yPos));
		sprite->setScale(scale);

		this->addChild(sprite);

	}

	/*TMXTiledMap* pMap = TMXTiledMap::create("LFR/other/map/2.tmx");//LFR/other/map/
	this->addChild(pMap);*/

	/*//�����Ǽ�����صĴ��룺 
	auto rootNode = CSLoader::createNode("MainScene.csd");//����Studio2.x����Դ·�� 
	this->addChild(rootNode);//����this�Ǽ�����ʾ��scene */
}

/************************************************************************
*  ���ز˵�
************************************************************************/
void MainScene::LoadMenu()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MainScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
		origin.y + closeItem->getContentSize().height/2));

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

#ifdef _USE_PC_

void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_A:
		m_bCanMove = true;
		m_nDirection=directon_left;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		m_bCanMove = true;
		m_nDirection=directon_right;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		m_bCanMove = true;
		m_nDirection=directon_down;
		break;
	case EventKeyboard::KeyCode::KEY_W:
		m_bCanMove = true;
		m_nDirection=directon_up;
		break;
	case EventKeyboard::KeyCode::KEY_Q:
		{
			HeroManager::GetInstance()->HeroAttack(HeroManager::GetInstance()->GetCurrentHero()->GetHeroID(),HeroManager::GetInstance()->GetCurrentEnemy()->GetHeroID(),HeroManager::GetInstance()->GetCurrentHero()->GetFaceDirection());
		}

	default:
		break;
	}
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_A:
		m_bCanMove = false;
		m_nDirection=directon_stay;
		//HeroManager::GetInstance()->HeroMoveStop(HeroManager::GetInstance()->GetCurrentHero()->GetHeroID());
		GameLogic::GetInstance()->HeroAction(STATIC_ACTION,m_nDirection);
		break;
	case EventKeyboard::KeyCode::KEY_D:
		m_bCanMove = false;
		m_nDirection=directon_stay;
		GameLogic::GetInstance()->HeroAction(STATIC_ACTION,m_nDirection);
		break;
	case EventKeyboard::KeyCode::KEY_S:
		m_bCanMove = false;
		m_nDirection=directon_stay;
		GameLogic::GetInstance()->HeroAction(STATIC_ACTION,m_nDirection);
		break;
	case EventKeyboard::KeyCode::KEY_W:
		m_bCanMove = false;
		m_nDirection=directon_stay;
		GameLogic::GetInstance()->HeroAction(STATIC_ACTION,m_nDirection);
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		if (GameLogic::GetInstance()->GetSuspendState() == OUR_SUSPEND)
		{
			GameLogic::GetInstance()->ContinueGame();
		}
		else if(GameLogic::GetInstance()->GetSuspendState() == NOT_SUSPEND)
		{
			GameLogic::GetInstance()->SetSuspendState(OUR_SUSPEND);
			GameLogic::GetInstance()->SuspendGame(); //��ͣ��Ϸ
		}
		break;
	default:
		break;
	}
	m_bCanMove = false;
	m_nDirection=directon_stay;
}

#endif