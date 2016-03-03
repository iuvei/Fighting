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

	int									AllocGameIndex();															// 产生一个游戏索引
	void									DeallocGameIndex(int nGameIndex);							// 施放一个游戏索引

	void									PlayerArrive(SOCKET fd,sockaddr_in& addr);								// 一个玩家接入服务器的时候调用
	void									PlayerLeave(Player* player,ControlPackage* pContolPack);		// 一个玩家断开连接的时候调用

    void									Analyze(SOCKET fd,ControlPackage* pContolPack);					// 分析请求包的请求类型

	void									Login(Player* player,ControlPackage* pContolPack);					// 登陆
	void									Logout(Player* player,ControlPackage* pContolPack);				// 注销


	void									RequestGameInfo(Player* player,ControlPackage* pContolPack);		// 请求游戏信息
	

	void									CreateGameRound(Player* player,ControlPackage* pContolPack);		// 创建一局游戏
	void									JoinGameRound(Player* player,ControlPackage* pContolPack);			// 加入一局游戏
	void									SuspendGameRound(Player* player,ControlPackage* pContolPack); //暂停游戏
	void									ContinueGameRound(Player* player,ControlPackage* pContolPack); //继续游戏
	void									LeaveGameRound(Player* player,ControlPackage* pContolPack);		// 结束游戏

	GameRound*					GetGameRound(int index);																// 根据索引获取一局游戏

	Player*								GetPlayer(SOCKET s);																		// 根据套接字获取一个玩家

	void									TransmitDataPackBuffer(Player* sender,PackageHead* head,char* buffer );		// 转发数据包

	void									Register(Player* player,ControlPackage* pContolPack);						// 注册

	bool									IsLogin(Player* player);																		// 玩家是否已经登陆

protected:
	bool									IsUserIDValid(const string& strID);													// 判断玩家ID是否有效

	bool									IsUserValid(const string& strID,const string& strPsw);						// 判断用户是否有效

	void									DumpGameInfo();																				// 打印游戏信息

	void									Respond(Player* player,ControlPackage* pContolPack);					// 回应客户
	
	string								GetAllGameInfo();																				// 获取所有游戏信息
private:
    FightingServer*											m_pFightingServer;													// 服务器

	typedef vector<GameRound*>::iterator	VecGameRoundIter;

	vector<GameRound*>								m_GameRoundList;													// 游戏列表

	int															m_nGameRoundCount;												// 游戏数量

	typedef vector<Player*>::iterator				VecPlayerIter;
	vector<Player*>										m_PlayerList;																// 玩家列表

	bool															m_IndexArray[GAME_MAX_COUNT];		
	mutex														m_IndexArrayLock;
};

#endif // GAMELOGIC_H
