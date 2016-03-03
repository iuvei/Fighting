#ifdef WIN32
#include "ServerLogic.h"
#include "../CommonClass/NetAchive.h"
#include "../CommonClass//CodeMsg.h"
#else
#include "ServerLogic.h"
#include "../CommonClass/NetAchive.h"
#include "../CommonClass/CodeMsg.h"
#endif

#include "FightingServer.h"

ServerLogic::ServerLogic(FightingServer* pServer)
{
    m_pFightingServer = pServer;
	m_nGameRoundCount = 0;

	for (int i = 0; i < GAME_MAX_COUNT; ++i)
	{
		m_IndexArray[i] = false;
	}
	
}

ServerLogic::~ServerLogic()
{
	for (int i = 0; i < m_GameRoundList.size(); ++i)
	{
		delete m_GameRoundList[i];
	}

	m_GameRoundList.clear();

	for (int i = 0; i < m_PlayerList.size(); ++i)
	{
		delete m_PlayerList[i];
	}

	m_nGameRoundCount = 0;

	m_PlayerList.clear();
	
}


// �������ư���������ȡ���ư��Ĵ��룬������Ӧ�Ĵ�����
void ServerLogic::Analyze(SOCKET fd,ControlPackage* pContolPack)
{
	Player* player = GetPlayer(fd);
	if (0 == player)
	{
		return;
	}

	switch (pContolPack->GetPackageCode())
	{
	case code_login:
		{
			Login(player,pContolPack);
		}
		break;
	case code_leave:
		{
			LeaveGameRound(player ,pContolPack);
		}
		break;
	case code_create_game:
		{
			CreateGameRound(player ,pContolPack);
		}

		break;
	case code_join_game_req_gameinfo:
		{
			RequestGameInfo(player,pContolPack);
		}
		break;
	case code_join_game:
		{
			JoinGameRound(player,pContolPack);
		}
		break;
	case code_suspend_game:
		{
			SuspendGameRound(player,pContolPack);
		}
		break;
	case code_continue_game:
		{
			ContinueGameRound(player,pContolPack);
		}
		break;
	}
}

// ����һ����Ϸ������ÿһ����Ϸ����������Ψһ�ģ�
int ServerLogic::AllocGameIndex()
{
	m_IndexArrayLock.lock();

	int nGameIndex = -1;
	for (int i = 0; i < GAME_MAX_COUNT; ++i)
	{
		if (m_IndexArray[i] == false)
		{
			m_IndexArray[i] = true;
			nGameIndex = i;
			break;
		}
	}
	
	m_IndexArrayLock.unlock();
	return nGameIndex;
}

// ʩ��һ����Ϸ������
void ServerLogic::DeallocGameIndex(int nGameIndex)
{
	if (nGameIndex <0 || nGameIndex >= GAME_MAX_COUNT)
	{
		return;
	}

	m_IndexArrayLock.lock();
	m_IndexArray[nGameIndex] = false;
	m_IndexArrayLock.unlock();
}

// �����׽��ֻ�ȡ��Ҷ���
Player*		ServerLogic::GetPlayer(SOCKET s)
{
	for (int i = 0; i < m_PlayerList.size();++i)
	{
		if(m_PlayerList[i]->GetSocket() == s)
		{
			return m_PlayerList[i];
		}
	}
	return 0;
}

// ��ҽ���
void ServerLogic::PlayerArrive(SOCKET fd,sockaddr_in& addr)
{
	Player* pPlay = new Player(fd,addr);
	m_PlayerList.push_back(pPlay);
}

// ����뿪
void	ServerLogic::PlayerLeave(Player* player,ControlPackage* pContolPack)
{
	LeaveGameRound(player,pContolPack);

	for (VecPlayerIter it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
	{
		if ((*it) == player)
		{
			delete player;
			player = 0;
			m_PlayerList.erase(it);
			break;
		}
	}
}

// ��ҵ�½
void  ServerLogic::Login(Player* player,ControlPackage* pContolPack)
{
	if (IsLogin(player))
	{
		return;
	}
}

// ���ע��
void ServerLogic::Logout(Player* player,ControlPackage* pContolPack)
{

}

// ����һ����Ϸ
void ServerLogic::CreateGameRound(Player* player ,ControlPackage* pContolPack)
{
	if(m_GameRoundList.size() >= 64)
	{
		pContolPack->SetPackageCode(code_create_game_ret_failed);
		Respond(player,pContolPack);
		return;
	}
	int nGameRound = AllocGameIndex();

	player->SetIsCreator(true);
	player->SetHeroID(pContolPack->GetUserHeroID());

	GameRound* pRound = new GameRound;
	pRound->SetGameState(game_state_wait);
	pRound->SetCreatePlayer(player);
	pRound->SetGameIndex( nGameRound);
	m_GameRoundList.push_back(pRound);

	pContolPack->SetPackageCode(code_create_game_ret_ok);
	//m_pGameServer->SendControlPack(player->GetSocket(),pContolPack);
	Respond(player,pContolPack);

	m_nGameRoundCount++;
}

// ����������д��ڵȴ�״̬����Ϸ��Ϣ
void ServerLogic::RequestGameInfo(Player* player,ControlPackage* pContolPack)
{
	DumpGameInfo();
	string str = GetAllGameInfo();
	pContolPack->SetOtherInfo(str);
	pContolPack->SetPackageCode(code_join_game_rsp_gameinfo);
	pContolPack->SetGameRoundWaitCount(m_GameRoundList.size());
	//m_pGameServer->SendControlPack(player->GetSocket(),pContolPack);
	Respond(player,pContolPack);
}

// ������Ϸ������ȡ��Ϸ
GameRound* ServerLogic::GetGameRound(int index)
{
	for (int i = 0;i < m_GameRoundList.size();++i)
	{
		if (m_GameRoundList[i]->GetGameIndex() == index)
		{
			return m_GameRoundList[i];
		}	
	}
	return 0;
}

// ����һ���Ѿ���������Ϸ
void ServerLogic::JoinGameRound(Player* player ,ControlPackage* pContolPack)
{
	GameRound* pRound = GetGameRound(pContolPack->GetGameRoundIndex());
	if (pRound == 0 || pRound->GetGameState() != game_state_wait)
	{
		pContolPack->SetPackageCode(code_join_game_ret_failed);
		//m_pGameServer->SendControlPack(player->GetSocket(),pContolPack);
		Respond(player,pContolPack);
		return;
	}
	
	if (pRound->GetCreatePlayer()->GetHeroID() == pContolPack->GetUserHeroID())
	{
	}
	
	player->SetHeroID(pContolPack->GetUserHeroID());

	pRound->SetJoinPlayer( player);

	pRound->SetGameState(game_state_runing);

	ControlPackage packPrimary,packSecond;
	packSecond.SetPackageCode(code_join_game_ret_ok);
	packSecond.SetUserHeroID(pRound->GetCreatePlayer()->GetHeroID());
	Respond(player,&packSecond);

	packPrimary.SetPackageCode(code_join_game);
	packPrimary.SetUserHeroID(player->GetHeroID());
	Respond(pRound->GetCreatePlayer(),&packPrimary);
}

// ��ͣ��Ϸ
void ServerLogic::SuspendGameRound(Player* player,ControlPackage* pContolPack)
{
	ControlPackage pack;
	pack.SetPackageCode(code_suspend_game);

	for (VecGameRoundIter it = m_GameRoundList.begin(); it != m_GameRoundList.end(); ++it)
	{
		GameRound* pRound = 0;
		pRound = (*it);
		if (pRound->GetCreatePlayer() == player)
		{
			if(pRound->GetJoinPlayer() != 0)
			{
				Respond(pRound->GetJoinPlayer(),&pack);
			}
			break;
		}
		else if(pRound->GetJoinPlayer() == player)
		{
			if(pRound->GetCreatePlayer() != 0)
			{
				Respond(pRound->GetCreatePlayer(),&pack);
			}
			break;
		}
	}
}

// ��ͣ��Ϸ
void ServerLogic::ContinueGameRound(Player* player,ControlPackage* pContolPack)
{
	ControlPackage pack;
	pack.SetPackageCode(code_continue_game);

	for (VecGameRoundIter it = m_GameRoundList.begin(); it != m_GameRoundList.end(); ++it)
	{
		GameRound* pRound = 0;
		pRound = (*it);
		if (pRound->GetCreatePlayer() == player)
		{
			if(pRound->GetJoinPlayer() != 0)
			{
				Respond(pRound->GetJoinPlayer(),&pack);
			}
			break;
		}
		else if(pRound->GetJoinPlayer() == player)
		{
			if(pRound->GetCreatePlayer() != 0)
			{
				Respond(pRound->GetCreatePlayer(),&pack);
			}
			break;
		}
	}
}


// ����뿪��Ϸ����ʾ��Ϸ��������������ҶϿ����ӣ�
void ServerLogic::LeaveGameRound(Player* player,ControlPackage* pContolPack)
{
	ControlPackage pack;
	pack.SetPackageCode(code_leave);

	for (VecGameRoundIter it = m_GameRoundList.begin(); it != m_GameRoundList.end(); ++it)
	{
		GameRound* pRound = 0;
		pRound = (*it);
		if (pRound->GetCreatePlayer() == player)
		{
			DeallocGameIndex(pRound->GetGameIndex());
			if(pRound->GetJoinPlayer() != 0)
			{
				Respond(pRound->GetJoinPlayer(),&pack);
			}

			m_GameRoundList.erase(it);
			delete pRound;
			break;
		}
		else if(pRound->GetJoinPlayer() == player)
		{
			DeallocGameIndex(pRound->GetGameIndex());

			if(pRound->GetCreatePlayer() != 0)
			{
				Respond(pRound->GetCreatePlayer(),&pack);
			}
			m_GameRoundList.erase(it);
			delete pRound;
			break;
		}
	}
}

// ��ȡ���ڵȴ�״̬��������Ϸ��Ϣ
string ServerLogic::GetAllGameInfo()
{
	char buf[SZ_4096_LENGTH] = {0};
	NetAchive ar(buf,SZ_4096_LENGTH,NetAchive::SAVE_MODE);
	for (int i = 0;i < m_GameRoundList.size();++i)
	{
		if(m_GameRoundList[i]->GetGameState() == game_state_wait)
		{
			ar.Save_int(m_GameRoundList[i]->GetGameIndex());
			ar.Save_string(m_GameRoundList[i]->GetCreatePlayer()->GetUserID());
			ar.Save_int(m_GameRoundList[i]->GetCreatePlayer()->GetHeroID());
		}
	}
	string strRet = buf;
	return strRet;
}

// ����ӡ��Ϸ��Ϣ
void ServerLogic::DumpGameInfo()
{
	cout << "=============GameInfo=============" << endl;
	for (int i = 0;i < m_GameRoundList.size();++i)
	{
		if(m_GameRoundList[i]->GetGameState() == game_state_wait)
		{
			cout << "\tIndex = "<<m_GameRoundList[i]->GetGameIndex() <<" UserID = "<< m_GameRoundList[i]->GetCreatePlayer()->GetUserID()
				<< " HeroID = " << m_GameRoundList[i]->GetCreatePlayer()->GetHeroID()<<endl;
		}
	}
	cout << "=============GameInfo=============" << endl;
}

// ת�����ݰ�
void ServerLogic::TransmitDataPackBuffer(Player* sender ,PackageHead* head,char* buffer )
{

	for (int i = 0; i < m_GameRoundList.size(); ++i)
	{
		GameRound* pRound = m_GameRoundList[i];

		if (pRound->GetGameState() != game_state_runing)
		{
			continue;
		}

		if (sender == m_GameRoundList[i]->GetCreatePlayer())
		{
			m_pFightingServer->SendDataPackBuffer(m_GameRoundList[i]->GetJoinPlayer()->GetSocket(),*head,buffer);
		}
		else if(sender == m_GameRoundList[i]->GetJoinPlayer())
		{
			m_pFightingServer->SendDataPackBuffer(m_GameRoundList[i]->GetCreatePlayer()->GetSocket(),*head,buffer);
		}
	}
}

// ��Ӧ�ͻ�
void ServerLogic::Respond(Player* player,ControlPackage* pContolPack)
{
	m_pFightingServer->SendControlPack(player->GetSocket(),pContolPack);
}

// �ж�����Ƿ��Ѿ���½
bool ServerLogic::IsLogin(Player* player)
{
	return player->GetIsLogin();
}