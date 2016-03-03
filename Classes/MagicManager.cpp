#include "MagicManager.h"
#include "GameLogic.h"
#include "./scene/MainScene.h"
#include "Magic.h"
#include "HeroManager.h"
#include "../CommonClass/Util.h"
#include "cocos2d.h"
USING_NS_CC;
MagicManager* MagicManager::m_pInstance = 0;

MagicManager::MagicManager()
{

}

MagicManager* MagicManager::GetInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new MagicManager;
	}

	return m_pInstance;
}

/************************************************************************
*  把一个未处理的魔法加入到魔法队列中
************************************************************************/
void MagicManager::InsertMagic(Magic* mag)
{
	GameLogic::GetInstance()->GetMainScene()->addChild(mag,2);
	m_MagicVector.push_back(mag);
}


/************************************************************************
*  更新魔法的位置（由计时器调用更新）
************************************************************************/
void MagicManager::UpdateMagic()
{
	for (MagicVectorIter it = m_MagicVector.begin(); it != m_MagicVector.end(); ++it)
	{
		Point p = (*it)->getPosition();
		if ((*it) ->GetDirection()== directon_right)
		{
			(*it)->setPosition(Point(p.x + GlobalValue::magic_move_step,p.y));
		}
		else if((*it) ->GetDirection() == directon_left)
		{
			(*it)->setPosition(Point(p.x - GlobalValue::magic_move_step,p.y));
		}
	}
}


/************************************************************************
*  碰撞检测（由计时器定时调用）
************************************************************************/
void MagicManager::MagicCollisionDetection()
{
	if (!GameLogic::GetInstance()->IsGameRunning())
	{
		m_MagicVector.clear();
		return ;
	}
	// 遍历所有的魔法
	for (MagicVectorIter it = m_MagicVector.begin(); it != m_MagicVector.end(); ++it)
	{
		// 如果当前魔法已经失效，那么将其从队列中删除，然后跳出循环（因为每删除一次，vector的迭代器就会失效，所以要跳出循环）
		if (!(*it)->IsValid())
		{
			m_MagicVector.erase(it);
			break;
		}

		// 当前的魔法
		Magic* pMagic = (*it);

		// 获取魔法的当前位置
		Point magic_point = pMagic->getPosition();

		// 处理标志，如果已经处理那么可以从队列中将其删除
		bool bHandle  =  false;

		// 对每一个英雄
		vector<Hero*> hero_list = HeroManager::GetInstance()->GetHeroList();
		for (HeroVectorIter hero_it = hero_list.begin(); hero_it != hero_list.end(); ++hero_it)
		{
			Hero* pHero = (*hero_it);
			if (pHero == 0)
			{
				continue;
			}

			if (pHero->GetHeroID() == (*it)->BelongTo())
			{
				continue;
			}

			// 如果英雄已经死亡，那么处理下一个英雄
			if (pHero->GetHeroState() == DEATH_STATE)
			{
				continue;;
			}

			float x = pHero->getPosition().x - magic_point.x;

			float y = pHero->getPosition().y - magic_point.y;

			// 计算魔法和英雄之间的距离
			float distance = sqrt(pow(x,2) + pow(y,2));

			// 如果大于魔法的伤害距离，那么处理下一个英雄
			if (distance > GlobalValue::magic_damage_distance)
			{
				continue;
			}

			// 获取一个随机概率
			float probability = Util::GetProbability();

			if (probability <= GlobalValue::parry_probability)
			{
				// 格挡
				pHero->Parry();
			}
			else
			{
				// 被伤害，掉血
				pHero->SetBloodValue(pHero->GetBloodValue() - pMagic->GetMagicDamage());
			}

			bHandle = true;
			break;

		}

		if (!GameLogic::GetInstance()->IsGameRunning())
		{
			return;
		}
		// 已经处理过的魔法，从队列中将其删除
		if (bHandle)
		{
			GameLogic::GetInstance()->GetMainScene()->removeChild(*it);
			m_MagicVector.erase(it);
			break;
		}

	}

}

void MagicManager::Clean()
{
	m_MagicVector.clear();
}

void MagicManager::RandomMagic(int xPos,int yPos)
{
	Point point(xPos,yPos);

	cocos2d::Sequence *sequence = NULL;
	cocos2d::Spawn *spawn = NULL;

	Animation *attack;
	Texture2D *pic = 0;
	Vector<SpriteFrame*> arr;

	pic = TextureCache::getInstance()->addImage("LFR/magic/firen_exp.png");


	for (int y = (0); y < (3); ++y)
	{
		for (int x = (0); x < (5); ++x)
		{
			SpriteFrame* sf = SpriteFrame::createWithTexture(pic,CCRectMake(x * (160) ,y * (160),( 160),(160)));
			arr.pushBack(sf);
		}
	}

	auto magic = Magic::create();
	magic->setPosition(point);
	attack = Animation::createWithSpriteFrames(arr,0.2f);
	attack->setLoops(-1);

	auto action = Animate::create(attack);

	sequence = Sequence::create(action,0);

	InsertMagic(magic);

	magic->PlayMagic(sequence,point);
}


void MagicManager::ShowLamp()
{
	

	cocos2d::Sequence *sequence = NULL;
	cocos2d::Spawn *spawn = NULL;

	Animation *attack;
	Texture2D *pic = 0;
	Vector<SpriteFrame*> arr;

	pic = TextureCache::getInstance()->addImage("LFR/magic/firen_flame.png");


	for (int y = (0); y < (3); ++y)
	{
		for (int x = (0); x < (6); ++x)
		{
			SpriteFrame* sf = SpriteFrame::createWithTexture(pic,CCRectMake(x * (60) ,y * (64),( 60),(64)));
			arr.pushBack(sf);
		}
	}

	Point point(200,500);

	for (int i = 0 ;i < 6; ++i)
	{
		auto magic = Sprite::create();
		magic->setPosition(point);
		attack = Animation::createWithSpriteFrames(arr,0.2f);
		attack->setLoops(-1);

		auto action = Animate::create(attack);

		sequence = Sequence::create(action,0);

		GameLogic::GetInstance()->GetMainScene()->addChild(magic);

		magic->runAction(sequence);

		point.x += 100;
	}
}

void MagicManager::RandomMoney(int xPos,int yPos)
{
	auto magic = Magic::create();
	magic->SetMagicDamage(-5);
	InsertMagic(magic);
	magic->PlayMagic("LFR/other/component/money.png",Point(xPos,yPos));
	magic->setScale(0.1);
	
}

void MagicManager::TowerShoot()
{
	Point point(220,400);

	cocos2d::Sequence *sequence = NULL;
	cocos2d::Spawn *spawn = NULL;

	Animation *attack;
	Texture2D *pic = 0;
	Vector<SpriteFrame*> arr;

	pic = TextureCache::getInstance()->addImage("LFR/other/component/arrow1.png");


	for (int y = (0); y < (1); ++y)
	{
		for (int x = (0); x < (8); ++x)
		{
			SpriteFrame* sf = SpriteFrame::createWithTexture(pic,CCRectMake(x * (49) ,y * (49),( 49),(49)));
			arr.pushBack(sf);
		}
	}

	MoveBy* by = MoveBy::create(0.5,Point(200,-150));

	auto magic = Magic::create();
	magic->setPosition(point);
	magic->setScale(1.4);
	attack = Animation::createWithSpriteFrames(arr,0.1f);

	magic->SetMagicDamage(10);
	auto action = Animate::create(attack);

	spawn = Spawn::create(action,by,NULL);
	sequence = Sequence::create(spawn,0);

	InsertMagic(magic);

	magic->PlayMagic(sequence,point);
}