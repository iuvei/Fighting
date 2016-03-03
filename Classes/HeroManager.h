#ifndef _HERO_MANAGER_H_
#define _HERO_MANAGER_H_

#include "GameDefine.h"
#include <vector>
#include <string>
using namespace std;


class Hero;
class GameAI;

struct HeroInfoMap
{
	HERO_ID			id;
	std::string			name;
};

extern HeroInfoMap G_HeroInfo[];


/************************************************************************
*   Ӣ�۹����� ������Ӣ�۵���Ϊ�����ȵ�                                                            
************************************************************************/
class HeroManager
{
public:
	~HeroManager();

	static HeroManager* GetInstance();

	static HERO_ID			GetHeroIDByName(const char* pHeroName);			// ͨ��Ӣ�۵����ֻ�ȡӢ�۵�ID

	static const char*		GetHeroNameByID(HERO_ID id);								// ͨ��Ӣ�۵�ID��ȡӢ�۵�����

	static int					GetHeroDefineCount();												// ��ȡӢ�۵���������Ϸ����Ӣ�۵�������

	static Hero*				Create(HERO_ID id);													// ����Ӣ�ۣ�����������

	void							SendHeroStateToServer(Hero* pHero);						// ��Ӣ�۵�״̬���͵�������

	void							UpdateHero();															// ʵʱ����Ӣ��

	bool							InitHero();																	// ��ʼ��������Ϸ��ѡ���ʼ��Ӣ��

	Hero*						GetHero(HERO_ID h);												// ����Ӣ��id��ȡӢ��

	void							SetCurrentHero(HERO_ID h);									// ѡ��Ӣ��
	void							SetCurrentEnemy(HERO_ID h);									// ѡ�����

	Hero*						GetCurrentHero();														// ��ȡ��ǰ�����Ӣ��
	Hero*						GetCurrentEnemy();													// ��ȡ����

	void							HeroMove(HERO_ID h,tagDirecton directon);				// ����Ӣ���ƶ�

	void							HeroMagicAttack(HERO_ID sender,HERO_ID revcver,tagDirecton d);				// ��ħ��

	void							HeroAttack(HERO_ID sender,HERO_ID recver,tagDirecton d);						// ������

	vector<Hero*>&		GetHeroList();															// ��ȡ��Ϸ������Ӣ�۵��б���ʵ���ڵ�Ӣ�ۣ�

	void							Clean();
private:

	HeroManager();

	static HeroManager*m_pInstance;

	vector<Hero*>			m_HeroVector;									// �洢Ӣ�۵�����

	Hero*						m_pCurrentHero;								//	��ǰ��Ӣ��
	Hero*						m_pCurrentEnemy;							//	��ǰ�ĵ��ˣ�Ӣ�ۣ�

	HERO_ID					m_CurrentHeroID;							// ��ҵ�ǰѡ�е�Ӣ��
	HERO_ID					m_CurrentEnemyID;							// ��ǰ�ĵ��ˣ�Ӣ�ۣ�
	GameAI*					m_pGameAI;
};
#endif // !_HERO_MANAGER_H_
