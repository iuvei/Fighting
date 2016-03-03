#ifndef __HROCKER_H__
#define __HROCKER_H__

#include "cocos2d.h"

#include "GameDefine.h"

USING_NS_CC;

//���ڱ�ʶҡ����ҡ�˵ı���
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

	//����ҡ��(ҡ�˵Ĳ�����ͼƬ��Դ����ҡ�˱���ͼƬ��Դ������ʼ����)
	static HRocker* createHRocker(const char *rockerImageName,const char *rockerBGImageName,Point position);

	//����ҡ��(��ʾҡ�ˡ�����ҡ�˴����¼�)
	void startRocker(bool _isStopOther);

	//ֹͣҡ��(����ҡ�ˣ�ȡ��ҡ�˵Ĵ�������)
	void stopRocker();

	//�жϿ��Ƹ˷��������жϾ����ϡ��¡������˶�
	int rocketDirection;

	//��ǰ�������߷���,�����жϾ���ĳ��򣬾��������һ��ǳ���
	bool rocketRun;

	CREATE_FUNC(HRocker);

	void update(float dt);
private:
	//�Զ����ʼ������
	void rockerInit(const char* rockerImageName,const char* rockerBGImageName,Point position);
	//�Ƿ�ɲ���ҡ��
	bool isCanMove;
	//��ȡ��ǰҡ�����û�������ĽǶ�
	float getRad(Point pos1,Point pos2);
	//ҡ�˱���������
	Point rockerBGPosition;
	//ҡ�˱����İ뾶
	float rockerBGR;
	//�����¼�
	virtual bool touchBegan(Touch *pTouch, Event *pEvent);

	virtual void touchMoved(Touch *pTouch, Event *pEvent);

	virtual void touchEnded(Touch *pTouch, Event *pEvent);


#ifdef _USE_PC_
	//void					onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	//void					onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
#endif
};

#endif