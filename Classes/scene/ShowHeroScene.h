#ifndef _SHOW_HERO_SCENE_H_
#define _SHOW_HERO_SCENE_H_


#include "cocos2d.h"

#include "cocos-ext.h"

#include "GameDefine.h"

#include "NetworkModule.h"

#include "../CommonClass//GameRound.h"

class HeroListView;


class ShowHeroScene:public cocos2d::Layer
{
public:
	ShowHeroScene();

	virtual ~ShowHeroScene();

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(ShowHeroScene);
public:
	void					update(float delta);
	static void			jumpToMainScene(float t);

	void			CreateMenu();
	void			LoadBackGround();
	void			BeginGame();
	void			CreateGame();
	void			JoinGame();
	void			SettingGame();
	void			ExitGame();

	void			CreateGameRoundMenu(vector<GameRoundInfo>& gameRounfInfo);
private:
	bool			m_bJoinGame;
	bool			m_bIsCreator;

	HERO_ID m_nCurrentHeroID;
	HERO_ID m_nCurrentEnemyID;

	HeroListView*			m_pHeroListView;
};
#endif // !_SHOW_HERO_SCENE_H_
