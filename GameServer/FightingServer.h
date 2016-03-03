#ifndef GAMESERVER_H
#define GAMESERVER_H

#if (defined(WIN32) || defined(WIN64))
#include "../CommonClass/SelectSocket.h"
#include "../CommonClass/Package.h"
#else
#include "../CommonClass/SelectSocket.h"
#include "../CommonClass/Package.h"
#endif

#include "ServerLogic.h"
#include "../CommonClass/Player.h"
#include "../CommonClass/GameRound.h"

/************************************************************************
*  ��Ϸ����������
************************************************************************/
class FightingServer:public SelectSocket
{
public:
    FightingServer()
    {
        m_pServerLogic = new ServerLogic(this);
    }

    ~FightingServer()
    {
		if (m_pServerLogic != 0)
		{
			delete m_pServerLogic;
			m_pServerLogic = 0;
		}
		
		Close();
    }

    void				HandleAccpet(SOCKET fd,sockaddr_in& addr)
    {
		m_pServerLogic->PlayerArrive(fd,addr);		// ��ҽ��������
    }

    void				HandleRead(SOCKET fd,int nBytes)
    {
		PackageHead head;

		if (!ReadPackageHead(fd,head))										// �ȶ�ȡ��ͷ��
		{
			return;
		}
		
		if (head.GetPackageType() == package_control)			// �жϰ������ͣ����ư�
		{
			ControlPackage pack;
			pack.m_PackageHead = head;

			char controlBuffer[SZ_1024_LENGTH] = {0};
			ReadControlPack(fd,controlBuffer,head);					// ���հ���ʣ�ಿ��

			pack.Analyze(controlBuffer,strlen(controlBuffer));		// ������������

			m_pServerLogic->Analyze(fd,&pack);						// ���о����ҵ���߼�

			pack.Print();
		}
		else if(head.GetPackageType() == package_data)			// ���ݰ�
		{
			DataPackage pack;
			char buf[SZ_1024_LENGTH] = {0};
			ReadDataPackBuffer(fd,head,buf);
			pack.m_PackageHead = head;

			Player* sender = m_pServerLogic->GetPlayer(fd);
			if (0 == sender)
			{
				return;
			}
			m_pServerLogic->TransmitDataPackBuffer(sender,&head,buf);		// ����ת��
		}
    }

    void				HandleClose(SOCKET fd)
    {
		Player* pPlay = m_pServerLogic->GetPlayer(fd);			// �ͻ��Ͽ�����

		if (pPlay != 0)
		{
			m_pServerLogic->PlayerLeave(pPlay,0);
		}

        cout << "Client Leave!" << endl;
    }

	// ��ȡ��ͷ��
	bool				 ReadPackageHead(SOCKET fd,PackageHead& head)
	{
		char buf[1024] = {0};
		int len = recv(fd,buf,PACKAGE_HEAD_LEN,0);

		if(len <= 0)
			return false;

		head.Analyze(buf,len);
		return true;
	}

	// ��ȡ���ư���ͷ�����������Ϣ
    void            ReadControlPack(SOCKET fd,char* controlBuffer,PackageHead& head)
    {
        recv(fd,controlBuffer,head.GetPackageLength(),0);
		cout << controlBuffer << endl;
    }

	// ��ȡ���ݰ���ͷ�����������Ϣ
	void			  ReadDataPackBuffer(SOCKET fd,PackageHead& head,char* retBuffer)
	{
		int len = recv(fd,retBuffer,head.GetPackageLength(),0);
	}

	// �������ݰ�
	void			  SendDataPackBuffer(SOCKET fd,PackageHead& head,char* buffer) // �������ݰ�
	{
		char buf[SZ_1024_LENGTH] = {0};
		head.GetHeadBuffer(buf,PACKAGE_HEAD_LEN);
		send(fd,buf,PACKAGE_HEAD_LEN,0);
		send(fd,buffer,strlen(buffer),0);
	}

	// ���Ϳ��ư�
    void            SendControlPack(SOCKET fd,ControlPackage* controlPackage)
    {
        char sendBuf[SZ_1024_LENGTH] = {0};

		int len = controlPackage->GetPackageBuffer(sendBuf);

        send(fd,sendBuf,len,0);
    }

private:
    ServerLogic*									m_pServerLogic;			// �����߼���
};

#endif // GAMESERVER_H
