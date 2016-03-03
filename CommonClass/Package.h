#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_


#include "common.h"

#include "NetAchive.h"
#include "Util.h"
#include "CodeMsg.h"

// 包头部的长度
#ifndef PACKAGE_HEAD_LEN
#define PACKAGE_HEAD_LEN (SZ_8_LENGTH*8)
#endif

#ifndef SPILIT_CHAR
#define SPILIT_CHAR ('|')
#endif


// 包的类型
enum PackageType
{
    package_unknown = -1,		// 未知类型
    package_control,					// 控制包
    package_data						// 数据包
};

// 包头部
class PackageHead
{
public:
    PackageHead()
    {
        Clean();
    }

    void        Clean()
    {
        memset(m_szVersion,0,SZ_8_LENGTH);
        memset(m_szRequestCode,0,SZ_8_LENGTH);
        memset(m_szReturnCode,0,SZ_8_LENGTH);
        memset(m_szPackageLength,0,SZ_8_LENGTH);
        memset(m_szPackageType,0,SZ_8_LENGTH);

        memset(m_szReserved1,0,SZ_8_LENGTH);
        memset(m_szReserved2,0,SZ_8_LENGTH);
        memset(m_szReserved3,0,SZ_8_LENGTH);

		SetVersion("1.0");
		SetPackageLength(0);
		SetPackageType(package_control);
    }


    void        SetVersion(const char* v)
    {
        if(0 == v)
            return;

        strncpy(m_szVersion,v,SZ_8_LENGTH);

    }

    const char* GetVersion()
    {
        return m_szVersion;
    }

    void        SetPackageLength(unsigned int len)
    {
        if(len < 0)
            return;

        sprintf(m_szPackageLength,"%d",len);
    }

    unsigned int    GetPackageLength()
    {
        int len = atoi(m_szPackageLength);
        return len;
    }

    void            SetPackageType(PackageType type)
    {
        switch (type)
        {
        case package_control:
            strcpy(m_szPackageType,"0");
            break;
        case package_data:
            strcpy(m_szPackageType,"1");
            break;
        }
    }

    PackageType GetPackageType()
    {
        if(0 == strcmp(m_szPackageType,"0"))
            return package_control;

        if(0 == strcmp(m_szPackageType,"1"))
            return package_data;

        return package_unknown;
    }


    int             GetHeadBuffer(char* buf,int buf_len)
    {
        if(buf_len < PACKAGE_HEAD_LEN)
            return -1;

        memcpy(buf+SZ_8_LENGTH * 0,m_szVersion,SZ_8_LENGTH);
        memcpy(buf+SZ_8_LENGTH * 1,m_szRequestCode,SZ_8_LENGTH);
        memcpy(buf+SZ_8_LENGTH * 2,m_szReturnCode,SZ_8_LENGTH);
        memcpy(buf+SZ_8_LENGTH * 3,m_szPackageType,SZ_8_LENGTH);
        memcpy(buf+SZ_8_LENGTH * 4,m_szPackageLength,SZ_8_LENGTH);

        memcpy(buf+SZ_8_LENGTH * 5,m_szReserved1,SZ_8_LENGTH);
        memcpy(buf+SZ_8_LENGTH * 6,m_szReserved1,SZ_8_LENGTH);
        memcpy(buf+SZ_8_LENGTH * 7,m_szReserved1,SZ_8_LENGTH);

        return PACKAGE_HEAD_LEN;
    }

    void			Analyze(char* buf,int len)
    {

        if(len < PACKAGE_HEAD_LEN)
            return;

        memcpy(m_szVersion,         buf + SZ_8_LENGTH * 0,SZ_8_LENGTH);
        memcpy(m_szRequestCode,     buf + SZ_8_LENGTH * 1,SZ_8_LENGTH);
        memcpy(m_szReturnCode,      buf + SZ_8_LENGTH * 2,SZ_8_LENGTH);
        memcpy(m_szPackageType,     buf + SZ_8_LENGTH * 3,SZ_8_LENGTH);
        memcpy(m_szPackageLength,   buf + SZ_8_LENGTH * 4,SZ_8_LENGTH);

        memcpy(m_szReserved1,       buf + SZ_8_LENGTH * 5,SZ_8_LENGTH);
        memcpy(m_szReserved2,       buf + SZ_8_LENGTH * 6,SZ_8_LENGTH);
        memcpy(m_szReserved3,       buf + SZ_8_LENGTH * 7,SZ_8_LENGTH);
    }

public:
    char                m_szVersion[SZ_8_LENGTH];					// 版本
    char                m_szRequestCode[SZ_8_LENGTH];			// 请求码
    char                m_szReturnCode[SZ_8_LENGTH];			// 回应码
    char                m_szPackageType[SZ_8_LENGTH];			// 包类型
    char                m_szPackageLength[SZ_8_LENGTH];		// 包数据部分的长度
    char                m_szReserved1[SZ_8_LENGTH];				// 保留字
    char                m_szReserved2[SZ_8_LENGTH];				// 保留字
    char                m_szReserved3[SZ_8_LENGTH];				// 保留字
};

// 控制包
class ControlPackage
{
public:
	ControlPackage()
	{
		m_strUserID = "";
		m_strUserPassword = "";
		m_strUserName = "";
		m_nUserHeroID = -1;
		m_bUserLogin = false;

		m_nPackageCode = code_unknown;
		m_PackageHead.SetPackageType(package_control);

		m_nGameRoundIndex = -1;
		m_nGameRoundWaitCount = 0;
		m_strOtherInfo = "";
		m_nOtherInfoLen = 0;

		m_strGameName = "";
		m_nGameState = -1;//game_state_unknown
		m_bIsCreator = false;//invalid_player
	}

	PROPERTY_INIT(string,UserID,m_strUserID);
	PROPERTY_INIT(string,UserPassword,m_strUserPassword);
	PROPERTY_INIT(string,UserName,m_strUserName);
	PROPERTY_INIT(int,UserHeroID,m_nUserHeroID);
	PROPERTY_INIT(bool,UserLogin,m_bUserLogin);
	PROPERTY_INIT(bool,IsCreator,m_bIsCreator);


	PROPERTY_INIT(string,GameName,m_strGameName);
	PROPERTY_INIT(int,GameState,m_nGameState);

	PROPERTY_INIT(PackageCode,PackageCode,m_nPackageCode);
	PROPERTY_INIT(int,GameRoundIndex,m_nGameRoundIndex);
	PROPERTY_INIT(int,GameRoundWaitCount,m_nGameRoundWaitCount);
	PROPERTY_INIT(string,OtherInfo,m_strOtherInfo);//m_nOtherInfoLen
	PROPERTY_INIT(int,OtherInfoLen,m_nOtherInfoLen);

	int				GetPackageBuffer(char* retBuffer)
	{

		char buf[1024 * 8] = {0};
		NetAchive ar(buf,1024*8,NetAchive::SAVE_MODE);

		ar.Save_int((int)m_nPackageCode);

		ar.Save_string(m_strUserID);
		ar.Save_string(m_strUserPassword);
		ar.Save_string(m_strUserName);
		ar.Save_int(m_nUserHeroID);
		ar.Save_bool(m_bUserLogin);
		ar.Save_bool(m_bIsCreator);

	
		ar.Save_int(m_nGameRoundIndex);
		ar.Save_int(m_nGameRoundWaitCount);
		ar.Save_string(m_strGameName);
		ar.Save_int(m_nGameState);

		m_nOtherInfoLen = m_strOtherInfo.size();
		ar.Save_int(m_nOtherInfoLen);
		ar.Save_string(m_strOtherInfo);

		m_strPackageBuffer = buf;

		int buffer_len = m_strPackageBuffer.size();

		m_PackageHead.SetPackageLength(buffer_len);

		char pszHead[SZ_1024_LENGTH] = {0};
		int len = m_PackageHead.GetHeadBuffer(pszHead,PACKAGE_HEAD_LEN);

		memcpy(retBuffer,pszHead,PACKAGE_HEAD_LEN);
		memcpy(retBuffer + PACKAGE_HEAD_LEN,m_strPackageBuffer.c_str(),m_strPackageBuffer.size());

		return (PACKAGE_HEAD_LEN + m_strPackageBuffer.size());
	}

	void				Analyze(const char* str,int len)
	{
		if (str == 0 || len <= 0)
		{
			return;
		}

		NetAchive ar((char*)str,len,NetAchive::LOAD_MODE);

		m_nPackageCode = (PackageCode)ar.Load_int();

		m_strUserID = ar.Load_string();
		m_strUserPassword = ar.Load_string();
		m_strUserName = ar.Load_string();
		m_nUserHeroID = ar.Load_int();
		m_bUserLogin = ar.Load_bool();
		m_bIsCreator = ar.Load_bool();


		m_nGameRoundIndex = ar.Load_int();
		m_nGameRoundWaitCount = ar.Load_int();
		m_strGameName = ar.Load_string();
		m_nGameState = ar.Load_int();

		m_nOtherInfoLen = ar.Load_int();
		m_strOtherInfo = ar.Load_string(m_nOtherInfoLen);

		m_strPackageBuffer = str;
	}

	void					Print()
	{
		cout << "ControlPackage"  << endl;
		cout << "{"  << endl;


		cout <<"\t" <<"PackageCode = " <<m_nPackageCode << endl;

		cout <<"\t" <<"UserID = " <<m_strUserID << endl;
		cout <<"\t" <<"Password = " <<m_strUserPassword << endl;
		cout <<"\t" <<"UserName = " <<m_strUserName << endl;
		cout <<"\t" <<"HeroID = " <<m_nUserHeroID << endl;
		cout <<"\t" <<"Login = " <<m_bUserLogin << endl;
		cout <<"\t" <<"IsCreator = " <<m_bIsCreator << endl;

		cout << "}" << endl;
	}

public:
	PackageHead        m_PackageHead;							// 包头部

private:
	PackageCode	m_nPackageCode;							// 控制码

	// 玩家信息 begin
	string				m_strUserID;										// 用户ID
	string				m_strUserPassword;							// 密码
	string				m_strUserName;								// 用户名字
	int					m_nUserHeroID;								// 选择的英雄ID
	bool					m_bUserLogin;									// 是否已经登陆			
	bool					m_bIsCreator;									// 玩家类型
	// 玩家信息 end


	string				m_strGameName;								// 游戏的名字
	int					m_nGameRoundIndex;						// 游戏的索引
	int					m_nGameState;								// 游戏的状态

	int					m_nGameRoundWaitCount;				// 处于等待状态的游戏的数量

	int					m_nOtherInfoLen;							//	其他信息的长度
	string				m_strOtherInfo;								// 其他信息

	string				m_strPackageBuffer;
};


// 数据包
class DataPackage
{
public:
    DataPackage(void)
    {
        Clean();
		m_PackageHead.SetPackageType(package_data);
    }

    void					Clean()
    {
        m_nHeroID = -1;
        m_nHeroState = -1;
        m_nFaceTo = -1;
        m_nPositionX = 0;
        m_nPositionY = 0;
        m_nBloodValue = 0;
        m_nMagicValue = 0;
		m_nMoveDirection = -1;
        m_strPackageBuffer= "";
    }

    virtual ~DataPackage(void)
    {
        Clean();
    }

	PROPERTY_INIT(int,MoveDirection,m_nMoveDirection);

    void				SetHeroID(int id)
    {
        m_nHeroID = id;
    }

    int					GetHeroID()
    {
        return m_nHeroID;
    }

    void				SetBlood(float v)
    {
        m_nBloodValue = v;
    }

    float					GetBlood()
    {
        return m_nBloodValue;
    }

    void				SetMagic(float v)
    {
        m_nBloodValue = v;
    }

    float					GetMagic()
    {
        return m_nBloodValue;
    }

    void				SetPosition(float x,float y)
    {
        m_nPositionX = x;
        m_nPositionY = y;
    }

    float					GetPositionX()
    {
        return m_nPositionX;
    }

    float					GetPositionY()
    {
        return m_nPositionY;
    }

    void				SetState(int state)
    {
        m_nHeroState = state;
    }

    int					GetState()
    {
        return m_nHeroState;
    }

    void				SetFaceTo(int faceTo)
    {
        m_nFaceTo = faceTo;
    }

    int					GetFaceTo()
    {
        return m_nFaceTo;
    }


    int				GetPackageBuffer(char* retBuffer)
    {
		char buf[1024 * 8] = {0};
        NetAchive ar(buf,1024*8,NetAchive::SAVE_MODE);
        ar.Save_int(m_nHeroID);
        ar.Save_int(m_nHeroState);
        ar.Save_int(m_nFaceTo);
		ar.Save_int(m_nMoveDirection);
        ar.Save_float(m_nPositionX);
        ar.Save_float(m_nPositionY);
        ar.Save_float(m_nBloodValue);
        ar.Save_float(m_nMagicValue);

        m_strPackageBuffer = buf;

        int buffer_len = m_strPackageBuffer.size();

        m_PackageHead.SetPackageLength(buffer_len);

        char pszHead[SZ_1024_LENGTH] = {0};
        int len = m_PackageHead.GetHeadBuffer(pszHead,PACKAGE_HEAD_LEN);

        memcpy(retBuffer,pszHead,PACKAGE_HEAD_LEN);
        memcpy(retBuffer + PACKAGE_HEAD_LEN,m_strPackageBuffer.c_str(),m_strPackageBuffer.size());

        return (PACKAGE_HEAD_LEN + m_strPackageBuffer.size());
    }



    void				Analyze(const char* str,int len)
    {
        assert(str != 0);
        assert(len > 0);

        NetAchive ar((char*)str,len,NetAchive::LOAD_MODE);

        m_nHeroID = ar.Load_int();
        m_nHeroState = ar.Load_int();
        m_nFaceTo = ar.Load_int();
		m_nMoveDirection = ar.Load_int();
        m_nPositionX = ar.Load_float();
        m_nPositionY = ar.Load_float();
        m_nBloodValue = ar.Load_float();
        m_nMagicValue = ar.Load_float();

        m_strPackageBuffer = str;
    }

    int				GetPackageBufferLength()
    {
        return m_PackageHead.GetPackageLength();
    }

    void				Print()
    {
        cout << "ID:" << m_nHeroID << endl;
        cout << "State:" << m_nHeroState << endl;
        cout << "FaceTo:" << m_nFaceTo << endl;
        cout << "PositionX:" << m_nPositionX << endl;
        cout << "PositionY:" << m_nPositionY << endl;
        cout << "Blood:" << m_nBloodValue << endl;
        cout << "Magic:" << m_nMagicValue << endl;
    }

public:
    PackageHead        m_PackageHead;

private:
    int					m_nHeroID;					// 英雄ID
    int					m_nHeroState;				// 英雄状态
    int					m_nFaceTo;					// 英雄的脸部朝向
	int					m_nMoveDirection;		// 英雄移动的方向
    float					m_nPositionX;				// 英雄的位置X
    float					m_nPositionY;				// 英雄的位置Y
    float					m_nBloodValue;			// 英雄的血量
    float					m_nMagicValue;			// 英雄的魔法量

    string				m_strPackageBuffer;
};

#endif