#include "MapAnchor.h"
#include "BlockBase.h"
#include "GroundBlock.h"
#include "InteractionBlock.h"
#include "EnvironmentBlock.h"
#include "Mario.h"
#include "Mushroom.h"
#include "Star.h"
#include "StackBlock.h"
#include "FireBall.h"
#include "Kuri.h"
#include "Nori.h"
#include "ScoreBoard.h"
#include "GameMiddleScene.h"
#include "Flag.h"
#include "Castle.h"
#include <fstream>

USING_NS_CC;

bool MapAnchor::init()
{
	m_flag = NULL;

	m_mario = Mario::create();
	m_mario->SetMapAnchore(this);
	this->addChild(m_mario, SORT_ORDER::MARIO);

	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_backgroundColor = Color4F(145 / 255.0f, 143 / 255.0f, 254 / 255.0f, 1.0f);
	m_oneBlockSize = 80;
	m_visibleBlockWidthCount = 16;
	m_overCheckCollLineCount = 8;
	m_savePoint = 6000;
	m_pillarCount = 0;
	m_floorCount = 0;

	m_changeSceneTime = 3.0f;
	m_changeSceneDeltaTime = 0;

	m_fireBallIdx = 0;

	for (size_t i = 0; i < MAX_FIRE_BALL_COUNT; i++)
	{
		m_fireBallInfos[i].fireBall = FireBall::create();
		m_fireBallInfos[i].fireBall->setPosition(Vec2(-100, -100));

		for (auto& adjoinBlock : m_fireBallInfos[i].adjoinBlock)
		{
			adjoinBlock.block = NULL;
			adjoinBlock.collRect = Rect::ZERO;
		}

		this->addChild(m_fireBallInfos[i].fireBall, SORT_ORDER::FIRE_BALL);
	}

	this->setAnchorPoint(Vec2(0, 0));
	this->setPosition(Vec2(0, -m_oneBlockSize/2));
	
	m_background = DrawNode::create();
	this->addChild(m_background, SORT_ORDER::BACKGROUND);
	ClearAdjoinBlock();

	return true;
}

void MapAnchor::SetCheckCollStartIdx()
{
	Vec2 curPos = this->getPosition();
	m_checkCollStartIdx = (curPos.x * -1) / 80 - m_overCheckCollLineCount;
	if (m_checkCollStartIdx < 0)
	{
		m_checkCollStartIdx = 0;
	}
}

void MapAnchor::MoveMapAnchor(float dist)
{
	Vec2 curPos = this->getPosition();
	curPos.x -= dist;

	if (curPos.x < m_mapAnchorEndXpos)
	{
		curPos.x = m_mapAnchorEndXpos;
	}

	this->setPosition(curPos);

	SetCheckCollStartIdx();
}

void MapAnchor::DrawBackground()
{
	m_background->cleanup();
	m_background->drawSolidRect(Vec2(0, m_oneBlockSize / 2), Vec2(m_oneBlockSize * m_blocks.size(), m_visibleSize.height + m_oneBlockSize / 2), m_backgroundColor);
}

void MapAnchor::onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	m_mario->onKeyPressed(keycode, event);
}

void MapAnchor::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	m_mario->onKeyReleased(keycode, event);
}

void MapAnchor::UpdateMapAnchor(float deltaTime)
{
	float marioMoveXdist;
	
	switch (m_mario->GetMarioState())
	{
	case Mario::MARIO_STATE::DIED:
		m_changeSceneDeltaTime += deltaTime;
		if (m_changeSceneTime < m_changeSceneDeltaTime)
		{
			ReleaseObj();
			this->getParent()->removeChild(ScoreBoard::GetInstance(), false);
			GameMiddleScene* gameMiddleScene = GameMiddleScene::create();
			gameMiddleScene->SetScoreBoard();
			Director::getInstance()->replaceScene(gameMiddleScene);
		}
		else
		{
			m_mario->UpdateMario(deltaTime, &marioMoveXdist);
		}
		return;
		break;
	case Mario::MARIO_STATE::FLAG_DOWNING:
		for (auto fireballInfo : m_fireBallInfos)
		{
			fireballInfo.fireBall->SetDestroy();
		}
		m_mario->UpdateMario(deltaTime, &marioMoveXdist);
		return;
		break;
	case Mario::MARIO_STATE::GO_END:
	{
		m_mario->UpdateMario(deltaTime, &marioMoveXdist);

		Vec2 mapAnchorStartPos;
		mapAnchorStartPos = this->getPosition();
		mapAnchorStartPos.x *= -1;
		if (marioMoveXdist > 0 && m_mario->getPositionX() > mapAnchorStartPos.x + m_visibleSize.width / 3 - 40)
		{
			if (m_mario->getPositionX() > mapAnchorStartPos.x + m_visibleSize.width / 2 - 40)
			{
				MoveMapAnchor(marioMoveXdist);
			}
			else
			{
				MoveMapAnchor(marioMoveXdist * 0.5);
			}
		}
	}
		return;
		break;
	case Mario::MARIO_STATE::CLEAR:
	{
		ScoreBoard::GetInstance()->ChangeTimeToScore(deltaTime);
		if (ScoreBoard::GetInstance()->GetTime() == 0)
		{
			m_castle->SetCongratulation(deltaTime);
			if (m_castle->IsFinishCong())
			{
				m_changeSceneDeltaTime += deltaTime;
				if (m_changeSceneTime < m_changeSceneDeltaTime)
				{
					ReleaseObj();
					this->getParent()->removeChild(ScoreBoard::GetInstance(), false);
					GameMiddleScene* gameMiddleScene = GameMiddleScene::create();
					gameMiddleScene->SetGameClear();
					gameMiddleScene->SetScoreBoard();
					Director::getInstance()->replaceScene(gameMiddleScene);
				}
			}
		}
	}
		return;
		break;
	default:
		break;
	}

	if (m_mario->IsChanging())
	{
		m_mario->UpdateMario(deltaTime, &marioMoveXdist);
		return;
	}

	ScoreBoard::GetInstance()->UpdateScoreBoard(deltaTime);
	if (ScoreBoard::GetInstance()->GetTime() == 0)
	{
		m_mario->SetDie();
	}

	for (int i = 0; i < MAX_FIRE_BALL_COUNT; i++)
	{
		m_fireBallInfos[i].fireBall->UpdateFireBall(deltaTime, (m_checkCollStartIdx + m_overCheckCollLineCount + m_visibleBlockWidthCount + 4) * m_oneBlockSize);
	}

	for (auto& monsterInfo : m_monsterInfos)
	{
		monsterInfo.monster->UpdateMonster(deltaTime);
	}

	for (auto item : m_itemInfos)
	{
		item.item->UpdateItem(deltaTime);
	}
	
	m_mario->UpdateMario(deltaTime, &marioMoveXdist);

	Vec2 mapAnchorStartPos;
	mapAnchorStartPos = this->getPosition();
	mapAnchorStartPos.x *= -1;


	if (marioMoveXdist < 0 && m_mario->getPositionX() < mapAnchorStartPos.x)//not move mapanchor to left
	{
		m_mario->setPositionX(mapAnchorStartPos.x);
	}

	if (marioMoveXdist > 0 && m_mario->getPositionX() > mapAnchorStartPos.x + m_visibleSize.width / 3 - 40)
	{
		if (m_mario->getPositionX() > mapAnchorStartPos.x + m_visibleSize.width / 2 - 40)
		{
			MoveMapAnchor(marioMoveXdist);
		}
		else
		{
			MoveMapAnchor(marioMoveXdist * 0.5);
		}
	}

	CheckCollBlocksWithOtherObj();

	for (int idx = m_checkCollStartIdx; idx < m_checkCollStartIdx + m_visibleBlockWidthCount + m_overCheckCollLineCount * 2; idx++)
	{
		if (idx >= m_blocks.size())
		{
			break;
		}

		for (auto block : m_blocks[idx])
		{
			if (block != NULL)
			{
				block->LateUpdateBlock(deltaTime);
			}
		}
	}

	Item::ITEM_TYPE itemType;
	for (auto item : m_itemInfos)
	{
		if (item.item->CheckCollWithMario(m_mario->GetCollider(), &itemType))
		{
			m_mario->EatItem(itemType);
		}
	}

	Rect monsterCollider;
	Rect fireBallCollider;
	for (auto& monsterInfo : m_monsterInfos)
	{
		for (auto fireBall : m_fireBallInfos)
		{
			fireBallCollider = fireBall.fireBall->GetCollider();
			monsterCollider = monsterInfo.monster->GetCollider();
			if (fireBallCollider.intersectsRect(monsterCollider))
			{
				fireBall.fireBall->SetDestroy();
				if (m_mario->getPositionX() < monsterCollider.getMinX())
				{
					monsterInfo.monster->AttackedFromBullet(-1);//leftAttack
				}
				else
				{
					monsterInfo.monster->AttackedFromBullet(1);//RightAttack
				}
			}
		}
	}

	Monster* monster[2];

	for (int monsterIdx1 = 0; monsterIdx1 < m_monsterInfos.size(); monsterIdx1++)
	{
		for (int monsterIdx2 = monsterIdx1 + 1; monsterIdx2 < m_monsterInfos.size(); monsterIdx2++)
		{
			monster[0] = m_monsterInfos[monsterIdx1].monster;
			monster[1] = m_monsterInfos[monsterIdx2].monster;

			if (monster[0]->GetCollider(true).intersectsRect(monster[1]->GetCollider(true)))
			{
				if (monster[0]->IsCanTeamKill())
				{
					if (monster[0]->getPositionX() < monster[1]->getPositionX())
					{
						monster[1]->AttackedFromBullet(-1);
					}
					else
					{
						monster[1]->AttackedFromBullet(1);
					}
				}
				else if (monster[1]->IsCanTeamKill())
				{
					if (monster[1]->getPositionX() < monster[0]->getPositionX())
					{
						monster[0]->AttackedFromBullet(-1);
					}
					else
					{
						monster[0]->AttackedFromBullet(1);
					}
				}
				else
				{
					monster[0]->SetXPositionBecauseColl(0);
					monster[1]->SetXPositionBecauseColl(0);
				}
			}
		}
	}

	bool attakedUpside = false;
	for (int monsterIdx = 0; monsterIdx < m_monsterInfos.size(); monsterIdx++)
	{
		Monster* neighborMonster = NULL;
		if (monsterIdx + 1 < m_monsterInfos.size())
		{
			neighborMonster = m_monsterInfos[monsterIdx + 1].monster;
		}

		if (m_monsterInfos[monsterIdx].monster->CheckCollWithMario(m_mario->GetCollider(), m_mario->IsInvincible(), m_mario->IsDroping(), &attakedUpside, neighborMonster))
		{
			if (attakedUpside)
			{
				m_mario->SetJumpBecauseCollWithMonster();
			}
			else
			{
				m_mario->AttackedFromMonster();
			}
			break;
		}
	}

	m_mario->LateUpdateMario();
	if (m_savePoint < m_mario->getPositionX())
	{
		ScoreBoard::GetInstance()->TouchSavePoint();
	}
}

void MapAnchor::CheckCollBlocksWithOtherObj()
{
	Rect collisionRect = Rect::ZERO;

	for (int idx = m_checkCollStartIdx; idx < m_checkCollStartIdx + m_visibleBlockWidthCount + m_overCheckCollLineCount * 2; idx++)
	{
		if (idx >= m_blocks.size())
		{
			break;
		}

		for (auto block : m_blocks[idx])
		{
			if (block != NULL)
			{
				if (block->CheckColl(m_mario->GetCollider(), &collisionRect))
				{
					SetAdjoinBlock(block, collisionRect, m_mariosAdjoinBlock);
				}

				for (auto& monsterInfo : m_monsterInfos)
				{
					if (block->CheckColl(monsterInfo.monster->GetCollider(), &collisionRect))
					{
						if (collisionRect.size.height < collisionRect.size.width &&
							block->getPositionY() < monsterInfo.monster->getPositionY())
						{
							if (block->getPositionX() + m_oneBlockSize / 2 < monsterInfo.monster->getPositionX())
							{
								block->SetCollidedMonster(monsterInfo.monster, false);
							}
							else
							{
								block->SetCollidedMonster(monsterInfo.monster, true);
							}
						}

						SetAdjoinBlock(block, collisionRect, monsterInfo.adjoinBlock);
					}
				}

				for (int i = 0; i < MAX_FIRE_BALL_COUNT; i++)
				{
					if (block->CheckColl(m_fireBallInfos[i].fireBall->GetCollider(), &collisionRect))
					{
						SetAdjoinBlock(block, collisionRect, m_fireBallInfos[i].adjoinBlock);
					}
				}

				for (int itemIdx = 0; itemIdx < m_itemInfos.size(); itemIdx++)
				{
					if (block->CheckColl(m_itemInfos[itemIdx].item->GetCollider(), &collisionRect))
					{
						if (collisionRect.size.height < collisionRect.size.width &&
							block->getPositionY() < m_itemInfos[itemIdx].item->getPositionY())
						{
							if (block->getPositionX() + m_oneBlockSize / 2 < m_itemInfos[itemIdx].item->getPositionX())
							{
								block->SetCollidedItem(m_itemInfos[itemIdx].item, false);
							}
							else
							{
								block->SetCollidedItem(m_itemInfos[itemIdx].item, true);
							}
						}
						SetAdjoinBlock(block, collisionRect, m_itemInfos[itemIdx].adjoinBlock);
					}
				}
			}
		}
	}

	if (m_flag->CheckCollFlagWithMario(m_mario))
	{
		m_mario->setPositionX(m_flag->getPositionX() - 30);
		m_mario->SetFlagDowning();
	}
	else if (m_flag->CheckColl(m_mario->GetCollider(), &collisionRect))
	{
		SetAdjoinBlock(m_flag, collisionRect, m_mariosAdjoinBlock);
	}

	for (auto& monsterInfo : m_monsterInfos)
	{
		if (monsterInfo.adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block != NULL)
		{
			if (monsterInfo.monster->getPositionY() < monsterInfo.adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block->getPositionY())
			{
				if (monsterInfo.adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect.size.width > 10)
				{
					monsterInfo.monster->SetYPositionBecauseColl(-monsterInfo.adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect.size.height);
				}
			}
			else
			{
				monsterInfo.monster->SetYPositionBecauseColl(monsterInfo.adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect.size.height);
			}
		}

		if (monsterInfo.adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].block != NULL)
		{
			if (monsterInfo.adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.height > 40)
			{
				if (monsterInfo.monster->getPositionX() < monsterInfo.adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].block->getPositionX())
				{
					monsterInfo.monster->SetXPositionBecauseColl(-monsterInfo.adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.width);
				}
				else
				{
					monsterInfo.monster->SetXPositionBecauseColl(monsterInfo.adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.width);
				}
			}
		}
	}

	for (int i = 0; i < MAX_FIRE_BALL_COUNT; i++)
	{
		if (m_fireBallInfos[i].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block != NULL)
		{
			if (m_fireBallInfos[i].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block->getPositionY() <
				m_fireBallInfos[i].fireBall->getPositionY())
			{
				m_fireBallInfos[i].fireBall->SetPositionYBecauseCallWithBlock(
					m_fireBallInfos[i].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block->GetTopPosY(),
					m_fireBallInfos[i].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect.size.height);
			}
		}

		if (m_fireBallInfos[i].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].block != NULL)
		{
			if (m_fireBallInfos[i].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.width <
				m_fireBallInfos[i].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.height)
			{
				m_fireBallInfos[i].fireBall->SetDestroy();
			}
		}
	}

	for (int idx = 0; idx < m_itemInfos.size(); idx++)
	{
		if (m_itemInfos[idx].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block != NULL)
		{
			if (m_itemInfos[idx].item->getPositionY() < m_itemInfos[idx].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block->getPositionY())
			{
				if (m_itemInfos[idx].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect.size.width > 10)
				{
					m_itemInfos[idx].item->SetYPositionBecauseColl(-m_itemInfos[idx].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect.size.height);
				}
			}
			else
			{
				m_itemInfos[idx].item->SetYPositionBecauseColl(m_itemInfos[idx].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect.size.height);
			}
		}

		if (m_itemInfos[idx].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].block != NULL)
		{
			if (m_itemInfos[idx].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.height > 40)
			{
				if (m_itemInfos[idx].item->getPositionX() < m_itemInfos[idx].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].block->getPositionX())
				{
					m_itemInfos[idx].item->SetXPositionBecauseColl(-m_itemInfos[idx].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.width);
				}
				else
				{
					m_itemInfos[idx].item->SetXPositionBecauseColl(m_itemInfos[idx].adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.width);
				}
			}
		}
	}

	if (m_mariosAdjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block != NULL)
	{
		if (m_mario->getPositionY() < m_mariosAdjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block->getPositionY())
		{
			if (m_mariosAdjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect.size.width > 10)
			{
				if (m_mario->IsJumping())
				{
					m_mariosAdjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block->AttackedFromBelow(m_mario->IsBigMario());
					m_mario->SetYPosBecauseColl(-m_mariosAdjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect.size.height);
				}
			}
		}
		else
		{
			m_mario->SetYPosBecauseColl(m_mariosAdjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect.size.height);
		}
	}

	if (m_mariosAdjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].block != NULL)
	{
		if (m_mariosAdjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.height > 20)
		{
			if (m_mario->getPositionX() < m_mariosAdjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].block->getPositionX())
			{
				m_mario->setPositionX(m_mario->getPositionX() - m_mariosAdjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.width);
			}
			else
			{
				m_mario->setPositionX(m_mario->getPositionX() + m_mariosAdjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.width);
			}
		}
	}

	ClearAdjoinBlock();
}

void MapAnchor::SetAdjoinBlock(BlockBase* block, Rect blockCollRect, AdjoinBlock* adjoinBlock, int widthClearance)
{
	if (blockCollRect.size.height < blockCollRect.size.width + widthClearance)
	{
		if (adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block == NULL)
		{
			adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block = block;
			adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect = blockCollRect;
		}
		else
		{
			if (adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect.size.width <
				blockCollRect.size.width)
			{
				adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].block = block;
				adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::VERTICAL].collRect = blockCollRect;
			}
		}
	}
	else
	{
		if (adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].block == NULL)
		{
			adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].block = block;
			adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect = blockCollRect;
		}
		else
		{
			if (adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect.size.height <
				blockCollRect.size.height)
			{
				adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].block = block;
				adjoinBlock[AdjoinBlock::ADJOIN_BLOCK_STATE::HORIZONTAL].collRect = blockCollRect;
			}
		}
	}
}

void MapAnchor::ClearAdjoinBlock()
{
	for (auto& adjoinBlock : m_mariosAdjoinBlock)
	{
		adjoinBlock.block = NULL;
		adjoinBlock.collRect = Rect::ZERO;
	}

	for (auto& monster : m_monsterInfos)
	{
		for (auto& adjoinBlock : monster.adjoinBlock)
		{
			adjoinBlock.block = NULL;
			adjoinBlock.collRect = Rect::ZERO;
		}
	}

	for (auto& fireBallInfo : m_fireBallInfos)
	{
		for (auto& adjoinBlock : fireBallInfo.adjoinBlock)
		{
			adjoinBlock.block = NULL;
			adjoinBlock.collRect = Rect::ZERO;
		}
	}

	for (auto& iteminfo : m_itemInfos)
	{
		for (auto& adjoinBlock : iteminfo.adjoinBlock)
		{
			adjoinBlock.block = NULL;
			adjoinBlock.collRect = Rect::ZERO;
		}
	}
}

void MapAnchor::SetMap(std::string mapTextFilePath)
{
	std::ifstream ifile;

	char tempStr[100] = "";
	std::vector<std::string> stringVec;
	std::vector<std::string> tempStringVec;
	std::vector<std::vector<std::string>> blockStrVec;

	ifile.open(mapTextFilePath);

	if (ifile.is_open())
	{
		while (ifile.getline(tempStr, 100))
		{
			stringVec.push_back(tempStr);
		}
	}

	ifile.close();

	for (std::string oneLineString : stringVec)
	{
		StringSplit(oneLineString, ' ', &tempStringVec);
		tempStringVec.pop_back();
		blockStrVec.push_back(tempStringVec);
		tempStringVec.clear();
	}

	std::vector<BlockBase*> tempBlockVec;
	GroundBlock* tempGroundBlock = NULL;
	EnvironmentBlock* tempEnvironmentBlock = NULL;
	InteractionBlock* tempInteractionBlock = NULL;
	StackBlock* tempStackBlock = NULL;

	int line = 0;
	int arrayIdx = 0;

	for (auto oneBlockLineVec : blockStrVec)
	{
		for (int idx = 0; idx < oneBlockLineVec.size(); idx++)
		{
			if (line == 22)
			{
				idx = idx;
			}

			if (oneBlockLineVec[idx][0] == '_')
			{
				tempStringVec.push_back("N");
				String monsterCharTemp;
				monsterCharTemp.initWithFormat("%c", oneBlockLineVec[idx][1]);
				tempStringVec.push_back(monsterCharTemp._string);
			}
			else
			{
				StringSplit(oneBlockLineVec[idx], '_', &tempStringVec);
			}

			switch (tempStringVec[0][0])
			{
			case 'N':
				break;
			case 'B'://block
				tempInteractionBlock = InteractionBlock::create();
				switch (tempStringVec[0][1])//item
				{
				case '\0':
					tempInteractionBlock->SetBlock(InteractionBlock::INTERACTION_BLOCK_TYPE::BLOCK, SORT_ORDER::INTERACTION_BLOCK);
					break;
				case 'C'://coin
				{
					int coinCount = 0;
					//tempStringVec[0][2] coin count
					std::string coinCountStr = &tempStringVec[0][2];
					coinCount = atoi(coinCountStr.c_str());
					tempInteractionBlock->SetBlock(InteractionBlock::INTERACTION_BLOCK_TYPE::FAKE_QUESTION_BOX, 
						SORT_ORDER::INTERACTION_BLOCK);
					tempInteractionBlock->SetItem(InteractionBlock::INTERACTION_BLOCK_ITEM_TYPE::COIN, NULL, coinCount);
				}
					break;
				case 'M'://mushroom
				{
					tempInteractionBlock->SetBlock(InteractionBlock::INTERACTION_BLOCK_TYPE::FAKE_QUESTION_BOX,
						SORT_ORDER::INTERACTION_BLOCK);
					Mushroom* mushroomTemp = Mushroom::create();
					ItemInfo ItemInfoTemp;
					ItemInfoTemp.item = mushroomTemp;
					for (size_t i = 0; i < 2; i++)
					{
						ItemInfoTemp.adjoinBlock[i].block = NULL;
						ItemInfoTemp.adjoinBlock[i].collRect = Rect::ZERO;
					}
					m_itemInfos.push_back(ItemInfoTemp);
					this->addChild(mushroomTemp, SORT_ORDER::ITEM_STAY);
					tempInteractionBlock->SetItem(InteractionBlock::INTERACTION_BLOCK_ITEM_TYPE::MUSHROOM, mushroomTemp);
				}
					break;
				case 'S'://star
				{
					tempInteractionBlock->SetBlock(InteractionBlock::INTERACTION_BLOCK_TYPE::FAKE_QUESTION_BOX,
						SORT_ORDER::INTERACTION_BLOCK);
					Star* starTemp = Star::create();
					ItemInfo ItemInfoTemp;
					ItemInfoTemp.item = starTemp;
					for (size_t i = 0; i < 2; i++)
					{
						ItemInfoTemp.adjoinBlock[i].block = NULL;
						ItemInfoTemp.adjoinBlock[i].collRect = Rect::ZERO;
					}
					m_itemInfos.push_back(ItemInfoTemp);
					this->addChild(starTemp, SORT_ORDER::ITEM_STAY);
					tempInteractionBlock->SetItem(InteractionBlock::INTERACTION_BLOCK_ITEM_TYPE::STAR, starTemp);
				}
				break;
				default:
					break;
				}
				break;
			case 'G'://ground, grass
				switch (tempStringVec[0][1])
				{
				case '\0'://ground
					tempGroundBlock = GroundBlock::create();
					tempGroundBlock->SetBlock(GroundBlock::GROUND_BLOCK_TYPE::GROUND, SORT_ORDER::GROUND_BLOCK);
					break;
				case 'L'://grass left
					tempEnvironmentBlock = EnvironmentBlock::create();
					tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::GRASS_LEFT, SORT_ORDER::ENVIRONMENT_BLOCK);
					break;
				case 'R'://grass right
					tempEnvironmentBlock = EnvironmentBlock::create();
					tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::GRASS_RIGHT, SORT_ORDER::ENVIRONMENT_BLOCK);
					break;
				case 'C'://grass center
					tempEnvironmentBlock = EnvironmentBlock::create();
					tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::GRASS_CENTER, SORT_ORDER::ENVIRONMENT_BLOCK);
					break;
				default:
					break;
				}
				break;
			case 'F'://floor, flag
				switch (tempStringVec[0][1])
				{
				case '\0'://floor
					m_floorCount++;
					if (idx < oneBlockLineVec.size() && oneBlockLineVec[idx+1][0] != 'F')
					{
						tempGroundBlock = GroundBlock::create();
						tempGroundBlock->SetBlock(GroundBlock::GROUND_BLOCK_TYPE::FLOOR, SORT_ORDER::GROUND_BLOCK, m_floorCount);
						m_floorCount = 0;
					}
					break;
				case 'L'://flag
					m_flag = Flag::create();
					m_flag->SetBlockPos(Vec2(line * m_oneBlockSize, arrayIdx * m_oneBlockSize));
					this->addChild(m_flag, SORT_ORDER::INTERACTION_BLOCK);
					break;
				default:
					break;
				}
				break;
			case 'Q'://question box
				tempInteractionBlock = InteractionBlock::create();
				switch (tempStringVec[0][1])//item
				{
				case '\0':
					tempInteractionBlock->SetBlock(InteractionBlock::INTERACTION_BLOCK_TYPE::QUESTION_BOX, SORT_ORDER::INTERACTION_BLOCK);
					break;
				case 'C'://coin
				{
					int coinCount = 0;
					//tempStringVec[0][2] coin count
					std::string coinCountStr = &tempStringVec[0][2];
					coinCount = atoi(coinCountStr.c_str());
					tempInteractionBlock->SetBlock(InteractionBlock::INTERACTION_BLOCK_TYPE::QUESTION_BOX,
						SORT_ORDER::INTERACTION_BLOCK);
					tempInteractionBlock->SetItem(InteractionBlock::INTERACTION_BLOCK_ITEM_TYPE::COIN, NULL, coinCount);
				}
				break;
				case 'M'://mushroom
				{
					tempInteractionBlock->SetBlock(InteractionBlock::INTERACTION_BLOCK_TYPE::QUESTION_BOX,
						SORT_ORDER::INTERACTION_BLOCK);
					Mushroom* mushroomTemp = Mushroom::create();
					ItemInfo ItemInfoTemp;
					ItemInfoTemp.item = mushroomTemp;
					for (size_t i = 0; i < 2; i++)
					{
						ItemInfoTemp.adjoinBlock[i].block = NULL;
						ItemInfoTemp.adjoinBlock[i].collRect = Rect::ZERO;
					}
					m_itemInfos.push_back(ItemInfoTemp);
					this->addChild(mushroomTemp, SORT_ORDER::ITEM_STAY);
					tempInteractionBlock->SetItem(InteractionBlock::INTERACTION_BLOCK_ITEM_TYPE::MUSHROOM, mushroomTemp);
				}
					break;
				case 'S'://star
				{
					tempInteractionBlock->SetBlock(InteractionBlock::INTERACTION_BLOCK_TYPE::QUESTION_BOX,
						SORT_ORDER::INTERACTION_BLOCK);
					Star* starTemp = Star::create();
					ItemInfo ItemInfoTemp;
					ItemInfoTemp.item = starTemp;
					for (size_t i = 0; i < 2; i++)
					{
						ItemInfoTemp.adjoinBlock[i].block = NULL;
						ItemInfoTemp.adjoinBlock[i].collRect = Rect::ZERO;
					}
					m_itemInfos.push_back(ItemInfoTemp);
					this->addChild(starTemp, SORT_ORDER::ITEM_STAY);
					tempInteractionBlock->SetItem(InteractionBlock::INTERACTION_BLOCK_ITEM_TYPE::STAR, starTemp);
				}
					break;
				default:
					break;
				}
				break;
			case 'S'://stack
				switch (tempStringVec[0][1])
				{
				case 'L'://stack left
					//tempStringVec[0][2] stacknum
				{
					tempStackBlock = StackBlock::create();
					tempStackBlock->SetBlock(StackBlock::STACK_TYPE::STACK_LEFT, SORT_ORDER::INTERACTION_BLOCK);
				}
					break;
				case 'T'://stack top
					//tempStringVec[0][2] stacknum
				{
					tempStackBlock = StackBlock::create();
					tempStackBlock->SetBlock(StackBlock::STACK_TYPE::STACK_TOP, SORT_ORDER::INTERACTION_BLOCK, m_pillarCount);
					m_pillarCount = 0;
				}
					break;
				case 'P'://stack pillar
					m_pillarCount++;
					break;
				default:
					break;
				}
				break;
			case 'C'://castle, coin, cloud
				switch (tempStringVec[0][1])
				{
				case '\0'://castle
					m_castle = Castle::create();
					tempEnvironmentBlock = m_castle;
					tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::CASTLE, SORT_ORDER::ENVIRONMENT_BLOCK);
					break;
				case 'O'://out coin
					break;
				case 'L'://cloud left
					tempEnvironmentBlock = EnvironmentBlock::create();
					tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::CLOUD_LEFT, SORT_ORDER::ENVIRONMENT_BLOCK);
					break;
				case 'R'://cloud right
					tempEnvironmentBlock = EnvironmentBlock::create();
					tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::CLOUD_RIGHT, SORT_ORDER::ENVIRONMENT_BLOCK);
					break;
				case 'C'://cloud center
					tempEnvironmentBlock = EnvironmentBlock::create();
					tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::CLOUD_CENTER, SORT_ORDER::ENVIRONMENT_BLOCK);
					break;
				case 'T'://cloud top
					tempEnvironmentBlock = EnvironmentBlock::create();
					tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::CLOUD_TOP, SORT_ORDER::ENVIRONMENT_BLOCK);
					break;
				default:
					break;
				}
				break;
			case 'M'://mountain
				tempEnvironmentBlock = EnvironmentBlock::create();
				switch (tempStringVec[0][1])
				{
				case 'L'://mountain left
					tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::MOUNTAIN_LEFT, SORT_ORDER::ENVIRONMENT_BLOCK);
					break;
				case 'R'://mountain right
					tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::MOUNTAIN_RIGHT, SORT_ORDER::ENVIRONMENT_BLOCK);
					break;
				case 'C'://mountain center
					switch (tempStringVec[0][2])
					{
					case '1'://mountain center1
						tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::MOUNTAIN_CENTER1, SORT_ORDER::ENVIRONMENT_BLOCK);
						break;
					case '2'://mountain center2
						tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::MOUNTAIN_CENTER2, SORT_ORDER::ENVIRONMENT_BLOCK);
						break;
					case '3'://mountain center3
						tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::MOUNTAIN_CENTER3, SORT_ORDER::ENVIRONMENT_BLOCK);
						break;
					default:
						break;
					}
					break;
				case 'T'://mountain top
					tempEnvironmentBlock->SetBlock(EnvironmentBlock::ENVIRONMENT_BLOCK_TYPE::MOUNTAIN_TOP, SORT_ORDER::ENVIRONMENT_BLOCK);
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}

			if (tempEnvironmentBlock == NULL && tempGroundBlock == NULL && tempInteractionBlock == NULL && tempStackBlock == NULL)
			{
				tempBlockVec.push_back(NULL);
			}
			else
			{
				if (tempEnvironmentBlock != NULL)
				{
					tempEnvironmentBlock->SetBlockPos(Vec2(line * m_oneBlockSize, arrayIdx * m_oneBlockSize));
					this->addChild(tempEnvironmentBlock, SORT_ORDER::ENVIRONMENT_BLOCK);
					tempBlockVec.push_back(NULL);
				}

				if (tempGroundBlock != NULL)
				{
					tempGroundBlock->SetBlockPos(Vec2(line * m_oneBlockSize, arrayIdx * m_oneBlockSize));
					this->addChild(tempGroundBlock, SORT_ORDER::GROUND_BLOCK);
					tempBlockVec.push_back(tempGroundBlock);
				}

				if (tempInteractionBlock != NULL)
				{
					tempInteractionBlock->SetBlockPos(Vec2(line * m_oneBlockSize, arrayIdx * m_oneBlockSize));
					this->addChild(tempInteractionBlock, SORT_ORDER::INTERACTION_BLOCK);
					tempBlockVec.push_back(tempInteractionBlock);
				}

				if (tempStackBlock != NULL)
				{
					tempStackBlock->SetBlockPos(Vec2(line * m_oneBlockSize, arrayIdx * m_oneBlockSize));
					this->addChild(tempStackBlock, SORT_ORDER::INTERACTION_BLOCK);
					tempBlockVec.push_back(tempStackBlock);
				}
			}

			if (tempStringVec.size() == 2)
			{
				switch (tempStringVec[1][0])//monster
				{
				case 'F'://flower
					break;
				case 'K'://kuri
				{
					Kuri* kuri;
					MonsterInfo monsterInfo;
					monsterInfo.adjoinBlock[0].block = NULL;
					monsterInfo.adjoinBlock[1].block = NULL;
					kuri = Kuri::create();
					monsterInfo.monster = kuri;
					this->addChild(kuri, SORT_ORDER::MONSETER_BLOCK);
					kuri->setPosition(Vec2(line * m_oneBlockSize, arrayIdx * m_oneBlockSize));
					m_monsterInfos.push_back(monsterInfo);
				}
					break;
				case 'N'://nori
				{
					Nori* nori;
					MonsterInfo monsterInfo;
					monsterInfo.adjoinBlock[0].block = NULL;
					monsterInfo.adjoinBlock[1].block = NULL;
					nori = Nori::create();
					monsterInfo.monster = nori;
					this->addChild(nori, SORT_ORDER::MONSETER_BLOCK);
					nori->setPosition(Vec2(line * m_oneBlockSize, arrayIdx * m_oneBlockSize));
					m_monsterInfos.push_back(monsterInfo);
				}
					break;
				default:
					break;
				}
				//tempBlockVec[arrayIdx]->SetBlockPos(Vec2(line * m_oneBlockSize, arrayIdx * m_oneBlockSize));
			}

			arrayIdx++;
			tempGroundBlock = NULL;
			tempEnvironmentBlock = NULL;
			tempInteractionBlock = NULL;
			tempStackBlock = NULL;
			tempStringVec.clear();
		}

		arrayIdx = 0;
		line++;
		m_blocks.push_back(tempBlockVec);
		tempBlockVec.clear();
	}

	m_mapAnchorEndXpos = -(((int)m_blocks.size() - m_visibleBlockWidthCount) * m_oneBlockSize);
	SetCheckCollStartIdx();
	DrawBackground();
}

void MapAnchor::StringSplit(std::string strOrigin, char strTok, std::vector<std::string>* resultStringVec)
{
	int cutAt;

	while ((cutAt = strOrigin.find(strTok)) != strOrigin.npos)
	{
		if (cutAt > 0)
		{
			resultStringVec->push_back(strOrigin.substr(0, cutAt));
		}
		strOrigin = strOrigin.substr(cutAt + 1);
	}

	resultStringVec->push_back(strOrigin.substr(0, cutAt));
}

bool MapAnchor::Fire(Vec2 startPos, int dir)
{
	for (int i = 0; i < MAX_FIRE_BALL_COUNT; i++)
	{
		if (m_fireBallInfos[i].fireBall->Fire(startPos, dir))
		{
			return true;
		}
	}

	return false;
}

void MapAnchor::LoadSavePoint()
{
	this->setPositionX(-6480);
	m_mario->setPositionX(m_mario->getPositionX() + 6440);
	
	for (auto monsterInfo : m_monsterInfos)
	{
		if (monsterInfo.monster->getPositionX() < m_mario->getPositionX())
		{
			monsterInfo.monster->setVisible(false);
		}
	}

	SetCheckCollStartIdx();
}

void MapAnchor::ReleaseObj()
{
	for (auto& blockOneLine : m_blocks)
	{
		for (auto& block : blockOneLine)
		{
			if (block == NULL)
			{
				continue;
			}

			block->ReleaseObj();
		}

	}

	for (auto& monsterInfo : m_monsterInfos)
	{
		monsterInfo.monster->ReleaseObj();
	}
	m_castle->ReleaseObj();
	m_mario->Destroy();
}

MapAnchor::MapAnchor()
{
}

MapAnchor::~MapAnchor()
{
}
