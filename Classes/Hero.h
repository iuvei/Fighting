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
*   Ӣ���࣬����Ӣ�۵�ʱ����Ҫ����һ��Hero�����࣬��ʵ����ص��麯��                                                                   
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

	void							InitHero(HERO_ID id,tagDirecton hero_direction,tagDirecton face_to,bool bAI);				// ����Ӣ�۵�ID��Ӣ�۽��г�ʼ��
	
	void							Run(tagDirecton directon);		// �ƶ�

	void							StopAnimation();						// ֹͣ����
	

	tagDirecton				GetCurrentDirection();				// ��ȡӢ�۵�ǰ�ƶ��ķ���
	tagDirecton				GetPreDirection();						// ��ȡӢ�۵���һ���ƶ��ķ���
	tagDirecton				GetFaceDirection();					// ��ȡ���ĳ���

	void							SetFaceDirection(tagDirecton directon);

	void							SetBloodValue(float f);				// ����Ӣ�۵�Ѫ��
	float							GetBloodValue();						// ��ȡӢ�۵�Ѫ��

	void							SetMagicValue(float f);				// ����Ӣ�۵�ħ��ֵ
	float							GetMagicValue();						// ��ȡӢ�۵�ħ��ֵ

	void							UseAI(bool b);							// ����/�ر�AI
	bool							UseAI();									// �Ƿ����AI

	HERO_ID					GetHeroID();								// ��ȡӢ�۵�ID
	const char*				GetHeroName();						// ��ȡӢ�۵�����

	HERO_STATE			GetHeroState();							// ��ȡӢ�۵ĵ�ǰ״̬
	void							SetHeroState(HERO_STATE state);

	void							Die();											// Ӣ����������

	//================������Ҫʵ�ֵ��麯�� begin���Ѷ������и���m_pSequence��Ȼ����m_pSpriteִ��m_pSequence��==
	//================һ�㲻��ֱ��д���룬ʹ�ú꣺DEFINE_ANIMATION_WITH_CALLBACK��DEFINE_ANIMATION_WITHOUT_CALLBACK����
	//================�����еıȽ����⣬����д
	virtual void				SetAllAnimationFile(){}									// ��������Ӣ����Ҫ��ͼƬ��Դ

	virtual void				SetFacetoLeftStaticAnimation(){}					// ����Ӣ��������ʱ�ľ�ֹ�Ķ���
	virtual void				SetFacetoRightStaticAnimation(){}					// ����Ӣ��������ʱ�ľ�ֹ�Ķ���

	virtual void				SetFacetoLeftMoveAnimation(){}					// ����Ӣ��������ʱ���ƶ��Ķ���
	virtual void				SetFacetoRightMoveAnimation(){}					// ����Ӣ��������ʱ���ƶ��Ķ���

	virtual void				SetFacetoLeftAttackAnimation(){}					// ����Ӣ��������ʱ�Ĺ����Ķ���
	virtual void				SetFacetoRightAttackAnimation(){}				// ����Ӣ��������ʱ�Ĺ����Ķ���

	virtual void				SetFacetoLeftMagicAttackAnimation(cocos2d::Sequence*& seq){}	// ����Ӣ��������ʱ�ķ�ħ���Ķ�������������⣺�������õĶ������и���seq��
	virtual void				SetFacetoRightMagicAttackAnimation(cocos2d::Sequence*& seq){}	// ����Ӣ��������ʱ�ķ�ħ���Ķ�������������⣺�������õĶ������и���seq��

	virtual void				SetFacetoLeftParryAnimation(){}					// ����Ӣ��������ʱ�ĸ񵲵Ķ���
	virtual void				SetFacetoRightParryAnimation(){}					// ����Ӣ��������ʱ�ĸ񵲵Ķ���

	virtual void				SetFacetoLeftBeHitAnimation(){}					// ����Ӣ��������ʱ���ܵ������Ķ���
	virtual void				SetFacetoRightBeHitAnimation(){}					// ����Ӣ��������ʱ���ܵ������Ķ���

	virtual void				SetFacetoLeftGetUpAnimation(){}					// ����Ӣ��������ʱ���ܵ������󵹵�Ȼ��ӵ���վ�����Ķ���
	virtual void				SetFacetoRightGetUpAnimation(){}				// ����Ӣ��������ʱ���ܵ������󵹵�Ȼ��ӵ���վ�����Ķ���

	virtual void				SetFacetoLeftDieAnimation(){}						// ����Ӣ��������ʱ�������Ķ���
	virtual void				SetFacetoRightDieAnimation(){}					// ����Ӣ��������ʱ�������Ķ���
	//================������Ҫʵ�ֵ��麯�� end==========================

	void							Attack();													// ��������������

	void							MagicAttack();											// ħ������

	void							Parry();														// ��

	void							RunEnd();													// Ӣ���ƶ����������

	void							AttackEnd();												// �������������

	void							MagicAttackEnd();									// ħ���������������

	cocos2d::Sequence*GetSequense(){return m_pSequence;}

protected:
	void							CreateHeadButton(tagDirecton d);			// ��������ͷ��
	void							CreateBloodView(tagDirecton d);				// ����Ѫ��
	void							CreateMagicView(tagDirecton d);				// ����ħ����

protected:
	cocos2d::Sprite*			m_pSprite;											// Ӣ�۾���
	XControlButton*			m_pHeadButton;									// ͷ��ť
	ProgressView*				m_pLifebloodProgress;							// Ӣ�۵�Ѫ��
	ProgressView*				m_pMagicProgress;								// Ӣ�۵�ħ����

	cocos2d::Sequence*		m_pSequence;											// �˶�����
	
	tagDirecton					m_nPreDirection;										// Ӣ��ǰһ���˶��ķ���
	tagDirecton					m_nCurrentDirection;								// Ӣ�۵�ǰ���˶�����

	tagDirecton					m_nFaceTo;												// Ӣ�����ĳ���

	string							m_strHeroName;										// Ӣ�۵�����
	HERO_ID						m_HeroID;												// Ӣ�۵�ID


	//================��Щ�ļ����ı�����Ҫ�������LoadAllAnimationFile���������� begin==========================
	string							m_strHeroBigHeadFileName;						// ��ͷ��
	string							m_strHeroSmallHeadFileName;					// Сͷ��


	string							m_strFacetoLeftStaticFileName;				// �泯��ʱվ����ͼ���ļ�
	string							m_strFacetoRightStaticFileName;				// �泯��ʱվ����ͼ���ļ�

	string							m_strFacetoLeftMoveFileName;				// �泯��ʱ�����ƶ���ͼ���ļ�
	string							m_strFacetoRightMoveFileName;				// �泯��ʱ�����ƶ���ͼ���ļ�

	string							m_strFacetoLeftAttackFileName;				// �泯��ʱ���й�����ͼ���ļ�
	string							m_strFacetoRightAttackFileName;				// �泯��ʱ���й�����ͼ���ļ�

	string							m_strFacetoLeftMagicAttackFileName;		// �泯��ʱ��ħ����ͼ���ļ�
	string							m_strFacetoRightMagicAttackFileName;	// �泯��ʱ��ħ����ͼ���ļ�

	string							m_strFacetoLeftParryFileName;					// �泯��ʱ�񵲵�ͼ���ļ�
	string							m_strFacetoRightParryFileName;				// �泯��ʱ�񵲵�ͼ���ļ�

	string							m_strFacetoLeftBeHitFileName;				// �泯��ʱ�ܵ�������ͼ���ļ�
	string							m_strFacetoRightBeHitFileName;				// �泯��ʱ�ܵ�������ͼ���ļ�

	string							m_strFacetoLeftGetUpFileName;				// �泯��ʱ�ܹ�����վ������ͼ���ļ�
	string							m_strFacetoRightGetUpFileName;				// �泯��ʱ�ܹ�����վ������ͼ���ļ�

	string							m_strFacetoLeftDieFileName;					// �泯��ʱ������ͼ���ļ�
	string							m_strFacetoRightDieFileName;					// �泯��ʱ������ͼ���ļ�

	//================��Щ�ļ����ı�����Ҫ�������LoadAllAnimationFile���������� end==========================

	float								m_fBlood;													// Ѫ��
	bool								m_bUseAI;												// �Ƿ�����˹�����
	float								m_fMagic;												// ħ��ֵ

	HERO_STATE				m_State;													// Ӣ�۵�״̬

};

#endif  //_HERO_H_