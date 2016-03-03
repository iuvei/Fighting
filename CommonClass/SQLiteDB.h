#ifndef _SQLITE_DB_H_
#define _SQLITE_DB_H_

#include "./sqlite-src/sqlite3.h"
#include <string>
#include <vector>
using namespace std;

/*
** SQLite���ݿ���
*/

// ���ݿ�������
class SQLiteConnector
{
public:
	friend class SQLiteExcer;

	SQLiteConnector()
	{
		m_pDB = 0;
		m_bIsConnect = false;
	}

	~SQLiteConnector()
	{
		Close();
	}

	bool					Connect(const char* pszDBFileName)			// ���ӵ����ݿ�
	{
		if (0 == pszDBFileName)
		{
			return false;
		}

		int nRet = sqlite3_open_v2(pszDBFileName, &m_pDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); 

		if (nRet)
		{
			ErrorMsg(sqlite3_errmsg(m_pDB));
			Close();
			return false;
		}
		m_bIsConnect = true;
		return true;
	}

	void					Close()															// �ر����ݿ�
	{
		if (m_bIsConnect == true)
		{
			sqlite3_close(m_pDB);
		}
		m_bIsConnect = false;
		m_pDB = 0;
	}

	void					ErrorMsg(const char* msg)
	{
		printf("[SQLite Error]: %s\n",msg);
	}

	sqlite3*				GetHandler()													// ��ȡ���Ӷ���
	{
		return m_pDB;
	}

	bool					IsConnect()													// �Ƿ����ӳɹ�
	{
		return m_bIsConnect;
	}

private:
	bool							m_bIsConnect;
	sqlite3*						m_pDB;
};

// ������У���
class SQLiteRow
{
public:
	friend class SQLiteExcer;
	SQLiteRow():m_nFieldCount(0),m_nCurrentFieldIndex(0){}

	int                 GetFieldCount()							// ��ȡ�е�����
	{
		return m_nFieldCount;
	}

	const char*             GetValue(int index)			// ������������ȡ��Ӧֵ
	{
		if (index >= m_nCurrentFieldIndex)
		{
			return 0;
		}
		return m_NameValueMap[index].value.c_str();
	}

	const char*				GetFileldName(int index)	// ����������ȡ����
	{
		if (index >= m_nCurrentFieldIndex)
		{
			return 0;
		}
		return m_NameValueMap[index].name.c_str();
	}

private:
	struct SQLite_Name_Value
	{
		string name;
		string value;
	};

	void				Add(const char* name,const char* value)
	{
		SQLite_Name_Value nv;
		nv.name = name;
		nv.value = value;
		m_NameValueMap[m_nCurrentFieldIndex++] = nv;
	}
	unsigned long						m_nFieldCount;							// �е�����
	unsigned long						m_nCurrentFieldIndex;				// ��ǰ�е�����
	SQLite_Name_Value				m_NameValueMap[1024];		// ��Ž��
};

// SQLִ����
class SQLiteExcer
{
public:
	SQLiteExcer(SQLiteConnector& db):m_DB(db)
	{
		m_pCallBack = &SQLiteExcer::Help;
		m_nRowIndex = 0;
		m_nRowCount = 0;
	}

	bool					ExecSQL(const char* pszSQL)					// ִ��SQL���
	{
		if (0 == pszSQL || m_DB.GetHandler() == 0)
		{
			return false;
		}

		char* errMSG;

		int nRet = sqlite3_exec(m_DB.GetHandler(),pszSQL,m_pCallBack,(void*)this,&errMSG);

		if (nRet != SQLITE_OK)
		{
			m_DB.ErrorMsg(errMSG);

			return false;
		}

		return true;
	}

	vector<SQLiteRow>			GetResult()								// ��ȡ������Լ�����
	{
		return m_ResultList;
	}

	bool									Next(SQLiteRow& row)				// ���н����
	{
		if (m_nRowIndex >= m_ResultList.size())
		{
			return false;
		}

		row = m_ResultList[m_nRowIndex++];

		return true;
	}

	void									MoveToFist()								// �ƶ���������ĵ�һ��
	{
		m_nRowIndex = 0;
	}

	int									GetRowCount()
	{
		return m_nRowCount;
	}

private:

	static int Help(void *pOther, int argc, char **argv, char **szColName)			// һ���������������ڴ�������
	{  
		if (0 == pOther)
		{
			return -1;
		}

		SQLiteExcer* pExecer = (SQLiteExcer*)pOther;

		pExecer->m_nRowCount++;

		SQLiteRow row;

		row.m_nFieldCount =argc;

		for(int i = 0 ; i < argc ; i++)  
		{  
			row.Add(szColName[i],(argv[i] ? argv[i] : "NULL"));
		}  

		pExecer->m_ResultList.push_back(row);

		return 0;  
	}  

private:
	typedef int(*sqlite3_callback)(void*,int,char**,char**);

	SQLiteConnector&		m_DB;						// ���ݿ�������
	sqlite3_callback				m_pCallBack;			// �ص�����
	int								m_nRowCount;			// �����������
	vector<SQLiteRow>		m_ResultList;				// �����
	int								m_nRowIndex;			// ��ǰ�е�����
};

// Demo һ��ʹ�õ�����
/*

SQLiteConnector db;						// ����һ����������
if (!db.Connect("FightingDB.db"))		// ���ӵ����ݿ�
{
	return -1;
}

SQLiteExcer selectExec(db);				// ����һ��SQLִ�����ִ࣬�о����SQL���

selectExec.ExecSQL("select * from User;");

SQLiteRow row;						
while (selectExec.Next(row))				// ���������
{
	for (int index = 0; index < row.GetFieldCount(); ++index)
	{
		cout << row.GetValue(index)<< " ";
	}
	cout << endl;
}

string strSQL = "insert into User(userID,password) values('000002','000002');";

SQLiteExcer insertExec(db);				// ��������

if (!insertExec.ExecSQL(strSQL.c_str()))
{
	return -1;
}
*/
#endif // !_SQLITE_DB_H_
