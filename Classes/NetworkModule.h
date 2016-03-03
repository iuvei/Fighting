#ifndef _NETWORK_MODULE_
#define _NETWORK_MODULE_

#include "../CommonClass/Package.h"
#include "../CommonClass/NetAchive.h"
#include "../CommonClass/SelectSocket.h"
#include "../CommonClass/GameRound.h"

// ����߳�֧��
#include <thread>
#include <mutex>
// ����߳�֧��

//#define _USE_THREAD_

using namespace std;
// ����ģ��

class NetworkMoudle:public SelectSocketClient
{
public:
	NetworkMoudle();

	virtual ~NetworkMoudle();

	bool						Connect(const char* strIP,int nPort)	;		// ���ӷ�����									

	bool						IsConnect();												// �ж��Ƿ��Ѿ����ӷ�����

	bool						ReadPackageHead(PackageHead& head,bool withTimeOut = false);		// ��ȡ��ͷ��

	bool						ReadControlPackBody(PackageHead& head,ControlPackage* pack,bool withTimeOut = false);	// ��ȡ���ư�
		
	bool						ReadDataPackBody(PackageHead& head,DataPackage* pack,bool withTimeOut = false);			// ��ȡ���ݰ�

	bool						ReadControlPack(ControlPackage* pack,bool withTimeOut = false);											// ��ȡ���ư�

	void						SendControlPack(ControlPackage* pack) ;																				// ���Ϳ��ư�

	void						SendDataPack(DataPackage& pack);																						// �������ݰ�

	bool						ReadDataPack(DataPackage& pack,bool withTimeOut = false);													// ��ȡ���ݰ�

	// ҵ���� begin

	// ��½
	bool						Login(const string& id,const string& password);																		// ��½

	// ע��
	void						Logout();																																	// ע��

	// ע��
	void						Register();																																	// ע��

	// ����һ����Ϸ
	bool						CreateGame(const string& strGameName,int nHeroID);															// ������Ϸ

	// ����һ����Ϸ
	bool						JoinGame(int nGameIndex,int nHeroID) ;																					// ������Ϸ

	void						SuspendGame();																														// ��ͣ��Ϸ

	void						ContinueGame();																														// ������Ϸ

	void						StopGame();																																// ֹͣ��Ϸ

	bool										RequestAllGameInfo();																								// �������������Ϸ��Ϣ

	vector<GameRoundInfo>& GetGameRoundList();																								// ��ȡ��Ϸ�б�

	GameRoundInfo*					GetFirstGame();																											// ��ȡ��һ����Ϸ

	GameRoundInfo*					GetGameRound(int nGameIndex);																				// ��ȡһ����Ϸ

	// ҵ���� end

private:


	int												m_nServerPort;								// �������˿�

	string											m_strIP;										// ������IP

	PlatformInitSocket						 m_PlatformInitSocket;					// ���ڲ�ͬƽ̨���׽��ֳ�ʼ��


	vector<GameRoundInfo>			m_GameRoundList;						//��Ϸ��Ϣ�б�

	bool												m_bIsConnect;								// �Ƿ��Ѿ����ӵ�������
};


#endif