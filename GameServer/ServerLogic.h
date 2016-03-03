#ifndef _SERVERLOGIC_H
#define _SERVERLOGIC_H

#if (defined(WIN32) || defined(WIN64))
#include "../CommonClass/common.h"
#include "../CommonClass/SelectSocket.h"
#else
#include "../CommonClass/common.h"
#include "../CommonClass/SelectSocket.h"
#endif // WIN32


#include <mutex>

class FightingServer;
class GameRound;
class Player;
class PackageHead;
class DataPackage;
class ControlPackage;

#ifndef GAME_MAX_COUNT
#define GAME_MAX_COUNT (64)
#endif // !GAME_MAX_COUNT

struct IndexInfo
{
	int index;
	bool used;
};

class ServerLogic
{
public:
    ServerLogic(FightingServer* pServer);

	~ServerLogic();

	int									AllocGameIndex();															// ����һ����Ϸ����
	void									DeallocGameIndex(int nGameIndex);							// ʩ��һ����Ϸ����

	void									PlayerArrive(SOCKET fd,sockaddr_in& addr);								// һ����ҽ����������ʱ�����
	void									PlayerLeave(Player* player,ControlPackage* pContolPack);		// һ����ҶϿ����ӵ�ʱ�����

    void									Analyze(SOCKET fd,ControlPackage* pContolPack);					// �������������������

	void									Login(Player* player,ControlPackage* pContolPack);					// ��½
	void									Logout(Player* player,ControlPackage* pContolPack);				// ע��


	void									RequestGameInfo(Player* player,ControlPackage* pContolPack);		// ������Ϸ��Ϣ
	

	void									CreateGameRound(Player* player,ControlPackage* pContolPack);		// ����һ����Ϸ
	void									JoinGameRound(Player* player,ControlPackage* pContolPack);			// ����һ����Ϸ
	void									SuspendGameRound(Player* player,ControlPackage* pContolPack); //��ͣ��Ϸ
	void									ContinueGameRound(Player* player,ControlPackage* pContolPack); //������Ϸ
	void									LeaveGameRound(Player* player,ControlPackage* pContolPack);		// ������Ϸ

	GameRound*					GetGameRound(int index);																// ����������ȡһ����Ϸ

	Player*								GetPlayer(SOCKET s);																		// �����׽��ֻ�ȡһ�����

	void									TransmitDataPackBuffer(Player* sender,PackageHead* head,char* buffer );		// ת�����ݰ�

	void									Register(Player* player,ControlPackage* pContolPack);						// ע��

	bool									IsLogin(Player* player);																		// ����Ƿ��Ѿ���½

protected:
	bool									IsUserIDValid(const string& strID);													// �ж����ID�Ƿ���Ч

	bool									IsUserValid(const string& strID,const string& strPsw);						// �ж��û��Ƿ���Ч

	void									DumpGameInfo();																				// ��ӡ��Ϸ��Ϣ

	void									Respond(Player* player,ControlPackage* pContolPack);					// ��Ӧ�ͻ�
	
	string								GetAllGameInfo();																				// ��ȡ������Ϸ��Ϣ
private:
    FightingServer*											m_pFightingServer;													// ������

	typedef vector<GameRound*>::iterator	VecGameRoundIter;

	vector<GameRound*>								m_GameRoundList;													// ��Ϸ�б�

	int															m_nGameRoundCount;												// ��Ϸ����

	typedef vector<Player*>::iterator				VecPlayerIter;
	vector<Player*>										m_PlayerList;																// ����б�

	bool															m_IndexArray[GAME_MAX_COUNT];		
	mutex														m_IndexArrayLock;
};

#endif // GAMELOGIC_H
