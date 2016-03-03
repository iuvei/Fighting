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
*  ��һ��δ�����ħ�����뵽ħ��������
************************************************************************/
void MagicManager::InsertMagic(Magic* mag)
{
	GameLogic::GetInstance()->GetMainScene()->addChild(mag,2);
	m_MagicVector.push_back(mag);
}


/************************************************************************
*  ����ħ����λ�ã��ɼ�ʱ�����ø��£�
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
*  ��ײ��⣨�ɼ�ʱ����ʱ���ã�
************************************************************************/
void MagicManager::MagicCollisionDetection()
{
	if (!GameLogic::GetInstance()->IsGameRunning())
	{
		m_MagicVector.clear();
		return ;
	}
	// �������е�ħ��
	for (MagicVectorIter it = m_MagicVector.begin(); it != m_MagicVector.end(); ++it)
	{
		// �����ǰħ���Ѿ�ʧЧ����ô����Ӷ�����ɾ����Ȼ������ѭ������Ϊÿɾ��һ�Σ�vector�ĵ������ͻ�ʧЧ������Ҫ����ѭ����
		if (!(*it)->IsValid())
		{
			m_MagicVector.erase(it);
			break;
		}

		// ��ǰ��ħ��
		Magic* pMagic = (*it);

		// ��ȡħ���ĵ�ǰλ��
		Point magic_point = pMagic->getPosition();

		// �����־������Ѿ�������ô���ԴӶ����н���ɾ��
		bool bHandle  =  false;

		// ��ÿһ��Ӣ��
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

			// ���Ӣ���Ѿ���������ô������һ��Ӣ��
			if (pHero->GetHeroState() == DEATH_STATE)
			{
				continue;;
			}

			float x = pHero->getPosition().x - magic_point.x;

			float y = pHero->getPosition().y - magic_point.y;

			// ����ħ����Ӣ��֮��ľ���
			float distance = sqrt(pow(x,2) + pow(y,2));

			// �������ħ�����˺����룬��ô������һ��Ӣ��
			if (distance > GlobalValue::magic_damage_distance)
			{
				continue;
			}

			// ��ȡһ���������
			float probability = Util::GetProbability();

			if (probability <= GlobalValue::parry_probability)
			{
				// ��
				pHero->Parry();
			}
			else
			{
				// ���˺�����Ѫ
				pHero->SetBloodValue(pHero->GetBloodValue() - pMagic->GetMagicDamage());
			}

			bHandle = true;
			break;

		}

		if (!GameLogic::GetInstance()->IsGameRunning())
		{
			return;
		}
		// �Ѿ��������ħ�����Ӷ����н���ɾ��
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