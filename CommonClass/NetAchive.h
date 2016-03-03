#ifndef NETACHIVE_H
#define NETACHIVE_H

#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>

using namespace std;

// ÍøÂçÐòÁÐ°ü
class NetAchive
{
public:
	const static int SAVE_MODE = 0;
	const static int LOAD_MODE = 1;

	NetAchive(char* buf,int len,int mode,char cSpilitChar = '|')
	{
		Reset(buf,len,mode,cSpilitChar);
	}

	void        Clean()
	{
		m_pBuffer = 0;
		m_nBufferLen = 0;
		m_nPosition = 0;
		m_cSpilitChar = '|';
	}

	void        Save_string(const string& str)
	{
		string strTemp = str;
		if (str.size() == 0)
		{
			strTemp = "";
		}
		
		strTemp += "|";
		m_nBufferLen += strTemp.size();
		memcpy(m_pBuffer + m_nPosition,strTemp.c_str(),strTemp.size());
		m_nPosition += strTemp.size();
	}

	string      Load_string()
	{
		char buf[1024 * 8] = {0};
		int index = 0;
		while(m_nPosition < m_nBufferLen)
		{
			if(m_pBuffer[m_nPosition] != '|')
			{
				buf[index++] = m_pBuffer[m_nPosition];
				m_nPosition++;
			}
			else
			{
				m_nPosition++;
				break;
			}
		}

		return buf;
	}

	string      Load_string(int len)
	{
		if (len <= 0)
		{
			return "";
		}

		
		char buf[1024 * 8] = {0};
		int index = 0;
		while(m_nPosition < m_nBufferLen && index < len)
		{
			buf[index++] = m_pBuffer[m_nPosition];
			
			m_nPosition++;
			
		}

		return buf;
	}

	void        Save_int(int v)
	{
		char buf[16] = {0};
		sprintf(buf,"%d",v);
		string strTemp = buf;
		strTemp += "|";
		m_nBufferLen += strTemp.size();
		memcpy(m_pBuffer + m_nPosition,strTemp.c_str(),strTemp.size());
		m_nPosition += strTemp.size();
	}

	int         Load_int()
	{
		string str = Load_string();
		return atoi(str.c_str());
	}

	void		Save_bool(bool v)
	{
		if (v == true)
		{
			Save_int(1);
		}
		else
		{
			Save_int(0);
		}
	}

	bool		Load_bool()
	{
		int v = Load_int();
		if (1 == v)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void		Save_float(float v)
	{
		char buf[64] = {0};
		sprintf(buf,"%f",v);
		string strTemp = buf;
		strTemp += "|";
		m_nBufferLen += strTemp.size();
		memcpy(m_pBuffer + m_nPosition,strTemp.c_str(),strTemp.size());
		m_nPosition += strTemp.size();
	}

	float		Load_float()
	{
		string str = Load_string();
		return atof(str.c_str());
	}

	int        GetBufferLength()
	{
		return m_nBufferLen;
	}

	void        Reset(char* buf,int len,int mode,char cSpilitChar = '|')
	{
		Clean();

		m_cSpilitChar = cSpilitChar;

		if(LOAD_MODE == mode)
			m_nBufferLen = len;

		m_pBuffer = buf;
	}

private:
	char*       m_pBuffer;
	int         m_nBufferLen;
	int         m_nPosition;
	char		m_cSpilitChar;
};

#endif
