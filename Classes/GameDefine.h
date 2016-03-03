#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include <string>

#define _USE_PC_																					// 是否使用主机PC的宏标志，定义该宏则可以使用键盘和鼠标控制人物
#define _TEST_																						// 测试宏

#define CONFIG_FILE					("config.json")									//配置文件名
#define BACKGROUND_PATH		("LFR/other/background.png")			// 背景图路径

// 英雄名字 -begin
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
// 英雄名字 -end


/************************************************************************
*   方向                                                                  
************************************************************************/
typedef enum
{
	directon_invalid = -1,
	directon_stay,					// 不动
	directon_right,					// 向右
	directon_up,						// 向上
	directon_left,					// 向左
	directon_down,					// 向下
}tagDirecton;

/************************************************************************
*   英雄人物ID                                                                   
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
*   场景ID                                                                   
************************************************************************/
enum SCENE_ID
{
	INVALID_SCENE = -1,			// 无效场景
	START_SCENE,						// 开始场景
	CHOOSEGAME_SCENE,		// 游戏选择场景
	MAIN_SCENE,						// 主场景
	RESULT_SCENE,					// 游戏结果场景
	CREAT_GAME_SCENE,			// 创建游戏场景
	CREATEORJOIN_SCENE,		// 选择创建或者加入游戏的场景
	SHOW_HERO_SCENE,			// 单机下选择英雄的场景
};



/************************************************************************
*   英雄状态                                                                  
************************************************************************/
enum HERO_STATE
{
	INVALID_STATE,
	STATIC_STATE,								// 静止不动
	MOVE_STATE,								// 移动
	ATTACK_STATE,							// 物理攻击
	MAGICATTACK_STATE,					// 魔法攻击
	DEATH_STATE								// 死亡
};

/************************************************************************
*   游戏类型                                                                  
************************************************************************/
enum GAME_TYPE
{
	INVALID_GAME_TYPE = -1,	// 无效的游戏
	SINGLE_GAME_TYPE,			// 单机游戏
	CREATE_GAME_TYPE,			// 创建的游戏
	JOIN_GAME_TYPE				// 加入的游戏
};

// 英雄的行为类型
enum HERO_ACTION
{
	STATIC_ACTION,				// 静止
	MOVE_ACTION,				// 移动
	ATTACK_ACTION,				// 攻击
	MAGICATTACK_ACTION, // 魔法攻击
};

/************************************************************************
*   AI的设置                                                                  
************************************************************************/
struct AI_Probability
{
	float														static_probability;			// 英雄静止不动的概率
	float														move_probability;			// 英雄走动的概率
	float														attack_probability;		// 英雄攻击的概率
	float														magic_probability;			// 英雄放大招的概率
};
     
/************************************************************************
*   全局变量                                                                   
************************************************************************/

class GlobalValue
{
public:
	static std::string									server_ip;						// 服务器IP
	static int												server_port;					// 服务器端口
	static int												hero_move_step;			// 人物走每走一步的长度
	static int												magic_distance;			// 魔法的有效距离
	static float											parry_probability;			// 格挡的概率
	static AI_Probability								nomal_ai_probability;	// AI概率的设置
	static AI_Probability								attack_ai_probability;	// 

	static bool											use_ai;							// 是否使用AI的标志
	static int												ai_check_distance;		// ai英雄的视距
	static int												ai_attack_distance;		// ai英雄的攻击范围
	static int												magic_move_step;		// 魔法移动的步长
	static int												attack_damage;			// 物理伤害值
	static int												magic_damage;				// 魔法伤害值
	static int												magic_damage_distance;	// 魔法伤害范围，在此范围内，对手才能收到伤害
	static int												attack_damage_distance;	// 物理伤害范围，在此范围内，对手才能受到伤害
	static int												magic_cost;						// 放一次魔法，魔法的消耗量
	static int												blood_regain_value;			// 回血量
	static int												magic_regain_value;			// 回魔量

#ifdef _TEST_
	static std::string									current_hero_name;
	static std::string									current_enemy_name;
#endif
};


/************************************************************************
*  创建带回调的动画
*  pictureName：动画文件名
*  rowIndex：所需要的动画在文件的第几行
*  colBeginIndex： 开始的列位置
*	colEndIndex：结束的列位置
*  width：动画宽度
*  height：动画高度
*  time_interval：动画播放的时间间隔
*  repeate_flag：是否重复播放
*	repeate_count：重复的次数
*	call_back：回调函数
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
*  创建不带回调的动画
*  pictureName：动画文件名
*  rowIndex：所需要的动画在文件的第几行
*  colBeginIndex： 开始的列位置
*	colEndIndex：结束的列位置
*  width：动画宽度
*  height：动画高度
*  time_interval：动画播放的时间间隔
*  repeate_flag：是否重复播放
*	repeate_count：重复的次数
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
*  创建魔法动画
*  seq：保存魔法动画的序列
*  pictureName：动画文件名
*	directon：魔法的朝向
*	len：魔法的有效距离
*  rowBeginIndex：所需要的动画在文件的第几行（开始）
*	colEndIndex：所需要的动画在文件的第几行（结束）
*  colBeginIndex： 开始的列位置
*	colEndIndex：结束的列位置
*  width：动画宽度
*  height：动画高度
*  time_interval：动画播放的时间间隔
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