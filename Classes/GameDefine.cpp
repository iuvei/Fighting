#include "GameDefine.h"

std::string			GlobalValue::server_ip										= "127.0.0.1";
int					GlobalValue::server_port									= 8899;
int					GlobalValue::hero_move_step							= 0;
int					GlobalValue::magic_distance								= 0;
float					GlobalValue::parry_probability							= 0;
AI_Probability	GlobalValue::nomal_ai_probability;
AI_Probability	GlobalValue::attack_ai_probability;
bool					GlobalValue::use_ai											= false;
int					GlobalValue::ai_check_distance							= 0;
int					GlobalValue::ai_attack_distance							= 0;
int					GlobalValue::magic_move_step							= 0;

int					GlobalValue::attack_damage								= 0;
int					GlobalValue::magic_damage								= 0;
int					GlobalValue::magic_damage_distance				= 0;
int					GlobalValue::attack_damage_distance				= 0;
int					GlobalValue::magic_cost									= 0;
int					GlobalValue::blood_regain_value						= 0;
int					GlobalValue::magic_regain_value						= 0;

#ifdef _TEST_
std::string			GlobalValue::current_hero_name						= "bandit";
std::string			GlobalValue::current_enemy_name					= "rudolf";
#endif
/*
HeroInfoMap G_HeroInfo[] = 
{
	{BANDIT_ID,BANDIT_NAME},
	{BAT_ID,BAT_NAME},
	{DAVIS_ID,DAVIS_NAME},
	{DEEP_ID,DEEP_NAME},
	{DENNIS_ID,DENNIS_NAME},
	{FIREN_ID,FIREN_NAME},
	{FIRZEN_ID,FIRZEN_NAME},
	{FREEZE_ID,FREEZE_NAME},
	{HENRY_ID,HENRY_NAME},
	{HUNTER_ID,HUNTER_NAME},
	{JACK_ID,JACK_NAME},
	{JAN_ID,JAN_NAME},
	{JOHN_ID,JOHN_NAME},
	{JULIAN_ID,JULIAN_NAME},
	{JUSTIN_ID,JUSTIN_NAME},
	{KNIGHT_ID,KNIGHT_NAME},
	{LOUIS_ID,LOUIS_NAME},
	{LOUISEX_ID,LOUISEX_NAME},
	{MARK_ID,MARK_NAME},
	{MONK_ID,MONK_NAME},
	{RUDOLF_ID,RUDOLF_NAME},
	{SORCERER_ID,SORCERER_NAME},
	{WOODY_ID,WOODY_NAME},
};


HERO_ID GlobalValue::GetHeroIDByName(const char* pHeroName)
{
	int count = sizeof(G_HeroInfo)/sizeof(HeroInfoMap);
	for (int i = 0; i < count; ++i)
	{
		if (0 == strcmp(pHeroName,G_HeroInfo[i].name.c_str()))
		{
			return G_HeroInfo[i].id;
		}
	}

	return INVALID_ID;
}

const char* GlobalValue::GetHeroNameByID(HERO_ID id)
{
	int count = sizeof(G_HeroInfo)/sizeof(HeroInfoMap);
	for (int i = 0; i < count; ++i)
	{
		if (id == G_HeroInfo[i].id)
		{
			return G_HeroInfo[i].name.c_str();
		}
	}

	return "";
}

int GlobalValue::GetHeroCount()
{
	return  sizeof(G_HeroInfo)/sizeof(HeroInfoMap);
}*/