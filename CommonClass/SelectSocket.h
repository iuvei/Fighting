#ifndef SELECTSOCKET_H
#define SELECTSOCKET_H

// 所有的头文件

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

// 定义各种长度
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

// 在windows环境下没有socklen_t类型，所以需要定义
#if (defined(WIN32) || defined(WIN64))
#define socklen_t int
#endif

// 在Linux下没有SOCKET类型，所以要定义
#if (defined(WIN32) || defined(WIN64))
#else
typedef  int								SOCKET;
#endif

// windows和Linux对套接字进行控制和关闭的函数不相同
// 这里为了一致处理所以定义了一些宏
#if (defined(WIN32) || defined(WIN64))
#define IoctlSocket ioctlsocket
#define CloseSocket closesocket
#else
#define IoctlSocket ioctl
#define CloseSocket close
#endif

// 在windows的环境下，在使用套接字之前需要加载一些动态库
// PlatformInitSocket的作用就是为了方便在windows下加载这些动态库
// 在Linux环境下PlatformInitSocket没有任何作用
#if (defined(WIN32) || defined(WIN64))
class PlatformInitSocket
{
public:
	// 构造函数进行动态库加载
	PlatformInitSocket(int iminorVer = 2,int imajorVer = 2)
	{
		WSADATA wsaData;
		WORD socketVersion = MAKEWORD(iminorVer,imajorVer);
		if (WSAStartup(socketVersion,&wsaData) != 0)
		{
			exit(0);
		}
	}

	// 析构函数进行动态库释放
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



// select模式的套接字
// 用于服务器端，用户需要定义一个类继承自SelectSocket
// 实现HandleAccpet、HandleRead、HandleClose这三个虚函数
// 然后调用过程就是：
//		1、CreateServer
//		2、ServerLoop
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

	// 根据ip和端口创建一个监听套接字
	// 如果ip==0，那么表示监听计算机上的任意一个地址
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

	// 服务器主循环
	void    ServerLoop()
	{
		FD_SET(m_serverSocketfd,&m_readSet);
		m_nMaxSocket = m_serverSocketfd;

		for(;;)
		{
			// 设置超时
			fd_set readSet = m_readSet;
			timeval timeout;
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;

			// 调用select函数
			int ret = Select(m_nMaxSocket,&readSet,0,0,&timeout);//select(0,&readSet,0,0,&timeout);

			// 出错
			if(ret == -1)
			{
				break;
			}

			// 超时
			if(ret == 0)
			{
				continue;
			}

			// 套接字可读（或者有客户到来）
			// 在Linux和windows下fd_set的结构有些不相同
			// 所以需要分别处理
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
				// 可读的套接字是监听套接字，那么表示有客户接入
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

				// 来到这里表示一般的套接字可读
				unsigned long nRead = 0;

				// 获取可读的字节数量
				// 如果可读，但是可读的字节数量又为0，那么表示客户端关闭
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

	// 关闭套接字
	void				Close()
	{
		CloseSocket(m_serverSocketfd);
	}

	// 处理客户接入
	virtual void    HandleAccpet(SOCKET fd,sockaddr_in& addr)
	{
	}

	// 处理数据可读
	virtual void    HandleRead(SOCKET fd,int nBytes)
	{

	}

	// 处理客户离开
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
	fd_set					m_readSet;						// 套接字的可读集合
	SOCKET				m_serverSocketfd;			// 监听套接字
	sockaddr_in			m_Addr;							// 地址
	int						m_nMaxSocket;				// 最大的套接字（用于Linux下）

	PlatformInitSocket m_PlatformInitSocket;		// 平台相关的套接字环境初始化
};

// select模式的套接字
// 用于客户端，用户需要定义一个类继承自SelectSocketClient
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

	// 连接到服务器，返回值表示成功或者失败
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

	// 没有超时设置的读（接收数据）
	int				Read(char* buf,int len)
	{
		return recv(m_serverSocketfd,buf,len,0);
	}

	// 有超时设置的读（接收数据）
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

	// 写到套接字中（即发送）
	int				Write(char* buf,int len)
	{
		return send(m_serverSocketfd,buf,len,0);
	}

	// 关闭套接字
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
	SOCKET				m_serverSocketfd;					// 连接服务器的套接字
	sockaddr_in			m_Addr;									// 地址
	int						m_nMaxSocket;						// 套接字的最大数量

	PlatformInitSocket m_PlatformInitSocket;				// 用于初始化套接字
};

#endif // SELECTSOCKET_H
