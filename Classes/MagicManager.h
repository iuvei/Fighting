#ifndef _MAGIC_MANAGER_H_
#define _MAGIC_MANAGER_H_

#include <vector>
using namespace std;

class Magic;
class Hero;

/************************************************************************
*   Ӣħ�������� ������ħ������Ϊ�����ȵ�                                                            
************************************************************************/
class MagicManager
{
public:
	typedef vector<Magic*>::iterator		MagicVectorIter;
	typedef vector<Hero*>::iterator		HeroVectorIter;
	static MagicManager* GetInstance();

	void							MagicCollisionDetection();										// ��ײ��⣨ħ����Ӣ��֮�䣩

	void							InsertMagic(Magic* mag);											// ��ħ�������м���һ��ħ��

	void							Clean();

	void							UpdateMagic();															// ����ħ���б����ħ��λ��

	void							RandomMagic(int xPos,int yPos);								// ʩ�������ħ��������˭������������

	void							ShowLamp();																// ��ȼ��Ļ�Ϸ������

	void							RandomMoney(int xPos,int yPos);								// ���������Ǯ���񵽻��Ѫ

	void							TowerShoot();
private:
	MagicManager();

	static MagicManager* m_pInstance;

	vector<Magic*>		m_MagicVector;														// ħ���б�
};

#endif // !_MAGIC_MANAGER_H_
