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
*   英雄管理器 ：管理英雄的行为动作等等                                                            
************************************************************************/
class HeroManager
{
public:
	~HeroManager();

	static HeroManager* GetInstance();

	static HERO_ID			GetHeroIDByName(const char* pHeroName);			// 通过英雄的名字获取英雄的ID

	static const char*		GetHeroNameByID(HERO_ID id);								// 通过英雄的ID获取英雄的名字

	static int					GetHeroDefineCount();												// 获取英雄的总数（游戏所有英雄的数量）

	static Hero*				Create(HERO_ID id);													// 创建英雄（工厂函数）

	void							SendHeroStateToServer(Hero* pHero);						// 把英雄的状态发送到服务器

	void							UpdateHero();															// 实时更新英雄

	bool							InitHero();																	// 初始化根据游戏的选择初始化英雄

	Hero*						GetHero(HERO_ID h);												// 根据英雄id获取英雄

	void							SetCurrentHero(HERO_ID h);									// 选择英雄
	void							SetCurrentEnemy(HERO_ID h);									// 选择敌人

	Hero*						GetCurrentHero();														// 获取当前的玩家英雄
	Hero*						GetCurrentEnemy();													// 获取敌人

	void							HeroMove(HERO_ID h,tagDirecton directon);				// 控制英雄移动

	void							HeroMagicAttack(HERO_ID sender,HERO_ID revcver,tagDirecton d);				// 放魔法

	void							HeroAttack(HERO_ID sender,HERO_ID recver,tagDirecton d);						// 物理攻击

	vector<Hero*>&		GetHeroList();															// 获取游戏运行中英雄的列表（真实存在的英雄）

	void							Clean();
private:

	HeroManager();

	static HeroManager*m_pInstance;

	vector<Hero*>			m_HeroVector;									// 存储英雄的容器

	Hero*						m_pCurrentHero;								//	当前的英雄
	Hero*						m_pCurrentEnemy;							//	当前的敌人（英雄）

	HERO_ID					m_CurrentHeroID;							// 玩家当前选中的英雄
	HERO_ID					m_CurrentEnemyID;							// 当前的敌人（英雄）
	GameAI*					m_pGameAI;
};
#endif // !_HERO_MANAGER_H_
