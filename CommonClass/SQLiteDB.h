#ifndef _SQLITE_DB_H_
#define _SQLITE_DB_H_

#include "./sqlite-src/sqlite3.h"
#include <string>
#include <vector>
using namespace std;

/*
** SQLite数据库类
*/

// 数据库连接类
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

	bool					Connect(const char* pszDBFileName)			// 连接到数据库
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

	void					Close()															// 关闭数据库
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

	sqlite3*				GetHandler()													// 获取连接对象
	{
		return m_pDB;
	}

	bool					IsConnect()													// 是否连接成功
	{
		return m_bIsConnect;
	}

private:
	bool							m_bIsConnect;
	sqlite3*						m_pDB;
};

// 结果（行）类
class SQLiteRow
{
public:
	friend class SQLiteExcer;
	SQLiteRow():m_nFieldCount(0),m_nCurrentFieldIndex(0){}

	int                 GetFieldCount()							// 获取列的数量
	{
		return m_nFieldCount;
	}

	const char*             GetValue(int index)			// 根据列索引获取对应值
	{
		if (index >= m_nCurrentFieldIndex)
		{
			return 0;
		}
		return m_NameValueMap[index].value.c_str();
	}

	const char*				GetFileldName(int index)	// 根据索引获取列名
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
	unsigned long						m_nFieldCount;							// 列的数量
	unsigned long						m_nCurrentFieldIndex;				// 当前列的索引
	SQLite_Name_Value				m_NameValueMap[1024];		// 存放结果
};

// SQL执行类
class SQLiteExcer
{
public:
	SQLiteExcer(SQLiteConnector& db):m_DB(db)
	{
		m_pCallBack = &SQLiteExcer::Help;
		m_nRowIndex = 0;
		m_nRowCount = 0;
	}

	bool					ExecSQL(const char* pszSQL)					// 执行SQL语句
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

	vector<SQLiteRow>			GetResult()								// 获取结果集自己处理
	{
		return m_ResultList;
	}

	bool									Next(SQLiteRow& row)				// 编列结果集
	{
		if (m_nRowIndex >= m_ResultList.size())
		{
			return false;
		}

		row = m_ResultList[m_nRowIndex++];

		return true;
	}

	void									MoveToFist()								// 移动到结果集的第一行
	{
		m_nRowIndex = 0;
	}

	int									GetRowCount()
	{
		return m_nRowCount;
	}

private:

	static int Help(void *pOther, int argc, char **argv, char **szColName)			// 一个帮助函数，用于处理结果集
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

	SQLiteConnector&		m_DB;						// 数据库连接器
	sqlite3_callback				m_pCallBack;			// 回调函数
	int								m_nRowCount;			// 结果集的行数
	vector<SQLiteRow>		m_ResultList;				// 结果集
	int								m_nRowIndex;			// 当前行的索引
};

// Demo 一个使用的例子
/*

SQLiteConnector db;						// 定义一个连接器类
if (!db.Connect("FightingDB.db"))		// 连接到数据库
{
	return -1;
}

SQLiteExcer selectExec(db);				// 定义一个SQL执行器类，执行具体的SQL语句

selectExec.ExecSQL("select * from User;");

SQLiteRow row;						
while (selectExec.Next(row))				// 遍历结果集
{
	for (int index = 0; index < row.GetFieldCount(); ++index)
	{
		cout << row.GetValue(index)<< " ";
	}
	cout << endl;
}

string strSQL = "insert into User(userID,password) values('000002','000002');";

SQLiteExcer insertExec(db);				// 插入数据

if (!insertExec.ExecSQL(strSQL.c_str()))
{
	return -1;
}
*/
#endif // !_SQLITE_DB_H_
