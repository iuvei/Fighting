#ifndef _NETWORK_MODULE_
#define _NETWORK_MODULE_

#include "../CommonClass/Package.h"
#include "../CommonClass/NetAchive.h"
#include "../CommonClass/SelectSocket.h"
#include "../CommonClass/GameRound.h"

// 添加线程支持
#include <thread>
#include <mutex>
// 添加线程支持

//#define _USE_THREAD_

using namespace std;
// 网络模块

class NetworkMoudle:public SelectSocketClient
{
public:
	NetworkMoudle();

	virtual ~NetworkMoudle();

	bool						Connect(const char* strIP,int nPort)	;		// 连接服务器									

	bool						IsConnect();												// 判断是否已经连接服务器

	bool						ReadPackageHead(PackageHead& head,bool withTimeOut = false);		// 读取包头部

	bool						ReadControlPackBody(PackageHead& head,ControlPackage* pack,bool withTimeOut = false);	// 读取控制包
		
	bool						ReadDataPackBody(PackageHead& head,DataPackage* pack,bool withTimeOut = false);			// 读取数据包

	bool						ReadControlPack(ControlPackage* pack,bool withTimeOut = false);											// 读取控制包

	void						SendControlPack(ControlPackage* pack) ;																				// 发送控制包

	void						SendDataPack(DataPackage& pack);																						// 发送数据包

	bool						ReadDataPack(DataPackage& pack,bool withTimeOut = false);													// 读取数据包

	// 业务处理 begin

	// 登陆
	bool						Login(const string& id,const string& password);																		// 登陆

	// 注销
	void						Logout();																																	// 注销

	// 注册
	void						Register();																																	// 注册

	// 创建一局游戏
	bool						CreateGame(const string& strGameName,int nHeroID);															// 创建游戏

	// 加入一局游戏
	bool						JoinGame(int nGameIndex,int nHeroID) ;																					// 加入游戏

	void						SuspendGame();																														// 暂停游戏

	void						ContinueGame();																														// 继续游戏

	void						StopGame();																																// 停止游戏

	bool										RequestAllGameInfo();																								// 向服务器请求游戏信息

	vector<GameRoundInfo>& GetGameRoundList();																								// 获取游戏列表

	GameRoundInfo*					GetFirstGame();																											// 获取第一局游戏

	GameRoundInfo*					GetGameRound(int nGameIndex);																				// 获取一局游戏

	// 业务处理 end

private:


	int												m_nServerPort;								// 服务器端口

	string											m_strIP;										// 服务器IP

	PlatformInitSocket						 m_PlatformInitSocket;					// 用于不同平台的套接字初始化


	vector<GameRoundInfo>			m_GameRoundList;						//游戏信息列表

	bool												m_bIsConnect;								// 是否已经连接到服务器
};


#endif