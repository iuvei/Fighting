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

//创建摇杆(摇杆的操作题图片资源名，摇杆背景图片资源名，起始坐标)
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

//自定义初始化函数
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
	rocketDirection=-1;//表示摇杆方向不变
}

//启动摇杆(显示摇杆、监听摇杆触屏事件)
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

//停止摇杆(隐藏摇杆，取消摇杆的触屏监听)
void HRocker::stopRocker()
{
	Sprite *rocker = (Sprite *)this->getChildByTag(tag_rocker);
	rocker->setVisible(false);

	Sprite * rockerBG = (Sprite *)this->getChildByTag(tag_rockerBG);
	rockerBG->setVisible(false);

}


//获取当前摇杆与用户触屏点的角度
float HRocker::getRad(Point pos1,Point pos2)
{
	float px1 = pos1.x;
	float py1 = pos1.y;
	float px2 = pos2.x;
	float py2 = pos2.y;

	//得到两点x的距离
	float x = px2 - px1;
	//得到两点y的距离
	float y = py1 - py2;
	//算出斜边长度
	float xie = sqrt(pow(x,2) + pow(y,2));
	//得到这个角度的余弦值(通过三角函数中的店里：角度余弦值=斜边/斜边)
	float cosAngle = x / xie;
	//通过反余弦定理获取到期角度的弧度
	float rad = acos(cosAngle);
	//注意：当触屏的位置Y坐标<摇杆的Y坐标，我们要去反值-0~-180
	if (py2 < py1)
	{
		rad = -rad;
	}
	return rad;
}

Point getAngelePosition(float r,float angle){
	return ccp(r*cos(angle),r*sin(angle));
}

//触摸事件
bool HRocker::touchBegan(Touch *pTouch, Event *pEvent)
{
	auto target = static_cast<Layer*>(pEvent->getCurrentTarget());//获取的当前触摸的目标  
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
//移动事件
void HRocker::touchMoved(Touch *pTouch, Event *pEvent)
{
	auto target = static_cast<Layer*>(pEvent->getCurrentTarget());//获取的当前触摸的目标  
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
	//得到摇杆与触屏点所形成的角度
	float angle = getRad(rockerBGPosition,point);
	//判断两个圆的圆心距是否大于摇杆背景的半径
	if (sqrt(pow((rockerBGPosition.x - point.x),2) + pow((rockerBGPosition.y - point.y),2)) >= rockerBGR)
	{

		//保证内部小圆运动的长度限制
		rocker->setPosition(ccpAdd(getAngelePosition(rockerBGR,angle),ccp(rockerBGPosition.x,rockerBGPosition.y)));
		//  CCLOG("touch");
	}
	else
	{
		//当没有超过，让摇杆跟随用户触屏点移动即可
		rocker->setPosition(point);
		//CCLOG("touch");
	}

	//判断方向
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
//离开事件
void HRocker::touchEnded(Touch *pTouch, Event *pEvent)
{
	auto target = static_cast<Layer*>(pEvent->getCurrentTarget());//获取的当前触摸的目标  
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
