#ifndef _MAGIC_H_
#define _MAGIC_H_

#include <string>
#include "cocos2d.h"
#include "GameLogic.h"
#include "GameDefine.h"
USING_NS_CC;
using namespace std;

/************************************************************************
*  ħ����
************************************************************************/
class Magic:public cocos2d::Node
{
public:
	Magic(void);
	virtual						~Magic(void);

	CREATE_FUNC(Magic);

	virtual bool				init();

	void							PlayOver();								// ħ��ʩ�����

	bool							IsValid();									// �Ƿ���Ч

	void							PlayMagic(HERO_ID id,cocos2d::Sequence* seq);		// ��ħ��

	void							PlayMagic(cocos2d::Sequence* seq,Point point);		// ��ħ��

	void							PlayMagic(const string& strFileName,Point point);	// ��ħ��

	HERO_ID					BelongTo();								// �����ĸ�Ӣ��

	void							SetMagicDistance(int d);			// ����ħ����Ч����

	int							GetMagicDistance();					// ��ȡħ����Ч����

	void							SetMagicDamage(int v);			// ������Ч���˺���Χ

	int							GetMagicDamage();					// ��ȡ��Ч���˺���Χ

	tagDirecton				GetDirection();
private:
	cocos2d::Sprite*			m_pMagic;					// ħ��
	bool								m_bIsValid;					// ħ���Ƿ���Ч
	tagDirecton					m_Direction;					// ħ���ķ���
	HERO_ID						m_nHeroID;					// ����Ӣ�۵�ID
	int								m_nMagicDistance;		// ħ������Ч����
	int								m_nMagicDamage;
};

#endif

