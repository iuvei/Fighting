#include "NetworkModule.h"
#include "../CommonClass/Player.h"

NetworkMoudle::NetworkMoudle()
{

}

NetworkMoudle::~NetworkMoudle()
{

}

bool	NetworkMoudle::Connect(const char* strIP,int nPort)				// ���ӵ�������
{
	m_strIP = strIP;
	m_nServerPort = nPort;
	m_bIsConnect = ConnectToServer(strIP,nPort);
	return m_bIsConnect;
}

bool NetworkMoudle::IsConnect()
{
	return m_bIsConnect;
}


bool	NetworkMoudle::ReadPackageHead(PackageHead& head,bool withTimeOut)
{
	char buf[1024] = {0};

	int len = 0;

	if (withTimeOut)
	{
		len = TryRead(buf,PACKAGE_HEAD_LEN);
	}
	else
	{
		len = Read(buf,PACKAGE_HEAD_LEN);
	}

	if (len <= 0)
	{
		return false;
	}

	head.Analyze(buf,len);

	return true;
}

bool	NetworkMoudle::ReadControlPackBody(PackageHead& head,ControlPackage* pack,bool withTimeOut)
{
	char buf[1024] = {0};

	int len = 0;

	pack->m_PackageHead = head;

	memset(buf,0,SZ_1024_LENGTH);

	if (withTimeOut)
	{
		len = TryRead(buf,head.GetPackageLength());//recv(fd,controlBuffer,head.GetPackageLength(),0);
	}
	else
	{
		len = Read(buf,head.GetPackageLength());
	}

	if (len <= 0)
	{
		return false;
	}

	pack->Analyze(buf,strlen(buf));
	pack->Print();

	return true;
}

bool	NetworkMoudle::ReadDataPackBody(PackageHead& head,DataPackage* pack,bool withTimeOut)
{
	char buf[1024] = {0};

	int len = 0;

	pack->m_PackageHead = head;

	memset(buf,0,SZ_1024_LENGTH);

	if (withTimeOut)
	{
		len = TryRead(buf,head.GetPackageLength());//recv(fd,controlBuffer,head.GetPackageLength(),0);
	}
	else
	{
		len = Read(buf,head.GetPackageLength());
	}

	if (len <= 0)
	{
		return false;
	}

	pack->Analyze(buf,strlen(buf));

	return true;
}

bool	NetworkMoudle::ReadControlPack(ControlPackage* pack,bool withTimeOut)		// ��ȡ���ư�
{

	// ���ȶ�ȡͷ��
	PackageHead head;

	char buf[1024] = {0};

	int len = 0;

	if (withTimeOut)
	{
		len = TryRead(buf,PACKAGE_HEAD_LEN);//recv(fd,buf,PACKAGE_HEAD_LEN,0);
	}
	else
	{
		len = Read(buf,PACKAGE_HEAD_LEN);//recv(fd,buf,PACKAGE_HEAD_LEN,0);
	}

	if (len <= 0)
	{
		return false;
	}

	head.Analyze(buf,len);

	pack->m_PackageHead = head;

	memset(buf,0,SZ_1024_LENGTH);

	if (withTimeOut)
	{
		len = TryRead(buf,head.GetPackageLength());//recv(fd,controlBuffer,head.GetPackageLength(),0);
	}
	else
	{
		len = Read(buf,head.GetPackageLength());
	}

	if (len <= 0)
	{
		return false;
	}

	pack->Analyze(buf,len);

	return true;

}

void	NetworkMoudle::SendControlPack(ControlPackage* pack) // ���Ϳ��ư�
{

	char sendBuf[SZ_1024_LENGTH] = {0};

	int len = pack->GetPackageBuffer(sendBuf);

	Write(sendBuf,len);
}

void	NetworkMoudle::SendDataPack(DataPackage& pack) // �������ݰ�
{
	char buf[SZ_1024_LENGTH] = {0};
	int len = pack.GetPackageBuffer(buf);
	Write(buf,len);
}

bool	NetworkMoudle::ReadDataPack(DataPackage& pack,bool withTimeOut)
{
	char buf[SZ_1024_LENGTH] = {0};

	int len = 0;
	if(withTimeOut)
	{
		len = TryRead(buf,PACKAGE_HEAD_LEN);//recv(m_nServerfd,buf,PACKAGE_HEAD_LEN,0);
	}
	else
	{
		len = Read(buf,PACKAGE_HEAD_LEN);
	}

	if (len <= 0)
	{
		return false;
	}

	pack.m_PackageHead.Analyze(buf,PACKAGE_HEAD_LEN);
	memset(buf,0,SZ_1024_LENGTH);

	if(withTimeOut)
	{
		len = TryRead(buf,pack.m_PackageHead.GetPackageLength());//recv(m_nServerfd,buf,PACKAGE_HEAD_LEN,0);
	}
	else
	{
		len = Read(buf,pack.m_PackageHead.GetPackageLength());
	}

	if (len <= 0)
	{
		return false;
	}
	pack.Analyze(buf,len);
	return true;

}


// ҵ���� begin
bool	NetworkMoudle::Login(const string& id,const string& password)
{
	// ����һ�����ư�
	ControlPackage pack;

	// ���ÿ��ƴ��루��½��
	pack.SetPackageCode(code_login);

	// ������������
	pack.SetUserID(id);
	pack.SetUserPassword(password);

	// ���Ϳ��ư�
	SendControlPack(&pack);

	char controlBuffer[SZ_1024_LENGTH] = {0};

	// ��ȡ�������ķ��صĿ��ư�
	bool bOK = ReadControlPack(&pack);
	if (!bOK)
	{
		return false;
	}

	// �жϻ�Ӧ��
	if (pack.GetPackageCode() != code_login_ret_ok)
	{
		return false;
	}

	return true;

}

void NetworkMoudle::Logout()
{

}

void	NetworkMoudle::StopGame()
{
	ControlPackage pack;
	pack.SetPackageCode(code_leave);
	SendControlPack(&pack);
}

bool	NetworkMoudle::CreateGame(const string& strGameName,int nHeroID)
{
	ControlPackage pack;

	pack.SetUserHeroID(nHeroID);
	pack.SetGameName(strGameName);
	pack.SetIsCreator(true);

	pack.SetPackageCode(code_create_game);

	SendControlPack(&pack);

	bool bOK = ReadControlPack(&pack);

	if (!bOK)
	{
		return false;
	}

	if (pack.GetPackageCode() == code_create_game_ret_failed)
	{
		return false;
	}

	return true;
}
vector<GameRoundInfo>& NetworkMoudle::GetGameRoundList()
{
	return m_GameRoundList;
}

GameRoundInfo* NetworkMoudle::GetFirstGame()
{
	if (m_GameRoundList.size() == 0)
	{
		return 0;
	}
	return &(*(m_GameRoundList.begin()));
}

GameRoundInfo* NetworkMoudle::GetGameRound(int nGameIndex)
{
	vector<GameRoundInfo>::iterator it;

	for (it = m_GameRoundList.begin(); it != m_GameRoundList.end(); ++it)
	{
		if ((*it).m_nGameIndex == nGameIndex)
		{
			return &(*it);
		}
	}

	return 0;
}

bool	NetworkMoudle::RequestAllGameInfo()
{
	// ����һ�����ư�
	ControlPackage pack;

	// ������ƴ���
	pack.SetPackageCode(code_join_game_req_gameinfo);

	// ���Ϳ��ư�
	SendControlPack(&pack);

	char controlBuffer[SZ_1024_LENGTH] = {0};

	// ��ȡ�������������Ŀ��ư�
	bool bOK = ReadControlPack(&pack);
	if (!bOK)
	{
		return false;
	}

	// �жϻ�Ӧ��
	if (pack.GetPackageCode() != code_join_game_rsp_gameinfo)
	{
		return false;
	}

	// �ӿ��ư��ж�ȡ����
	int game_count = pack.GetGameRoundWaitCount();
	if (game_count == 0)
	{
		return false;
	}

	GameRoundInfo gameRound;

	char buf[SZ_4096_LENGTH] = {0};

	// ��ȡ���ư��е��������ݣ���Ϸ��Ϣ����ڴ˴���
	memcpy(buf,pack.GetOtherInfo().c_str(),pack.GetOtherInfo().size());

	NetAchive ar(buf,strlen(buf),NetAchive::LOAD_MODE);

	// ������Ϸ��Ϣ
	for (int i = 0; i < game_count; ++i)
	{
		gameRound.m_nGameIndex = (ar.Load_int());
		gameRound.m_strCreateUserID = (ar.Load_string());
		gameRound.m_nCreateHeroID = (ar.Load_int()) ;
		m_GameRoundList.push_back(gameRound);
	}

	return true;
}

bool	NetworkMoudle::JoinGame(int nGameIndex,int nHeroID)
{
	ControlPackage pack;
	pack.SetGameRoundIndex(nGameIndex);
	pack.SetUserHeroID(nHeroID);

	pack.SetPackageCode(code_join_game);

	SendControlPack(&pack);

	char controlBuffer[SZ_1024_LENGTH] = {0};

	bool bOK = ReadControlPack(&pack);

	if (!bOK)
	{
		return false;
	}

	if (pack.GetPackageCode() == code_join_game_ret_failed)
	{
		return false;
	}

	return true;
}


void NetworkMoudle::SuspendGame()
{
	ControlPackage pack;

	pack.SetPackageCode(code_suspend_game);

	SendControlPack(&pack);
}

void NetworkMoudle::ContinueGame()
{
	ControlPackage pack;

	pack.SetPackageCode(code_continue_game);

	SendControlPack(&pack);
}
// ҵ���� end