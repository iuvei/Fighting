#ifndef _GAMEROUND_VIEW_H_
#define _GAMEROUND_VIEW_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameDefine.h"
#include "cocos-ext.h"
#include "HeroListView.h"
#include "NetworkModule.h"

USING_NS_CC_EXT;
USING_NS_CC;


class GameRoundListView:public cocos2d::CCLayer,public cocos2d::extension::TableViewDelegate,public cocos2d::extension::TableViewDataSource
{
public:
	CREATE_FUNC(GameRoundListView);

	bool					InitView(Point ori,Size itemSize,int count,bool bVERTICAL,bool bScale,vector<GameRound>& gamrround);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}

	//触摸到某个cell的事件
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);

	//定制每个cell的size
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);

	//定制每个cell的内容
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);

	//确定这个tableview的cell行数
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);

	//设置单元格高亮的状态
	virtual void tableCellHighlight(TableView* table, TableViewCell* cell);

	//设置单元格非高亮的状态
	virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);

	int				GetItemCount()
	{
		return m_nItemCount;
	}

	Size				GetItemSize()
	{
		return m_ItemSize;
	}

	int					GetHeroID()
	{
		return m_nHeroID;
	}

private:
	Size					m_ItemSize;
	int					m_nItemCount;
	int					m_nHeroID;		
	vector<GameRound>		m_GameRoundList;

};
#endif // !_XLIST_VIEW_H_
