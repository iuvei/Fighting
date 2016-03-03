#ifndef SELECTSOCKET_H
#define SELECTSOCKET_H

// ���е�ͷ�ļ�

#include <string.h>
#include <time.h>
#include <stdio.h>

#include <stdarg.h>

#include <iostream>
#include <fstream>
#include <map>
#include <deque>
#include <vector>
#include <set>
#include <list>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#if (defined(WIN32) || defined(WIN64))
#include <direct.h>
#include <process.h>
#include <tchar.h>
#include <WINSOCK2.H>
#include <Mswsock.h>
#include <windows.h>
#ifdef _DEBUG
#include <assert.h>
#endif

#include <io.h>
#include <winsock.h>
#include <wininet.h>

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#pragma comment(lib,"WS2_32")
#pragma comment(lib,"odbc32")

#else
#include<pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sys/types.h>		/* some systems still require this */
#include <sys/stat.h>
#include <sys/termios.h>	/* for winsize */

#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <stddef.h>		/* for offsetof */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <signal.h>		/* for SIG_ERR */

#endif

using namespace std;

// ������ֳ���
#ifndef SZ_LENGTH
#define SZ_LENGTH 128
#endif

#ifndef SZ_S_LENGTH
#define SZ_S_LENGTH 64
#endif

#ifndef SZ_L_LENGTH
#define SZ_L_LENGTH 512
#endif

#ifndef SZ_8_LENGTH
#define SZ_8_LENGTH 8
#endif

#ifndef SZ_16_LENGTH
#define SZ_16_LENGTH 16
#endif

#ifndef SZ_32_LENGTH
#define SZ_32_LENGTH 32
#endif

#ifndef SZ_1024_LENGTH
#define SZ_1024_LENGTH 1024
#endif

#ifndef SZ_2048_LENGTH
#define SZ_2048_LENGTH 2048
#endif

#ifndef SZ_4096_LENGTH
#define SZ_4096_LENGTH 4096
#endif

// ��windows������û��socklen_t���ͣ�������Ҫ����
#if (defined(WIN32) || defined(WIN64))
#define socklen_t int
#endif

// ��Linux��û��SOCKET���ͣ�����Ҫ����
#if (defined(WIN32) || defined(WIN64))
#else
typedef  int								SOCKET;
#endif

// windows��Linux���׽��ֽ��п��ƺ͹رյĺ�������ͬ
// ����Ϊ��һ�´������Զ�����һЩ��
#if (defined(WIN32) || defined(WIN64))
#define IoctlSocket ioctlsocket
#define CloseSocket closesocket
#else
#define IoctlSocket ioctl
#define CloseSocket close
#endif

// ��windows�Ļ����£���ʹ���׽���֮ǰ��Ҫ����һЩ��̬��
// PlatformInitSocket�����þ���Ϊ�˷�����windows�¼�����Щ��̬��
// ��Linux������PlatformInitSocketû���κ�����
#if (defined(WIN32) || defined(WIN64))
class PlatformInitSocket
{
public:
	// ���캯�����ж�̬�����
	PlatformInitSocket(int iminorVer = 2,int imajorVer = 2)
	{
		WSADATA wsaData;
		WORD socketVersion = MAKEWORD(iminorVer,imajorVer);
		if (WSAStartup(socketVersion,&wsaData) != 0)
		{
			exit(0);
		}
	}

	// �����������ж�̬���ͷ�
	~PlatformInitSocket()
	{
		WSACleanup();
	}
};

#else
class PlatformInitSocket
{

};
#endif



// selectģʽ���׽���
// ���ڷ������ˣ��û���Ҫ����һ����̳���SelectSocket
// ʵ��HandleAccpet��HandleRead��HandleClose�������麯��
// Ȼ����ù��̾��ǣ�
//		1��CreateServer
//		2��ServerLoop
class SelectSocket
{
public:
	SelectSocket()
	{
		FD_ZERO(&m_readSet);
		m_nMaxSocket = -1;
		m_serverSocketfd = -1;
	}

	~SelectSocket()
	{
		if (m_serverSocketfd != -1)
		{
			Close();
		}	
	}

	// ����ip�Ͷ˿ڴ���һ�������׽���
	// ���ip==0����ô��ʾ����������ϵ�����һ����ַ
	void    CreateServer(const char* ip,unsigned int port)
	{
		m_serverSocketfd = socket(AF_INET,SOCK_STREAM,0);

		m_Addr.sin_family = AF_INET;

		if (0 == ip)
		{
			m_Addr.sin_addr.s_addr =htonl(INADDR_ANY);	
		}
		else
		{
			m_Addr.sin_addr.s_addr = inet_addr(ip);
		}

		m_Addr.sin_port = htons(port);

		bind(m_serverSocketfd,(sockaddr*)&m_Addr,sizeof(sockaddr_in));

		listen(m_serverSocketfd,5);
	}

	// ��������ѭ��
	void    ServerLoop()
	{
		FD_SET(m_serverSocketfd,&m_readSet);
		m_nMaxSocket = m_serverSocketfd;

		for(;;)
		{
			// ���ó�ʱ
			fd_set readSet = m_readSet;
			timeval timeout;
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;

			// ����select����
			int ret = Select(m_nMaxSocket,&readSet,0,0,&timeout);//select(0,&readSet,0,0,&timeout);

			// ����
			if(ret == -1)
			{
				break;
			}

			// ��ʱ
			if(ret == 0)
			{
				continue;
			}

			// �׽��ֿɶ��������пͻ�������
			// ��Linux��windows��fd_set�Ľṹ��Щ����ͬ
			// ������Ҫ�ֱ���
#if (defined(WIN32) || defined(WIN64))
			for (int i = 0 ; i < (int)m_readSet.fd_count; ++i)
#else
			for(int fd = 0; fd < m_nMaxSocket + 1; ++fd)
#endif
			{
#if (defined(WIN32) || defined(WIN64))
				if(FD_ISSET(m_readSet.fd_array[i], &readSet) == 0)	
					continue;

				int fd = m_readSet.fd_array[i];
#else
				if(FD_ISSET(fd,&readSet) == 0)
					continue;
#endif
				// �ɶ����׽����Ǽ����׽��֣���ô��ʾ�пͻ�����
				if(fd == m_serverSocketfd)
				{
					sockaddr_in addr;
					int addr_len = sizeof(sockaddr_in);
					int client_fd = accept(m_serverSocketfd,(sockaddr*)&addr,(socklen_t*)&addr_len);
					if(client_fd > m_nMaxSocket)
						m_nMaxSocket = client_fd;

					FD_SET(client_fd,&m_readSet);
					HandleAccpet(client_fd,addr);
					continue;
				}

				// ���������ʾһ����׽��ֿɶ�
				unsigned long nRead = 0;

				// ��ȡ�ɶ����ֽ�����
				// ����ɶ������ǿɶ����ֽ�������Ϊ0����ô��ʾ�ͻ��˹ر�
				IoctlSocket(fd,FIONREAD,&nRead);

				if(nRead == 0)
				{
					// client leave
					HandleClose(fd);
					CloseSocket(fd);
					FD_CLR(fd,&m_readSet);

				}
				else
				{
					HandleRead(fd,nRead);
				}
			}

		}

	}

	// �ر��׽���
	void				Close()
	{
		CloseSocket(m_serverSocketfd);
	}

	// ����ͻ�����
	virtual void    HandleAccpet(SOCKET fd,sockaddr_in& addr)
	{
	}

	// �������ݿɶ�
	virtual void    HandleRead(SOCKET fd,int nBytes)
	{

	}

	// ����ͻ��뿪
	virtual void    HandleClose(SOCKET fd)
	{

	}

private:
	int				Select(int maxSocket,fd_set* r_s,fd_set* w_s,fd_set* e_s,timeval* timeout)
	{
#if (defined(WIN32) || defined(WIN64))
		int ret = select(0,r_s,w_s,e_s,timeout);
#else
		int ret = select(maxSocket + 1,r_s,w_s,e_s,timeout);
#endif
		return ret;
	}

private:
	fd_set					m_readSet;						// �׽��ֵĿɶ�����
	SOCKET				m_serverSocketfd;			// �����׽���
	sockaddr_in			m_Addr;							// ��ַ
	int						m_nMaxSocket;				// �����׽��֣�����Linux�£�

	PlatformInitSocket m_PlatformInitSocket;		// ƽ̨��ص��׽��ֻ�����ʼ��
};

// selectģʽ���׽���
// ���ڿͻ��ˣ��û���Ҫ����һ����̳���SelectSocketClient
class SelectSocketClient
{
public:
	SelectSocketClient()
	{
		m_nMaxSocket = -1;
		m_serverSocketfd = -1;
	}

	~SelectSocketClient()
	{
		if (m_serverSocketfd != -1)
		{
			Close();
		}	
	}

	// ���ӵ�������������ֵ��ʾ�ɹ�����ʧ��
	bool				ConnectToServer(const char* ip,unsigned int port)
	{
		m_serverSocketfd = socket(AF_INET,SOCK_STREAM,0);

		m_Addr.sin_family = AF_INET;
		m_Addr.sin_addr.s_addr = inet_addr(ip);
		m_Addr.sin_port = htons(port);

		if (-1 == connect(m_serverSocketfd,(sockaddr*)&m_Addr,sizeof(m_Addr)))
		{
			return false;
		}

		return true;
	}

	// û�г�ʱ���õĶ����������ݣ�
	int				Read(char* buf,int len)
	{
		return recv(m_serverSocketfd,buf,len,0);
	}

	// �г�ʱ���õĶ����������ݣ�
	int				TryRead(char* buf,int len)
	{
		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(m_serverSocketfd,&readSet);
		m_nMaxSocket = m_serverSocketfd;

		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;

		int ret = Select(m_nMaxSocket,&readSet,0,0,&timeout);//select(0,&readSet,0,0,&timeout);

		if(ret == -1)
		{
			return -1;
		}

		if(ret == 0)
		{
			return 0;
		}

#if (defined(WIN32) || defined(WIN64))
		if(FD_ISSET(m_serverSocketfd, &readSet) == 0)	
			return 0;
#else
		if(FD_ISSET(m_serverSocketfd,&readSet) == 0)
			return 0;
#endif

		unsigned long nRead = 0;

		IoctlSocket(m_serverSocketfd,FIONREAD,&nRead);

		return recv(m_serverSocketfd,buf,len,0);
	}

	// д���׽����У������ͣ�
	int				Write(char* buf,int len)
	{
		return send(m_serverSocketfd,buf,len,0);
	}

	// �ر��׽���
	void				Close()
	{
		CloseSocket(m_serverSocketfd);
	}

private:
	int				Select(int maxSocket,fd_set* r_s,fd_set* w_s,fd_set* e_s,timeval* timeout)
	{
#if (defined(WIN32) || defined(WIN64))
		int ret = select(0,r_s,w_s,e_s,timeout);
#else
		int ret = select(maxSocket + 1,r_s,w_s,e_s,timeout);
#endif
		return ret;
	}

private:
	SOCKET				m_serverSocketfd;					// ���ӷ��������׽���
	sockaddr_in			m_Addr;									// ��ַ
	int						m_nMaxSocket;						// �׽��ֵ��������

	PlatformInitSocket m_PlatformInitSocket;				// ���ڳ�ʼ���׽���
};

#endif // SELECTSOCKET_H
