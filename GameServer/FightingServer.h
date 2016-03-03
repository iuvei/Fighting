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
*  游戏服务器主类
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
		m_pServerLogic->PlayerArrive(fd,addr);		// 玩家接入服务器
    }

    void				HandleRead(SOCKET fd,int nBytes)
    {
		PackageHead head;

		if (!ReadPackageHead(fd,head))										// 先读取包头部
		{
			return;
		}
		
		if (head.GetPackageType() == package_control)			// 判断包的类型：控制包
		{
			ControlPackage pack;
			pack.m_PackageHead = head;

			char controlBuffer[SZ_1024_LENGTH] = {0};
			ReadControlPack(fd,controlBuffer,head);					// 接收包的剩余部分

			pack.Analyze(controlBuffer,strlen(controlBuffer));		// 解析包的数据

			m_pServerLogic->Analyze(fd,&pack);						// 进行具体的业务逻辑

			pack.Print();
		}
		else if(head.GetPackageType() == package_data)			// 数据包
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
			m_pServerLogic->TransmitDataPackBuffer(sender,&head,buf);		// 进行转发
		}
    }

    void				HandleClose(SOCKET fd)
    {
		Player* pPlay = m_pServerLogic->GetPlayer(fd);			// 客户断开连接

		if (pPlay != 0)
		{
			m_pServerLogic->PlayerLeave(pPlay,0);
		}

        cout << "Client Leave!" << endl;
    }

	// 读取包头部
	bool				 ReadPackageHead(SOCKET fd,PackageHead& head)
	{
		char buf[1024] = {0};
		int len = recv(fd,buf,PACKAGE_HEAD_LEN,0);

		if(len <= 0)
			return false;

		head.Analyze(buf,len);
		return true;
	}

	// 读取控制包除头部外的其他信息
    void            ReadControlPack(SOCKET fd,char* controlBuffer,PackageHead& head)
    {
        recv(fd,controlBuffer,head.GetPackageLength(),0);
		cout << controlBuffer << endl;
    }

	// 读取数据包除头部外的其他信息
	void			  ReadDataPackBuffer(SOCKET fd,PackageHead& head,char* retBuffer)
	{
		int len = recv(fd,retBuffer,head.GetPackageLength(),0);
	}

	// 发送数据包
	void			  SendDataPackBuffer(SOCKET fd,PackageHead& head,char* buffer) // 发送数据包
	{
		char buf[SZ_1024_LENGTH] = {0};
		head.GetHeadBuffer(buf,PACKAGE_HEAD_LEN);
		send(fd,buf,PACKAGE_HEAD_LEN,0);
		send(fd,buffer,strlen(buffer),0);
	}

	// 发送控制包
    void            SendControlPack(SOCKET fd,ControlPackage* controlPackage)
    {
        char sendBuf[SZ_1024_LENGTH] = {0};

		int len = controlPackage->GetPackageBuffer(sendBuf);

        send(fd,sendBuf,len,0);
    }

private:
    ServerLogic*									m_pServerLogic;			// 处理逻辑类
};

#endif // GAMESERVER_H
