#ifndef _PLAYER_H_
#define _PLAYER_H_

#if (defined(WIN32) || defined(WIN64))
#include "../CommonClass/common.h"
#include "../CommonClass/SelectSocket.h"
#else
#include "../CommonClass/common.h"
#include "../CommonClass/SelectSocket.h"
#endif 

#include "../CommonClass/Package.h"

// 玩家类
class Player
{
public:
	Player()
	{
		m_strUserID = -1;
		m_strPassword = "";
		m_nHeroID = -1;
		m_nUserState = -1;
		m_bIsCreator = false;
		m_bLogin = false;
	}

	Player(SOCKET s,sockaddr_in& addr)
	{
		m_socketFD = s;
		m_Addr = addr;
		m_strUserID = -1;
		m_strPassword = "";
		m_nHeroID = -1;
		m_nUserState = -1;
		m_strIP = Util::GetIPString(m_Addr);
		m_nPort = Util::GetPort(m_Addr);
		m_bIsCreator = false;
		m_bLogin = false;
		m_strUserName = "";
	}

	~Player()
	{
		CloseSocket(m_socketFD);
	}

	// 属性访问方法 begin
	PROPERTY_INIT(string,UserID,m_strUserID);
	PROPERTY_INIT(string,Password,m_strPassword);
	PROPERTY_INIT(string,UserName,m_strUserName);
	PROPERTY_INIT(int,HeroID,m_nHeroID);
	PROPERTY_INIT(int,UserState,m_nUserState);
	PROPERTY_INIT(string,IP,m_strIP);
	PROPERTY_INIT(int,Port,m_nPort);
	PROPERTY_INIT(bool,IsCreator,m_bIsCreator);
	PROPERTY_INIT(bool,IsLogin,m_bLogin);
	// 属性访问方法 end


	// 联网模式下的属性访问函数 begin
	SOCKET			GetSocket()
	{
		return m_socketFD;
	}

	void					SetSocket(SOCKET s,sockaddr_in* pAddr)
	{
		m_socketFD = s;
		m_Addr = *pAddr;
		m_strIP = Util::GetIPString(m_Addr);
		m_nPort = Util::GetPort(m_Addr);
	}
	// 联网模式下的属性的访问函数 end


private:
	string				m_strUserID;				// 英雄ID
	string				m_strPassword;			// 密码
	string				m_strUserName;		// 名字
	int					m_nHeroID;				// 选择的英雄的ID（根据这个ID来查找英雄）
	int					m_nUserState;			// 玩家的状态
	bool					m_bIsCreator;			// 玩家类型
	bool					m_bLogin;					// 是否已经登陆

	// 联网模式下需要的属性
	SOCKET			m_socketFD;				// 套接字
	sockaddr_in		m_Addr;					// 地址
	string				m_strIP;					// ip
	int					m_nPort;					// 端口
};

#endif // !_PLAYER_H_
