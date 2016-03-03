#include "HeroListView.h"
#include "HeroManager.h"
bool HeroListView::InitView(Point ori,Size itemSize,int count,bool bVERTICAL,bool bScale)
{
	if ( !Layer::init() )
	{
		return false;
	}

	m_pSprite = 0;

	Size winSize = Director::getInstance()->getWinSize();

	Size realSize = itemSize;
	if (bScale)
	{
		realSize.width = realSize.width * 2;
		realSize.height = realSize.height * 2;
	}

	m_ItemSize = realSize;

	m_nItemCount = count;

	if (bVERTICAL)// shuizhi
	{
		TableView* tableView = TableView::create(this, Size(realSize.width, realSize.height * 5));
		tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
		tableView->setPosition(ori);
		tableView->setDelegate(this);
		tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		this->addChild(tableView);
		tableView->reloadData();
	}
	else // shuiping
	{
		TableView* tableView = TableView::create(this, Size(realSize.width * 5, realSize.height));
		tableView->setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
		tableView->setPosition(ori);
		tableView->setDelegate(this);
		this->addChild(tableView);
		tableView->reloadData();
	}

	m_nHeroID = INVALID_ID;

	return true;
}

//触摸到某个cell的事件
void HeroListView::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
	HERO_ID id = HERO_ID(cell->getTag());

	std::string path = "";

	m_nHeroID = INVALID_ID;

	switch ((HERO_ID)id)
	{
	case BANDIT_ID:
		path = "LFR/bandit/bandit_0_mirror.png";
		break;
	case BAT_ID:
		path = "LFR/bat/bat_0_mirror.png";
		break;
	case DAVIS_ID:
		path = "LFR/davis/davis_0_mirror.png";
		break;
	case DEEP_ID:
		path = "LFR/deep/deep_0_mirror.png";
		break;
	case DENNIS_ID:
		path = "LFR/dennis/dennis_0_mirror.png";
		break;
	case FIREN_ID:
		path = "LFR/firen/firen_0_mirror.png";
		break;
	case FIRZEN_ID:
		path = "LFR/firzen/firzen_0_mirror.png";
		break;
	case FREEZE_ID:
		path = "LFR/freeze/freeze_0_mirror.png";
		break;
	case HENRY_ID:
		path = "LFR/henry/henry_0_mirror.png";
		break;
	case HUNTER_ID:
		path = "LFR/hunter/hunter_0_mirror.png";
		break;
	case JACK_ID:
		path = "LFR/jack/jack_0_mirror.png";
		break;
	case JAN_ID:
		path = "LFR/jan/jan_0_mirror.png";
		break;
	case JOHN_ID:
		path = "LFR/john/john_0_mirror.png";
		break;
	case JULIAN_ID:
		path = "LFR/julian/julian_0_mirror.png";
		break;
	case JUSTIN_ID:
		path = "LFR/justin/justin_0_mirror.png";
		break;
	case KNIGHT_ID:
		path = "LFR/knight/knight_0_mirror.png";
		break;
	case LOUIS_ID:
		path = "LFR/louis/louis_0_mirror.png";
		break;
	case LOUISEX_ID:
		path = "LFR/louisex/louisex_0_mirror.png";
		break;
	case MARK_ID:
		path = "LFR/mark/mark_0_mirror.png";
		break;
	case MONK_ID:
		path = "LFR/monk/monk_0_mirror.png";
		break;
	case RUDOLF_ID:
		path = "LFR/rudolf/rudolf_0_mirror.png";
		break;
	case SORCERER_ID:
		path = "LFR/sorcerer/sorcerer_0_mirror.png";
		break;
	case WOODY_ID:
		path = "LFR/woody/woody_0_mirror.png";
		break;
	default:
		return;
		break;
	}

	if (m_pSprite != 0)
	{
		m_pSprite->stopAllActions();
		this->removeChild(m_pSprite);
		m_pSprite = 0;
	}

	m_nHeroID = id;

	cocos2d::Sequence *sequence = NULL;
	cocos2d::Spawn *spawn = NULL;

	Animation *attack;
	Texture2D *pic = 0;
	Vector<SpriteFrame*> arr;

	pic = TextureCache::getInstance()->addImage(path.c_str());

	for (int i = 9; i >= 2; --i)
	{
		int index = 0;
		SpriteFrame* sf = SpriteFrame::createWithTexture(pic,Rect(i * 80,index * 80,80,80));
		arr.pushBack(sf);
	}
	m_pSprite = Sprite::create();
	m_pSprite->setPosition(450,300);
	m_pSprite->setScale(3);
	this->addChild(m_pSprite);

	attack = Animation::createWithSpriteFrames(arr,0.1f);

	auto action = Animate::create(attack);
	auto Repeat_left = Repeat::create(action,-2.5);	
	auto m_pSequence = Sequence::create(action,Repeat_left,NULL);
	m_pSprite->runAction(m_pSequence);

}

//定制每个cell的size
cocos2d::CCSize HeroListView::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return m_ItemSize;
}

//定制每个cell的内容
cocos2d::extension::TableViewCell* HeroListView::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{

	/*CCString *string = CCString::createWithFormat("index = %d",idx);
	TableViewCell *cell = table->dequeueCell();
	if (!cell) 
	{
	cell = new TableViewCell();//CustomTableCell();
	cell->autorelease();

	//添加背景图片
	CCSprite *sprite = CCSprite::create("LFR/other/button_p.png");
	sprite->setAnchorPoint(Point(0,0));
	sprite->setPosition(Point(0, 0));
	cell->addChild(sprite,1);

	//添加label
	CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Arial", 15);
	label->setAnchorPoint(Point(0,0));
	label->setTag(123);
	label->cocos2d::CCNode::setPosition(5, 5);
	cell->addChild(label,2);

	//添加menu
	CCMenuItemImage *itemImage = CCMenuItemImage::create("LFR/other/button.png", "LFR/other/button_p.png", cell, NULL);
	CCMenu *menu = CCMenu::create(itemImage,NULL);

	//注意CCMenu是无法修改锚点的，其默认位置是在其几何中心
	menu->setPosition(ccp(180,26));

	//可以通过修改CCMenuItemImage的锚点的位置
	//        itemImage->setAnchorPoint(CCPointZero);
	//        menu->setPosition(ccp(180,6));

	cell->addChild(menu, 2);
	}

	else
	{
	//由于每一个cell的label都是不同的，所以我们通过tag值获取到label来对label进行修改
	CCLabelTTF *label = (CCLabelTTF*)cell->getChildByTag(123);
	label->setString(string->getCString());
	}*/

	std::string name = HeroManager::GetHeroNameByID((HERO_ID)idx);
	TableViewCell *cell ;//= table->dequeueCell();
	std::string path = "";
	std::string suffix_name = "_f.png";
	switch ((HERO_ID)idx)
	{
	case BANDIT_ID:
		path = "LFR/bandit/bandit";
		break;
	case BAT_ID:
		path = "LFR/bat/bat";
		break;
	case DAVIS_ID:
		path = "LFR/davis/davis";
		break;
	case DEEP_ID:
		path = "LFR/deep/deep";
		break;
	case DENNIS_ID:
		path = "LFR/dennis/dennis";
		break;
	case FIREN_ID:
		path = "LFR/firen/firen";
		break;
	case FIRZEN_ID:
		path = "LFR/firzen/firzen";
		break;
	case FREEZE_ID:
		path = "LFR/freeze/freeze";
		break;
	case HENRY_ID:
		path = "LFR/henry/henry";
		break;
	case HUNTER_ID:
		path = "LFR/hunter/hunter";
		break;
	case JACK_ID:
		path = "LFR/jack/jack";
		break;
	case JAN_ID:
		path = "LFR/jan/jan";
		break;
	case JOHN_ID:
		path = "LFR/john/john";
		break;
	case JULIAN_ID:
		path = "LFR/julian/julian";
		break;
	case JUSTIN_ID:
		path = "LFR/justin/justin";
		break;
	case KNIGHT_ID:
		path = "LFR/knight/knight";
		break;
	case LOUIS_ID:
		path = "LFR/louis/louis";
		break;
	case LOUISEX_ID:
		path = "LFR/louisex/louisex";
		break;
	case MARK_ID:
		path = "LFR/mark/mark";
		break;
	case MONK_ID:
		path = "LFR/monk/monk";
		break;
	case RUDOLF_ID:
		path = "LFR/rudolf/rudolf";
		break;
	case SORCERER_ID:
		path = "LFR/sorcerer/sorcerer";
		break;
	case WOODY_ID:
		path = "LFR/woody/woody";
		break;
	default:
		break;
	}

	path += suffix_name;


	cell = new (std::nothrow) CustomTableViewCell();
	cell->autorelease();
	auto sprite = Sprite::create(path.c_str());
	sprite->setAnchorPoint(Vec2::ZERO);
	sprite->setPosition(Vec2(0, 0));
	sprite->setScale(1);
	cell->addChild(sprite);
	cell->setTag(idx);

	auto label = Label::createWithSystemFont(name, "Helvetica", 20.0);
	label->setPosition(Vec2::ZERO);
	label->setAnchorPoint(Vec2::ZERO);
	label->setTag(123);
	cell->addChild(label);


	return cell;
}

//确定这个tableview的cell行数
ssize_t HeroListView::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return m_nItemCount;
}

//设置单元格高亮的状态
void HeroListView::tableCellHighlight(TableView* table, TableViewCell* cell)
{
}

//设置单元格非高亮的状态
void HeroListView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

}