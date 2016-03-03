#ifndef _JAN_H_
#define _JAN_H_

#include "Hero.h"

class Jan:public Hero
{
public:
	CREATE_FUNC(Jan);
	//================子类需要实现下面的虚函数==========================
	virtual void				SetAllAnimationFile();
	virtual void				SetFacetoLeftStaticAnimation();
	virtual void				SetFacetoRightStaticAnimation();

	virtual void				SetFacetoLeftMoveAnimation();
	virtual void				SetFacetoRightMoveAnimation();

	virtual void				SetFacetoLeftAttackAnimation();
	virtual void				SetFacetoRightAttackAnimation();

	virtual void				SetFacetoLeftMagicAttackAnimation(cocos2d::Sequence*& seq);
	virtual void				SetFacetoRightMagicAttackAnimation(cocos2d::Sequence*& seq);

	virtual void				SetFacetoLeftParryAnimation();
	virtual void				SetFacetoRightParryAnimation();

	virtual void				SetFacetoLeftInjuredAnimation();
	virtual void				SetFacetoRightInjuredAnimation();

	virtual void				SetFacetoLeftBeHitAnimation();
	virtual void				SetFacetoRightBeHitAnimation();

	virtual void				SetFacetoLeftGetUpAnimation();
	virtual void				SetFacetoRightGetUpAnimation();

	virtual void				SetFacetoLeftDieAnimation();
	virtual void				SetFacetoRightDieAnimation();
	//================子类需要实现下面的虚函数==========================
};
#endif // !_BAT_H_
