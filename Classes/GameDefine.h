#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include <string>

#define _USE_PC_																					// �Ƿ�ʹ������PC�ĺ��־������ú������ʹ�ü��̺�����������
#define _TEST_																						// ���Ժ�

#define CONFIG_FILE					("config.json")									//�����ļ���
#define BACKGROUND_PATH		("LFR/other/background.png")			// ����ͼ·��

// Ӣ������ -begin
#define BANDIT_NAME							("bandit")
#define BAT_NAME									("bat")
#define DAVIS_NAME								("davis")
#define DEEP_NAME								("deep")
#define DENNIS_NAME							("dennis")
#define FIREN_NAME								("firen")
#define FIRZEN_NAME								("firzen")
#define FREEZE_NAME								("freeze")
#define HENRY_NAME								("henry")
#define HUNTER_NAME							("hunter")
#define JACK_NAME									("jack")
#define JAN_NAME									("jan")
#define JOHN_NAME								("john")
#define JULIAN_NAME								("julian")
#define JUSTIN_NAME								("justin")
#define KNIGHT_NAME							("knight")
#define LOUIS_NAME								("louis")
#define LOUISEX_NAME							("louisEX")
#define MARK_NAME								("mark")
#define MONK_NAME								("monk")
#define RUDOLF_NAME							("rudolf")
#define SORCERER_NAME						("sorcerer")
#define WOODY_NAME							("woody")
// Ӣ������ -end


/************************************************************************
*   ����                                                                  
************************************************************************/
typedef enum
{
	directon_invalid = -1,
	directon_stay,					// ����
	directon_right,					// ����
	directon_up,						// ����
	directon_left,					// ����
	directon_down,					// ����
}tagDirecton;

/************************************************************************
*   Ӣ������ID                                                                   
************************************************************************/
enum HERO_ID
{
	INVALID_ID = -1,
	BANDIT_ID,		
	BAT_ID	,				
	DAVIS_ID	,							
	DEEP_ID	,							
	DENNIS_ID,					
	FIREN_ID	,				
	FIRZEN_ID,						
	FREEZE_ID,		
	HENRY_ID,	
	HUNTER_ID,	
	JACK_ID,	
	JAN_ID	,	
	JOHN_ID	,	
	JULIAN_ID,	
	JUSTIN_ID,			
	KNIGHT_ID	,		
	LOUIS_ID	,		
	LOUISEX_ID	,			
	MARK_ID	,				
	MONK_ID	,			
	RUDOLF_ID	,				
	SORCERER_ID	,				
	WOODY_ID							
};




/************************************************************************
*   ����ID                                                                   
************************************************************************/
enum SCENE_ID
{
	INVALID_SCENE = -1,			// ��Ч����
	START_SCENE,						// ��ʼ����
	CHOOSEGAME_SCENE,		// ��Ϸѡ�񳡾�
	MAIN_SCENE,						// ������
	RESULT_SCENE,					// ��Ϸ�������
	CREAT_GAME_SCENE,			// ������Ϸ����
	CREATEORJOIN_SCENE,		// ѡ�񴴽����߼�����Ϸ�ĳ���
	SHOW_HERO_SCENE,			// ������ѡ��Ӣ�۵ĳ���
};



/************************************************************************
*   Ӣ��״̬                                                                  
************************************************************************/
enum HERO_STATE
{
	INVALID_STATE,
	STATIC_STATE,								// ��ֹ����
	MOVE_STATE,								// �ƶ�
	ATTACK_STATE,							// ������
	MAGICATTACK_STATE,					// ħ������
	DEATH_STATE								// ����
};

/************************************************************************
*   ��Ϸ����                                                                  
************************************************************************/
enum GAME_TYPE
{
	INVALID_GAME_TYPE = -1,	// ��Ч����Ϸ
	SINGLE_GAME_TYPE,			// ������Ϸ
	CREATE_GAME_TYPE,			// ��������Ϸ
	JOIN_GAME_TYPE				// �������Ϸ
};

// Ӣ�۵���Ϊ����
enum HERO_ACTION
{
	STATIC_ACTION,				// ��ֹ
	MOVE_ACTION,				// �ƶ�
	ATTACK_ACTION,				// ����
	MAGICATTACK_ACTION, // ħ������
};

/************************************************************************
*   AI������                                                                  
************************************************************************/
struct AI_Probability
{
	float														static_probability;			// Ӣ�۾�ֹ�����ĸ���
	float														move_probability;			// Ӣ���߶��ĸ���
	float														attack_probability;		// Ӣ�۹����ĸ���
	float														magic_probability;			// Ӣ�۷Ŵ��еĸ���
};
     
/************************************************************************
*   ȫ�ֱ���                                                                   
************************************************************************/

class GlobalValue
{
public:
	static std::string									server_ip;						// ������IP
	static int												server_port;					// �������˿�
	static int												hero_move_step;			// ������ÿ��һ���ĳ���
	static int												magic_distance;			// ħ������Ч����
	static float											parry_probability;			// �񵲵ĸ���
	static AI_Probability								nomal_ai_probability;	// AI���ʵ�����
	static AI_Probability								attack_ai_probability;	// 

	static bool											use_ai;							// �Ƿ�ʹ��AI�ı�־
	static int												ai_check_distance;		// aiӢ�۵��Ӿ�
	static int												ai_attack_distance;		// aiӢ�۵Ĺ�����Χ
	static int												magic_move_step;		// ħ���ƶ��Ĳ���
	static int												attack_damage;			// �����˺�ֵ
	static int												magic_damage;				// ħ���˺�ֵ
	static int												magic_damage_distance;	// ħ���˺���Χ���ڴ˷�Χ�ڣ����ֲ����յ��˺�
	static int												attack_damage_distance;	// �����˺���Χ���ڴ˷�Χ�ڣ����ֲ����ܵ��˺�
	static int												magic_cost;						// ��һ��ħ����ħ����������
	static int												blood_regain_value;			// ��Ѫ��
	static int												magic_regain_value;			// ��ħ��

#ifdef _TEST_
	static std::string									current_hero_name;
	static std::string									current_enemy_name;
#endif
};


/************************************************************************
*  �������ص��Ķ���
*  pictureName�������ļ���
*  rowIndex������Ҫ�Ķ������ļ��ĵڼ���
*  colBeginIndex�� ��ʼ����λ��
*	colEndIndex����������λ��
*  width���������
*  height�������߶�
*  time_interval���������ŵ�ʱ����
*  repeate_flag���Ƿ��ظ�����
*	repeate_count���ظ��Ĵ���
*	call_back���ص�����
************************************************************************/
#ifndef DEFINE_ANIMATION_WITH_CALLBACK
#define DEFINE_ANIMATION_WITH_CALLBACK(pictureName,rowIndex,colBeginIndex,colEndIndex,increaseFlag,width,height,time_interval,repeate_flag,repeate_count,call_back)\
	cocos2d::Sequence *sequence = NULL;\
	cocos2d::Spawn *spawn = NULL;\
	Animation* animation = NULL;\
	Texture2D* pTexture = NULL;\
	Vector<SpriteFrame*> vec;\
	pTexture = TextureCache::getInstance()->addImage((pictureName));\
	if((increaseFlag))\
	{\
		for(int i = (colBeginIndex);i < (colEndIndex); ++i)\
		{\
		SpriteFrame* sf = SpriteFrame::createWithTexture(pTexture,Rect(i * (width),rowIndex * (height),(width),(height)));\
		vec.pushBack(sf);\
		}\
	}\
	else\
	{\
		for(int i = (colBeginIndex); i > (colEndIndex); --i)\
		{\
			SpriteFrame* sf = SpriteFrame::createWithTexture(pTexture,Rect(i * (width),rowIndex * (height),(width),(height)));\
			vec.pushBack(sf);\
		}\
	}\
	animation = Animation::createWithSpriteFrames(vec,(time_interval));\
	auto animate = Animate::create(animation);\
	if((repeate_flag))\
	{\
		Repeat* repeate = Repeat::create(animate,(repeate_count));\
		m_pSequence = Sequence::create(animate,repeate,CallFunc::create(CC_CALLBACK_0(call_back,this)),NULL);\
		\
		m_pSprite->runAction(m_pSequence);\
	}\
	else\
	{\
		m_pSequence = Sequence::create(animate,CallFunc::create(CC_CALLBACK_0(call_back,this)),NULL);\
			\
		m_pSprite->runAction(m_pSequence);\
	}

#endif // !DEFINE_ANIMATION_WITH_CALLBACK



/************************************************************************
*  ���������ص��Ķ���
*  pictureName�������ļ���
*  rowIndex������Ҫ�Ķ������ļ��ĵڼ���
*  colBeginIndex�� ��ʼ����λ��
*	colEndIndex����������λ��
*  width���������
*  height�������߶�
*  time_interval���������ŵ�ʱ����
*  repeate_flag���Ƿ��ظ�����
*	repeate_count���ظ��Ĵ���
************************************************************************/
#ifndef DEFINE_ANIMATION_WITHOUT_CALLBACK
#define DEFINE_ANIMATION_WITHOUT_CALLBACK(pictureName,rowIndex,colBeginIndex,colEndIndex,increaseFlag,width,height,time_interval,repeate_flag,repeate_count)\
	cocos2d::Sequence *sequence = NULL;\
	cocos2d::Spawn *spawn = NULL;\
	Animation* animation = NULL;\
	Texture2D* pTexture = NULL;\
	Vector<SpriteFrame*> vec;\
	pTexture = TextureCache::getInstance()->addImage((pictureName));\
	if((increaseFlag))\
	{\
		for(int i = (colBeginIndex);i < (colEndIndex); ++i)\
		{\
			SpriteFrame* sf = SpriteFrame::createWithTexture(pTexture,Rect(i * (width),rowIndex * (height),(width),(height)));\
			vec.pushBack(sf);\
		}\
	}\
	else\
	{\
		for(int i = (colBeginIndex); i > (colEndIndex); --i)\
		{\
			SpriteFrame* sf = SpriteFrame::createWithTexture(pTexture,Rect(i * (width),rowIndex * (height),(width),(height)));\
			vec.pushBack(sf);\
		}\
	}\
	animation = Animation::createWithSpriteFrames(vec,(time_interval));\
	auto animate = Animate::create(animation);\
	if((repeate_flag))\
	{\
		Repeat* repeate = Repeat::create(animate,(repeate_count));\
		m_pSequence = Sequence::create(animate,repeate,NULL);\
		m_pSprite->runAction(m_pSequence);\
	}\
	else\
	{\
		m_pSequence = Sequence::create(animate,NULL);\
		m_pSprite->runAction(m_pSequence);\
	}
#endif// DEFINE_ANIMATION_WITHOUT_CALLBACK

/************************************************************************
*  ����ħ������
*  seq������ħ������������
*  pictureName�������ļ���
*	directon��ħ���ĳ���
*	len��ħ������Ч����
*  rowBeginIndex������Ҫ�Ķ������ļ��ĵڼ��У���ʼ��
*	colEndIndex������Ҫ�Ķ������ļ��ĵڼ��У�������
*  colBeginIndex�� ��ʼ����λ��
*	colEndIndex����������λ��
*  width���������
*  height�������߶�
*  time_interval���������ŵ�ʱ����
************************************************************************/
#ifndef DEFINE_MOVE_MAGIC
#define DEFINE_MOVE_MAGIC(seq,pictureName,directon,len,rowBeginIndex,rowEndIndex,colBeginIndex,colEndIndex,increaseFlage,width,height,time_interval)\
	cocos2d::Sequence *sequence = NULL;\
	cocos2d::Spawn *spawn = NULL;\
	Animation* animation = NULL;\
	Texture2D* pTexture = NULL;\
	Vector<SpriteFrame*> vec;\
	pTexture = TextureCache::getInstance()->addImage((pictureName));\
	if((increaseFlage))\
	{\
		for (int y = (rowBeginIndex); y < (rowEndIndex); ++y)\
		{\
			for (int x = (colBeginIndex); x < (colEndIndex); ++x)\
			{\
				SpriteFrame* sf = SpriteFrame::createWithTexture(pTexture,CCRectMake(x * (width) ,y * (height),( width),(height)));\
				vec.pushBack(sf);\
			}\
		}\
	}\
	else\
	{\
		for (int y = (rowBeginIndex); y < (rowEndIndex); ++y)\
		{\
			for (int x = (colBeginIndex); x > (colEndIndex); --x)\
			{\
				SpriteFrame* sf = SpriteFrame::createWithTexture(pTexture,CCRectMake(x * (width) ,y * (height), (width),(height)));\
				vec.pushBack(sf);\
			}\
		}\
	}\
	animation = Animation::createWithSpriteFrames(vec,(time_interval));\
	auto animate = Animate::create(animation);\
	int step = len / 100;\
	MoveBy* by = 0;\
	if (directon == directon_left)\
	{\
		by = MoveBy::create(step,Point(-len,0));\
	}\
	else if(directon == directon_right)\
	{\
		by = MoveBy::create(step,Point(len,0));\
	}\
	Repeat* repeate = Repeat::create(animate,/*(repeate_count)*/step*1.5);\
	spawn = Spawn::create(by,repeate,NULL);\
	seq = Sequence::create(spawn,NULL);

#endif



#endif