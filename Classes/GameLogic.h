#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"


#include <string>
#include <map>
#include "GameDefine.h"
using namespace std;

class Hero;
class MainScene;
class HRocker;
class Magic;
class NetworkMoudle;
class GameRound;
class Player;

// ��ͣ��״̬
enum SUSPEND_STATE
{
	NOT_SUSPEND,											// ��Ϸδ��ͣ
	OUR_SUSPEND,											// ��Ϸ�����У����Ǳ������ͣ��
	OPPONENT_SUSPEND,								// ��Ϸ�����У����Ǳ��Է���ͣ��
};

/************************************************************************
*   ��Ϸ�߼�                                                                   
************************************************************************/


class GameLogic
{
public:
	virtual						~GameLogic();

	typedef vector<Hero*>::iterator		HeroVectorIter;

	typedef vector<Magic*>::iterator		MagicVectorIter;

	static GameLogic* GetInstance();

	void							Clean();																		// ����

	void							StartGame(MainScene* s);										// ��ʼ��Ϸ

	void							LoopGame();																// ��Ϸѭ����������Ϸ�е���ײ��⣩�����������ļ�ʱ�������Ե���

	void							SuspendGame();														// ��ͣ��Ϸ��δʵ�֣�

	void							ContinueGame();														// ��֮ͣ�������Ϸ

	void							StopGame();																// �뿪��Ϸ

	void							OpponentStopGame();												// �����뿪

	void							ExitApp();																	// �����˳�		

	void							HeroAction(int action_type,int direction);					// Ӣ�۶���

	bool							ConnectToServer(const char* ip,int nPort);				// ���ӵ�������

	NetworkMoudle*		GetNetworkModule();												// ��ȡ����ģ��

	MainScene*				GetMainScene();														// ��ȡ������

	void							IsGameRunning(bool v);											// ������Ϸ�Ƿ�������

	bool							IsGameRunning();														// �ж���Ϸ�Ƿ�������

	void							SetSuspendState(int state);										// ������ͣ��Ϸ�Ļ���������ͣ״̬
		
	int							GetSuspendState();													// ������ͣ��Ϸ�Ļ�����ȡ��ͣ״̬

	void							JumpToScene(SCENE_ID id);										// ����֮����ת

	Player*						GetPlayer();																// ��ȡ���

	int							GetGameType();														// ��ȡ��Ϸ����

	void							SetGameType(int  type);											// ������Ϸ����
protected:

	GameLogic();

	void							LoadConfig();									// ���������ļ�

private:
	MainScene*				m_pMainScene;								// ������	

	static GameLogic*	m_pGameLogicInstance;					// ��Ϸ�߼�ʵ��

	bool							m_bInit;											// �Ƿ��ʼ�����

	NetworkMoudle*		m_pNetwork;									// ����ģ��

	bool							m_bIsGameRunning;							// ��Ϸ���б�־		

	int							m_nSuspendState;							// ��ͣ��״̬

	Player*						m_Player;											// �����Ϣ��

	int							m_nGameType;									// ��Ϸ����
};

#endif //_GAME_LOGIC_H_