#ifndef _GAMEROUND_H_
#define _GAMEROUND_H_


#include "../CommonClass/common.h"
#include "../CommonClass/SelectSocket.h"
#include "../CommonClass/Player.h"

// 游戏状态
enum GameRoundState
{
	game_state_unknown = -1,
	game_state_wait,
	game_state_runing,
	game_state_suspend,
	game_state_end
};

class Player;

// 游戏,主要用于服务器端
class GameRound
{
public:
	GameRound()
	{
		m_pCreatePlayer = 0;
		m_pJoinPlayer = 0;
		m_nGameState = game_state_unknown;
		m_nGameIndex = -1;
		m_strGameName = "";
	}

	PROPERTY_INIT(Player*,CreatePlayer,m_pCreatePlayer);
	PROPERTY_INIT(Player*,JoinPlayer,m_pJoinPlayer);
	PROPERTY_INIT(int,GameState,m_nGameState);
	PROPERTY_INIT(int,GameIndex,m_nGameIndex);
	PROPERTY_INIT(string,GameName,m_strGameName);

	/*
	void								ConverToControlPackage(ControlPackage& ctrlPack,PackageCode code)
	{
		
		ctrlPack.SetPackageCode(code);

		ctrlPack.SetCreatorUserID(m_pCreatePlayer->GetUserID());
		ctrlPack.SetCreatorUserPassword(m_pCreatePlayer->GetPassword());
		ctrlPack.SetCreatorUserName(m_pCreatePlayer->GetUserName());
		ctrlPack.SetCreatorUserHeroID(m_pCreatePlayer->GetHeroID());
		ctrlPack.SetCreatorUserLogin(m_pCreatePlayer->GetIsLogin());

		ctrlPack.SetJoinUserID(m_pJoinPlayer->GetUserID());
		ctrlPack.SetJoinUserPassword(m_pJoinPlayer->GetPassword());
		ctrlPack.SetJoinUserName(m_pJoinPlayer->GetUserName());
		ctrlPack.SetJoinUserHeroID(m_pJoinPlayer->GetHeroID());
		ctrlPack.SetJoinUserLogin(m_pJoinPlayer->GetIsLogin());

		ctrlPack.SetGameName(m_strGameName);
		ctrlPack.SetGameRoundIndex(m_nGameIndex);
		ctrlPack.SetGameState(m_nGameState);
		
	}

	void								GetFromControlPackage(ControlPackage& ctrlPack)
	{
		
		m_pCreatePlayer->SetUserID(ctrlPack.GetCreatorUserID());
		m_pCreatePlayer->SetPassword(ctrlPack.GetCreatorUserPassword());
		m_pCreatePlayer->SetUserName(ctrlPack.GetCreatorUserName());
		m_pCreatePlayer->SetHeroID(ctrlPack.GetCreatorUserHeroID());
		m_pCreatePlayer->SetIsLogin(ctrlPack.GetCreatorUserLogin());

		m_pJoinPlayer->SetUserID(ctrlPack.GetJoinUserID());
		m_pJoinPlayer->SetPassword(ctrlPack.GetJoinUserPassword());
		m_pJoinPlayer->SetUserName(ctrlPack.GetJoinUserName());
		m_pJoinPlayer->SetHeroID(ctrlPack.GetJoinUserHeroID());
		m_pJoinPlayer->SetIsLogin(ctrlPack.GetJoinUserLogin());

		m_strGameName = ctrlPack.GetGameName();
		m_nGameIndex = ctrlPack.GetGameRoundIndex();
		m_nGameState = ctrlPack.GetGameState();
		
	}
	*/
private:
	Player*							m_pCreatePlayer;		// 游戏创建者
	Player*							m_pJoinPlayer;			// 游戏加入者
	int								m_nGameState;		// 游戏状态
	string							m_strGameName;		// 游戏名称
	int								m_nGameIndex;		// 游戏的索引
};

// 游戏信息类，主要用于游戏客户端
class GameRoundInfo
{
public:
	int								m_nGameState;		// 游戏状态
	string							m_strGameName;		// 游戏名称
	int								m_nGameIndex;		// 游戏的索引

	string							m_strCreateUserID;	// 创建者的ID
	int								m_nCreateHeroID;	// 创建者的英雄的ID

	string							m_strJoinUserID;		// 加入者的ID
	int								m_nJoinHeroID;		// 加入者的英雄ID
};
#endif