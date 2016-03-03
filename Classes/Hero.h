#ifndef _HERO_H_
#define _HERO_H_


#include <string>
#include "cocos2d.h"
#include "GameLogic.h"
#include "GameDefine.h"

using namespace std;
USING_NS_CC;
class XControlButton;
class ProgressView;
class GameAI;
class MainScene;

/************************************************************************
*   英雄类，增加英雄的时候需要定义一个Hero的子类，并实现相关的虚函数                                                                   
************************************************************************/
class Hero:public cocos2d::Node
{
public:
	friend  class GameAI;
	friend class Magic;
	friend class HeroManager;

	Hero(void);
	virtual						~Hero(void);

	CREATE_FUNC(Hero);

	void							InitHero(HERO_ID id,tagDirecton hero_direction,tagDirecton face_to,bool bAI);				// 根据英雄的ID对英雄进行初始化
	
	void							Run(tagDirecton directon);		// 移动

	void							StopAnimation();						// 停止动画
	

	tagDirecton				GetCurrentDirection();				// 获取英雄当前移动的方向
	tagDirecton				GetPreDirection();						// 获取英雄的上一个移动的方向
	tagDirecton				GetFaceDirection();					// 获取脸的朝向

	void							SetFaceDirection(tagDirecton directon);

	void							SetBloodValue(float f);				// 设置英雄的血量
	float							GetBloodValue();						// 获取英雄的血量

	void							SetMagicValue(float f);				// 设置英雄的魔法值
	float							GetMagicValue();						// 获取英雄的魔法值

	void							UseAI(bool b);							// 开启/关闭AI
	bool							UseAI();									// 是否具有AI

	HERO_ID					GetHeroID();								// 获取英雄的ID
	const char*				GetHeroName();						// 获取英雄的名字

	HERO_STATE			GetHeroState();							// 获取英雄的当前状态
	void							SetHeroState(HERO_STATE state);

	void							Die();											// 英雄死亡处理

	//================子类需要实现的虚函数 begin（把动作序列赋给m_pSequence，然后用m_pSprite执行m_pSequence）==
	//================一般不用直接写代码，使用宏：DEFINE_ANIMATION_WITH_CALLBACK和DEFINE_ANIMATION_WITHOUT_CALLBACK即可
	//================除非有的比较特殊，不好写
	virtual void				SetAllAnimationFile(){}									// 加载所有英雄需要的图片资源

	virtual void				SetFacetoLeftStaticAnimation(){}					// 设置英雄脸朝左时的静止的动画
	virtual void				SetFacetoRightStaticAnimation(){}					// 设置英雄脸朝右时的静止的动画

	virtual void				SetFacetoLeftMoveAnimation(){}					// 设置英雄脸朝左时的移动的动画
	virtual void				SetFacetoRightMoveAnimation(){}					// 设置英雄脸朝右时的移动的动画

	virtual void				SetFacetoLeftAttackAnimation(){}					// 设置英雄脸朝左时的攻击的动画
	virtual void				SetFacetoRightAttackAnimation(){}				// 设置英雄脸朝右时的攻击的动画

	virtual void				SetFacetoLeftMagicAttackAnimation(cocos2d::Sequence*& seq){}	// 设置英雄脸朝左时的放魔法的动画（这个是例外：将创建好的动作序列赋给seq）
	virtual void				SetFacetoRightMagicAttackAnimation(cocos2d::Sequence*& seq){}	// 设置英雄脸朝右时的放魔法的动画（这个是例外：将创建好的动作序列赋给seq）

	virtual void				SetFacetoLeftParryAnimation(){}					// 设置英雄脸朝左时的格挡的动画
	virtual void				SetFacetoRightParryAnimation(){}					// 设置英雄脸朝右时的格挡的动画

	virtual void				SetFacetoLeftBeHitAnimation(){}					// 设置英雄脸朝左时的受到攻击的动画
	virtual void				SetFacetoRightBeHitAnimation(){}					// 设置英雄脸朝右时的受到攻击的动画

	virtual void				SetFacetoLeftGetUpAnimation(){}					// 设置英雄脸朝左时的受到攻击后倒地然后从地上站起来的动画
	virtual void				SetFacetoRightGetUpAnimation(){}				// 设置英雄脸朝右时的受到攻击后倒地然后从地上站起来的动画

	virtual void				SetFacetoLeftDieAnimation(){}						// 设置英雄脸朝左时的死亡的动画
	virtual void				SetFacetoRightDieAnimation(){}					// 设置英雄脸朝右时的死亡的动画
	//================子类需要实现的虚函数 end==========================

	void							Attack();													// 攻击（物理攻击）

	void							MagicAttack();											// 魔法攻击

	void							Parry();														// 格挡

	void							RunEnd();													// 英雄移动结束后调用

	void							AttackEnd();												// 攻击结束后调用

	void							MagicAttackEnd();									// 魔法攻击结束后调用

	cocos2d::Sequence*GetSequense(){return m_pSequence;}

protected:
	void							CreateHeadButton(tagDirecton d);			// 创建人物头像
	void							CreateBloodView(tagDirecton d);				// 创建血条
	void							CreateMagicView(tagDirecton d);				// 创建魔法条

protected:
	cocos2d::Sprite*			m_pSprite;											// 英雄精灵
	XControlButton*			m_pHeadButton;									// 头像按钮
	ProgressView*				m_pLifebloodProgress;							// 英雄的血条
	ProgressView*				m_pMagicProgress;								// 英雄的魔法条

	cocos2d::Sequence*		m_pSequence;											// 运动序列
	
	tagDirecton					m_nPreDirection;										// 英雄前一个运动的方向
	tagDirecton					m_nCurrentDirection;								// 英雄当前的运动方向

	tagDirecton					m_nFaceTo;												// 英雄脸的朝向

	string							m_strHeroName;										// 英雄的名字
	HERO_ID						m_HeroID;												// 英雄的ID


	//================这些文件名的变量需要在子类的LoadAllAnimationFile函数中设置 begin==========================
	string							m_strHeroBigHeadFileName;						// 大头像
	string							m_strHeroSmallHeadFileName;					// 小头像


	string							m_strFacetoLeftStaticFileName;				// 面朝左时站立的图像文件
	string							m_strFacetoRightStaticFileName;				// 面朝右时站立的图像文件

	string							m_strFacetoLeftMoveFileName;				// 面朝左时进行移动的图像文件
	string							m_strFacetoRightMoveFileName;				// 面朝右时进行移动的图像文件

	string							m_strFacetoLeftAttackFileName;				// 面朝左时进行攻击的图像文件
	string							m_strFacetoRightAttackFileName;				// 面朝右时进行攻击的图像文件

	string							m_strFacetoLeftMagicAttackFileName;		// 面朝左时放魔法的图像文件
	string							m_strFacetoRightMagicAttackFileName;	// 面朝右时放魔法的图像文件

	string							m_strFacetoLeftParryFileName;					// 面朝左时格挡的图像文件
	string							m_strFacetoRightParryFileName;				// 面朝右时格挡的图像文件

	string							m_strFacetoLeftBeHitFileName;				// 面朝左时受到攻击的图像文件
	string							m_strFacetoRightBeHitFileName;				// 面朝右时受到攻击的图像文件

	string							m_strFacetoLeftGetUpFileName;				// 面朝左时受攻击后站起来的图像文件
	string							m_strFacetoRightGetUpFileName;				// 面朝右时受攻击后站起来的图像文件

	string							m_strFacetoLeftDieFileName;					// 面朝左时死亡的图像文件
	string							m_strFacetoRightDieFileName;					// 面朝右时死亡的图像文件

	//================这些文件名的变量需要在子类的LoadAllAnimationFile函数中设置 end==========================

	float								m_fBlood;													// 血量
	bool								m_bUseAI;												// 是否具有人工智能
	float								m_fMagic;												// 魔法值

	HERO_STATE				m_State;													// 英雄的状态

};

#endif  //_HERO_H_