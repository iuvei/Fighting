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

// 暂停的状态
enum SUSPEND_STATE
{
	NOT_SUSPEND,											// 游戏未暂停
	OUR_SUSPEND,											// 游戏在运行，但是被玩家暂停了
	OPPONENT_SUSPEND,								// 游戏在运行，但是被对方暂停了
};

/************************************************************************
*   游戏逻辑                                                                   
************************************************************************/


class GameLogic
{
public:
	virtual						~GameLogic();

	typedef vector<Hero*>::iterator		HeroVectorIter;

	typedef vector<Magic*>::iterator		MagicVectorIter;

	static GameLogic* GetInstance();

	void							Clean();																		// 清理

	void							StartGame(MainScene* s);										// 开始游戏

	void							LoopGame();																// 游戏循环（用于游戏中的碰撞检测），由主场景的计时器周期性调用

	void							SuspendGame();														// 暂停游戏（未实现）

	void							ContinueGame();														// 暂停之后继续游戏

	void							StopGame();																// 离开游戏

	void							OpponentStopGame();												// 对手离开

	void							ExitApp();																	// 程序退出		

	void							HeroAction(int action_type,int direction);					// 英雄动作

	bool							ConnectToServer(const char* ip,int nPort);				// 连接到服务器

	NetworkMoudle*		GetNetworkModule();												// 获取网络模块

	MainScene*				GetMainScene();														// 获取主场景

	void							IsGameRunning(bool v);											// 设置游戏是否在运行

	bool							IsGameRunning();														// 判断游戏是否在运行

	void							SetSuspendState(int state);										// 假如暂停游戏的话，设置暂停状态
		
	int							GetSuspendState();													// 假如暂停游戏的话，获取暂停状态

	void							JumpToScene(SCENE_ID id);										// 场景之间跳转

	Player*						GetPlayer();																// 获取玩家

	int							GetGameType();														// 获取游戏类型

	void							SetGameType(int  type);											// 设置游戏类型
protected:

	GameLogic();

	void							LoadConfig();									// 加载配置文件

private:
	MainScene*				m_pMainScene;								// 主场景	

	static GameLogic*	m_pGameLogicInstance;					// 游戏逻辑实例

	bool							m_bInit;											// 是否初始化完成

	NetworkMoudle*		m_pNetwork;									// 网络模块

	bool							m_bIsGameRunning;							// 游戏运行标志		

	int							m_nSuspendState;							// 暂停的状态

	Player*						m_Player;											// 玩家信息类

	int							m_nGameType;									// 游戏类型
};

#endif //_GAME_LOGIC_H_