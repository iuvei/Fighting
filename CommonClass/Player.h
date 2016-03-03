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

// �����
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

	// ���Է��ʷ��� begin
	PROPERTY_INIT(string,UserID,m_strUserID);
	PROPERTY_INIT(string,Password,m_strPassword);
	PROPERTY_INIT(string,UserName,m_strUserName);
	PROPERTY_INIT(int,HeroID,m_nHeroID);
	PROPERTY_INIT(int,UserState,m_nUserState);
	PROPERTY_INIT(string,IP,m_strIP);
	PROPERTY_INIT(int,Port,m_nPort);
	PROPERTY_INIT(bool,IsCreator,m_bIsCreator);
	PROPERTY_INIT(bool,IsLogin,m_bLogin);
	// ���Է��ʷ��� end


	// ����ģʽ�µ����Է��ʺ��� begin
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
	// ����ģʽ�µ����Եķ��ʺ��� end


private:
	string				m_strUserID;				// Ӣ��ID
	string				m_strPassword;			// ����
	string				m_strUserName;		// ����
	int					m_nHeroID;				// ѡ���Ӣ�۵�ID���������ID������Ӣ�ۣ�
	int					m_nUserState;			// ��ҵ�״̬
	bool					m_bIsCreator;			// �������
	bool					m_bLogin;					// �Ƿ��Ѿ���½

	// ����ģʽ����Ҫ������
	SOCKET			m_socketFD;				// �׽���
	sockaddr_in		m_Addr;					// ��ַ
	string				m_strIP;					// ip
	int					m_nPort;					// �˿�
};

#endif // !_PLAYER_H_
