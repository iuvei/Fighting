#include "GameDefine.h"
#include "HRocker.h"
#include "GameLogic.h"
#include "Hero.h"
#include "HeroManager.h"

const double PI=3.1415;

HRocker::HRocker(void)
{
	rocketRun=false;

}

HRocker::~HRocker(void)
{
	//
	this->unscheduleUpdate();  
}


bool HRocker::init()
{

	if ( !Layer::init() )
	{
		return false;
	}

	return true;
}

//����ҡ��(ҡ�˵Ĳ�����ͼƬ��Դ����ҡ�˱���ͼƬ��Դ������ʼ����)
HRocker* HRocker::createHRocker(const char *rockerImageName,const char *rockerBGImageName,Point position)
{
	HRocker *layer = HRocker::create();
	if (layer)
	{
		layer->rockerInit(rockerImageName,rockerBGImageName,position);
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

//�Զ����ʼ������
void HRocker::rockerInit(const char* rockerImageName,const char* rockerBGImageName,Point position)
{
	Sprite *spRockerBG = Sprite::create(rockerBGImageName);
	spRockerBG->setPosition(position);
	spRockerBG->setVisible(false);
	addChild(spRockerBG,0,tag_rockerBG);

	Sprite *spRocker = Sprite::create(rockerImageName);
	spRocker->setPosition(position);
	spRocker->setVisible(false);
	addChild(spRocker,1,tag_rocker);

	rockerBGPosition = position;
	rockerBGR = spRockerBG->getContentSize().width*0.5;//
	rocketDirection=-1;//��ʾҡ�˷��򲻱�
}

//����ҡ��(��ʾҡ�ˡ�����ҡ�˴����¼�)
void HRocker::startRocker(bool _isStopOther)
{
	Sprite *rocker = (Sprite*)this->getChildByTag(tag_rocker);
	rocker->setVisible(true);

	Sprite *rockerBG = (Sprite *)this->getChildByTag(tag_rockerBG);
	rockerBG->setVisible(true);

	auto listener = EventListenerTouchOneByOne::create();



	listener->setSwallowTouches(true);
	

	listener->onTouchBegan = CC_CALLBACK_2(HRocker::touchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(HRocker::touchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(HRocker::touchEnded,this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener,-255);
	
	this->scheduleUpdate();
}

//ֹͣҡ��(����ҡ�ˣ�ȡ��ҡ�˵Ĵ�������)
void HRocker::stopRocker()
{
	Sprite *rocker = (Sprite *)this->getChildByTag(tag_rocker);
	rocker->setVisible(false);

	Sprite * rockerBG = (Sprite *)this->getChildByTag(tag_rockerBG);
	rockerBG->setVisible(false);

}


//��ȡ��ǰҡ�����û�������ĽǶ�
float HRocker::getRad(Point pos1,Point pos2)
{
	float px1 = pos1.x;
	float py1 = pos1.y;
	float px2 = pos2.x;
	float py2 = pos2.y;

	//�õ�����x�ľ���
	float x = px2 - px1;
	//�õ�����y�ľ���
	float y = py1 - py2;
	//���б�߳���
	float xie = sqrt(pow(x,2) + pow(y,2));
	//�õ�����Ƕȵ�����ֵ(ͨ�����Ǻ����еĵ���Ƕ�����ֵ=б��/б��)
	float cosAngle = x / xie;
	//ͨ�������Ҷ����ȡ���ڽǶȵĻ���
	float rad = acos(cosAngle);
	//ע�⣺��������λ��Y����<ҡ�˵�Y���꣬����Ҫȥ��ֵ-0~-180
	if (py2 < py1)
	{
		rad = -rad;
	}
	return rad;
}

Point getAngelePosition(float r,float angle){
	return ccp(r*cos(angle),r*sin(angle));
}

//�����¼�
bool HRocker::touchBegan(Touch *pTouch, Event *pEvent)
{
	auto target = static_cast<Layer*>(pEvent->getCurrentTarget());//��ȡ�ĵ�ǰ������Ŀ��  
	if (target != this)
	{
		//return false;
	}

	Point point = pTouch->getLocation();
	Sprite *rocker = (Sprite *)this->getChildByTag(tag_rocker);
	if (rocker->boundingBox().containsPoint(point))
	{
		isCanMove = true;
		CCLOG("begin");
	}
	return true;
}
//�ƶ��¼�
void HRocker::touchMoved(Touch *pTouch, Event *pEvent)
{
	auto target = static_cast<Layer*>(pEvent->getCurrentTarget());//��ȡ�ĵ�ǰ������Ŀ��  
	if (target != this)
	{
		//return ;
	}

	if (!isCanMove)
	{
		return;
	}
	Point point = pTouch->getLocation();
	Sprite *rocker = (Sprite *)this->getChildByTag(tag_rocker);
	//�õ�ҡ���봥�������γɵĽǶ�
	float angle = getRad(rockerBGPosition,point);
	//�ж�����Բ��Բ�ľ��Ƿ����ҡ�˱����İ뾶
	if (sqrt(pow((rockerBGPosition.x - point.x),2) + pow((rockerBGPosition.y - point.y),2)) >= rockerBGR)
	{

		//��֤�ڲ�СԲ�˶��ĳ�������
		rocker->setPosition(ccpAdd(getAngelePosition(rockerBGR,angle),ccp(rockerBGPosition.x,rockerBGPosition.y)));
		//  CCLOG("touch");
	}
	else
	{
		//��û�г�������ҡ�˸����û��������ƶ�����
		rocker->setPosition(point);
		//CCLOG("touch");
	}

	//�жϷ���
	if(angle>=-PI/4&&angle<PI/4)
	{
		rocketDirection=directon_right;
		rocketRun=false;
		CCLOG("%d",rocketDirection);
	}
	else if(angle>=PI/4&&angle<3*PI/4)
	{
		rocketDirection=directon_up;
		CCLOG("%d",rocketDirection);
	}
	else if((angle>=3*PI/4&&angle<=PI)||(angle>=-PI&&angle<-3*PI/4))
	{
		rocketDirection=directon_left;
		rocketRun=true;
		CCLOG("%d",rocketDirection);
	}
	else if(angle>=-3*PI/4&&angle<-PI/4)
	{
		rocketDirection=directon_down;
		CCLOG("%d",rocketDirection);
	}
}
//�뿪�¼�
void HRocker::touchEnded(Touch *pTouch, Event *pEvent)
{
	auto target = static_cast<Layer*>(pEvent->getCurrentTarget());//��ȡ�ĵ�ǰ������Ŀ��  
	if (target != this)
	{
		//return ;
	}

	if (!isCanMove)
	{
		return;
	}
	Sprite *rockerBG = (Sprite*)this->getChildByTag(tag_rockerBG);
	Sprite *rocker = (Sprite*)this->getChildByTag(tag_rocker);
	rocker->stopAllActions();
	rocker->runAction(CCMoveTo::create(0.08f,rockerBG->getPosition()));
	isCanMove = false;
	rocketDirection=directon_stay;

	// add by mobinsheng begin
	GameLogic::GetInstance()->HeroAction(STATIC_ACTION,-1);
	// add by mobinsheng end

	CCLOG("%d",rocketDirection);
	CCLOG("end");
}

void HRocker::update(float dt)
{
	if(isCanMove)
	{
		if (!GameLogic::GetInstance()->IsGameRunning())
		{
			return;
		}
		HeroManager::GetInstance()->HeroMove(HeroManager::GetInstance()->GetCurrentHero()->GetHeroID(),(tagDirecton)rocketDirection);
	}
}
