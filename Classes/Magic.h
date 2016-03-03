#ifndef _MAGIC_H_
#define _MAGIC_H_

#include <string>
#include "cocos2d.h"
#include "GameLogic.h"
#include "GameDefine.h"
USING_NS_CC;
using namespace std;

/************************************************************************
*  魔法类
************************************************************************/
class Magic:public cocos2d::Node
{
public:
	Magic(void);
	virtual						~Magic(void);

	CREATE_FUNC(Magic);

	virtual bool				init();

	void							PlayOver();								// 魔法施放完毕

	bool							IsValid();									// 是否还有效

	void							PlayMagic(HERO_ID id,cocos2d::Sequence* seq);		// 放魔法

	void							PlayMagic(cocos2d::Sequence* seq,Point point);		// 放魔法

	void							PlayMagic(const string& strFileName,Point point);	// 放魔法

	HERO_ID					BelongTo();								// 属于哪个英雄

	void							SetMagicDistance(int d);			// 设置魔法有效距离

	int							GetMagicDistance();					// 获取魔法有效距离

	void							SetMagicDamage(int v);			// 设置有效的伤害范围

	int							GetMagicDamage();					// 获取有效的伤害范围

	tagDirecton				GetDirection();
private:
	cocos2d::Sprite*			m_pMagic;					// 魔法
	bool								m_bIsValid;					// 魔法是否还有效
	tagDirecton					m_Direction;					// 魔法的方向
	HERO_ID						m_nHeroID;					// 所属英雄的ID
	int								m_nMagicDistance;		// 魔法的有效距离
	int								m_nMagicDamage;
};

#endif

