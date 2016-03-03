#ifndef _MAGIC_MANAGER_H_
#define _MAGIC_MANAGER_H_

#include <vector>
using namespace std;

class Magic;
class Hero;

/************************************************************************
*   英魔法管理器 ：管理魔法的行为动作等等                                                            
************************************************************************/
class MagicManager
{
public:
	typedef vector<Magic*>::iterator		MagicVectorIter;
	typedef vector<Hero*>::iterator		HeroVectorIter;
	static MagicManager* GetInstance();

	void							MagicCollisionDetection();										// 碰撞检测（魔法与英雄之间）

	void							InsertMagic(Magic* mag);											// 往魔法队列中加入一个魔法

	void							Clean();

	void							UpdateMagic();															// 更新魔法列表里的魔法位置

	void							RandomMagic(int xPos,int yPos);								// 施放随机的魔法，无论谁碰到都会受伤

	void							ShowLamp();																// 点燃屏幕上方的烛火

	void							RandomMoney(int xPos,int yPos);								// 地上随机掉钱，捡到会加血

	void							TowerShoot();
private:
	MagicManager();

	static MagicManager* m_pInstance;

	vector<Magic*>		m_MagicVector;														// 魔法列表
};

#endif // !_MAGIC_MANAGER_H_
