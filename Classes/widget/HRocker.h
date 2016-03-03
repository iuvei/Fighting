#ifndef __HROCKER_H__
#define __HROCKER_H__

#include "cocos2d.h"

#include "GameDefine.h"

USING_NS_CC;

//用于标识摇杆与摇杆的背景
typedef enum
{
	tag_rocker,
	tag_rockerBG,
}tagForHRocker;

class HRocker:public cocos2d::Layer
{
public:
	HRocker(void);

	~HRocker(void);

	virtual bool init();

	//创建摇杆(摇杆的操作题图片资源名，摇杆背景图片资源名，起始坐标)
	static HRocker* createHRocker(const char *rockerImageName,const char *rockerBGImageName,Point position);

	//启动摇杆(显示摇杆、监听摇杆触屏事件)
	void startRocker(bool _isStopOther);

	//停止摇杆(隐藏摇杆，取消摇杆的触屏监听)
	void stopRocker();

	//判断控制杆方向，用来判断精灵上、下、左、右运动
	int rocketDirection;

	//当前人物行走方向,用来判断精灵的朝向，精灵脸朝右还是朝左
	bool rocketRun;

	CREATE_FUNC(HRocker);

	void update(float dt);
private:
	//自定义初始化函数
	void rockerInit(const char* rockerImageName,const char* rockerBGImageName,Point position);
	//是否可操作摇杆
	bool isCanMove;
	//获取当前摇杆与用户触屏点的角度
	float getRad(Point pos1,Point pos2);
	//摇杆背景的坐标
	Point rockerBGPosition;
	//摇杆背景的半径
	float rockerBGR;
	//触屏事件
	virtual bool touchBegan(Touch *pTouch, Event *pEvent);

	virtual void touchMoved(Touch *pTouch, Event *pEvent);

	virtual void touchEnded(Touch *pTouch, Event *pEvent);


#ifdef _USE_PC_
	//void					onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	//void					onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
#endif
};

#endif