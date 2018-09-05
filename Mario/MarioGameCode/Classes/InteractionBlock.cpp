#include "InteractionBlock.h"
#include "ScoreBoard.h"
#include "Monster.h"

USING_NS_CC;

bool InteractionBlock::init()
{
	BlockBase::init();

	m_collidedMonster = NULL;
	m_collidedItem = NULL;
	m_isCollidedMonsterChangeDir = false;
	m_isCollidedItemChangeDir = false;

	EaseIn* upMoveAction = EaseIn::create(MoveBy::create(0.15f, Vec2(0, 50)), 0.7f);
	EaseIn* downMoveAction = EaseIn::create(MoveBy::create(0.10f, Vec2(0, -50)), 0.7f);
	CallFuncN* endAttackAction = CallFuncN::create(CC_CALLBACK_1(InteractionBlock::EndAttackAction, this));
	m_attackedAction = Sequence::create(upMoveAction, downMoveAction, NULL);
	m_attackedActionWithEndCallBack = Sequence::create(upMoveAction, downMoveAction, endAttackAction, NULL);
	m_backgroundColor = Color4F(145 / 255.0f, 143 / 255.0f, 254 / 255.0f, 1.0f);
	/////if die delete list
	m_attackedAction->retain();
	m_attackedActionWithEndCallBack->retain();

	m_interType = INTERACTION_BLOCK_TYPE::NULL_INTERACTION_BLOCK;

	return true;
}
void InteractionBlock::SetBlock(INTERACTION_BLOCK_TYPE blockType, int sortOrder)
{
	m_sortOrder = sortOrder;
	Sprite* tempSprite = NULL;

	m_background = DrawNode::create();
	m_background->drawSolidRect(Vec2::ZERO, Vec2(m_blockSize, m_blockSize), m_backgroundColor);
	this->addChild(m_background, sortOrder);

	switch (blockType)
	{
	case InteractionBlock::NULL_INTERACTION_BLOCK:
		break;
	case InteractionBlock::BLOCK:
		tempSprite = Sprite::create("./Interaction/Block.png");
		break;
	case InteractionBlock::QUESTION_BOX:
	{
		tempSprite = Sprite::create("./Interaction/QuestionBox1.png");
		Animation* questionBoxAnimation = Animation::create();
		questionBoxAnimation->addSpriteFrameWithFileName("./Interaction/QuestionBox3.png");
		questionBoxAnimation->addSpriteFrameWithFileName("./Interaction/QuestionBox2.png");
		questionBoxAnimation->addSpriteFrameWithFileName("./Interaction/QuestionBox1.png");
		questionBoxAnimation->addSpriteFrameWithFileName("./Interaction/QuestionBox1.png");
		questionBoxAnimation->addSpriteFrameWithFileName("./Interaction/QuestionBox2.png");
		questionBoxAnimation->setDelayPerUnit(0.15f);
		Animate* questionBoxAnimate = Animate::create(questionBoxAnimation);
		tempSprite->runAction(RepeatForever::create(questionBoxAnimate));
	}
		break;
	case InteractionBlock::FAKE_QUESTION_BOX:
		tempSprite = Sprite::create("./Interaction/Block.png");
		break;
	default:
		break;
	}

	m_interType = blockType;

	m_blockSprites.push_back(tempSprite);
	tempSprite->setScale(m_scale);
	tempSprite->setAnchorPoint(Vec2::ZERO);
	tempSprite->setPosition(Vec2::ZERO);
	this->addChild(tempSprite, m_sortOrder);
}

void InteractionBlock::SetItem(INTERACTION_BLOCK_ITEM_TYPE itemType, Item* item, int coinCount)
{
	m_innerItemType = itemType;
	m_coinCount = coinCount;
	m_inItemsIdx = 0;

	if (m_coinCount > 0)
	{
		for (size_t i = 0; i < m_coinCount; i++)
		{
			InCoin* inCoinTemp = InCoin::create();
			this->addChild(inCoinTemp);
			m_inItems.push_back(inCoinTemp);
		}
	}
	else
	{
		m_inItems.push_back(item);
	}
}

void InteractionBlock::AttackedFromBelow(bool isBigMario)
{
	if (m_curBlockState != BLOCK_STATE::IDLE)
	{
		return;
	}

	SetJumpCollidedObj();

	m_curBlockState = BLOCK_STATE::ATTACKED;
	switch (m_interType)
	{
	case InteractionBlock::NULL_INTERACTION_BLOCK:
		break;
	case InteractionBlock::BLOCK:
		if (isBigMario)
		{
			InstatiateBlockChip();
			m_background->setVisible(false);
		}
		else
		{
			m_blockSprites[0]->setTexture("./Interaction/BlockHit.png");
			m_blockSprites[0]->runAction(m_attackedActionWithEndCallBack);
		}
		break;
	case InteractionBlock::FAKE_QUESTION_BOX:
		m_blockSprites[0]->setTexture("./Interaction/BlockHit.png");

	case InteractionBlock::QUESTION_BOX:

		switch (m_innerItemType)
		{
		case INTERACTION_BLOCK_ITEM_TYPE::NULL_ITEM:
			break;
		case INTERACTION_BLOCK_ITEM_TYPE::COIN:
		{
			this->setZOrder(this->getZOrder() + 1);
			m_inItems[m_inItemsIdx]->Spawn(this);
			m_inItemsIdx++;
			m_coinCount--;
		}
			break;
		case INTERACTION_BLOCK_ITEM_TYPE::MUSHROOM:
			if (isBigMario)
			{
				m_inItems[m_inItemsIdx]->ChangeItemToFlower();
			}

		case INTERACTION_BLOCK_ITEM_TYPE::STAR:
			if (m_collidedItem != NULL)
			{
				if (m_isCollidedItemChangeDir)
				{
					m_collidedItem->ChangeMoveDir();
				}
				m_collidedItem->SetJump();
			}
			m_inItems[m_inItemsIdx]->Spawn();
			break;
		default:
			break;
		}

		if (m_coinCount > 0)
		{
			m_blockSprites[0]->runAction(m_attackedActionWithEndCallBack);
		}
		else
		{
			m_blockSprites[0]->stopAllActions();
			m_blockSprites[0]->setTexture("./Interaction/EmptyQuestionBox.png");
			m_blockSprites[0]->runAction(m_attackedAction);
		}
		break;
	default:
		break;
	}

}

void InteractionBlock::EndAttackAction(Node* obj)
{
	switch (m_interType)
	{
	case InteractionBlock::NULL_INTERACTION_BLOCK:
		break;
	case InteractionBlock::BLOCK:
		m_blockSprites[0]->setTexture("./Interaction/Block.png");
		break;
	case InteractionBlock::QUESTION_BOX:
		break;
	case InteractionBlock::FAKE_QUESTION_BOX:
		m_blockSprites[0]->setTexture("./Interaction/Block.png");
		break;
	default:
		break;
	}

	m_curBlockState = BLOCK_STATE::IDLE;
}

void InteractionBlock::InstatiateBlockChip()
{
	ScoreBoard::GetInstance()->IncreaseScore(50);

	String path = "";
	this->removeChild(m_blockSprites[0]);
	this->setPositionY(this->getPositionY() + 50);
	Sprite* blockChipSpriteTemp[4];

	Animation* blockChipAnimation;
	Animate* blockChipAnimate;
	this->setZOrder(m_sortOrder + 1);

	for (size_t i = 0; i < 4; i++)
	{
		blockChipSpriteTemp[i] = Sprite::create("./Interaction/BlockChip1.png");
		blockChipSpriteTemp[i]->setScale(m_scale);
		this->addChild(blockChipSpriteTemp[i]);

		blockChipAnimation = Animation::create();
		for (size_t j = 0; j < 4; j++)//add frame
		{
			path.initWithFormat("./Interaction/BlockChip%d.png", j + 1);
			blockChipAnimation->addSpriteFrameWithFileName(path._string);
		}
		blockChipAnimation->setDelayPerUnit(0.10f);
		blockChipAnimate = Animate::create(blockChipAnimation);
		blockChipSpriteTemp[i]->runAction(RepeatForever::create(blockChipAnimate));
		m_blockSprites.push_back(blockChipSpriteTemp[i]);
	}


	blockChipSpriteTemp[0]->setPosition(Vec2(20, 60));
	blockChipSpriteTemp[0]->runAction(JumpBy::create(0.8f, Vec2(blockChipSpriteTemp[0]->getPositionX() - 280, -this->getPositionY() - 50), 240 + (this->getPositionY() - 50) / 2, 1));
	blockChipSpriteTemp[1]->setPosition(Vec2(60, 60));
	blockChipSpriteTemp[1]->runAction(JumpBy::create(0.8f, Vec2(blockChipSpriteTemp[0]->getPositionX() + 280, -this->getPositionY() - 50), 240 + (this->getPositionY() - 50) /2, 1));
	blockChipSpriteTemp[2]->setPosition(Vec2(20, 20));
	blockChipSpriteTemp[2]->runAction(JumpBy::create(0.6f, Vec2(blockChipSpriteTemp[0]->getPositionX() - 160, -this->getPositionY()), 80 + this->getPositionY() / 2, 1));
	blockChipSpriteTemp[3]->setPosition(Vec2(60, 20));
	blockChipSpriteTemp[3]->runAction(JumpBy::create(0.6f, Vec2(blockChipSpriteTemp[0]->getPositionX() + 160, -this->getPositionY()), 80 + this->getPositionY() / 2, 1));

	m_curBlockState = BLOCK_STATE::DESTROYED;
}

void InteractionBlock::SetBlockPos(cocos2d::Vec2 pos)
{
	BlockBase::SetBlockPos(pos);
	if (m_innerItemType == INTERACTION_BLOCK_ITEM_TYPE::COIN)
	{
		for (auto item : m_inItems)
		{
			item->setPosition(Vec2(m_blockSize / 2, m_blockSize / 2));
		}
	}
	else
	{
		for (auto item : m_inItems)
		{
			item->setPosition(Vec2(pos.x + m_blockSize / 2, pos.y + m_blockSize / 2));
		}
	}
}

void InteractionBlock::SetCollidedMonster(Monster* monster, bool isChangeDir)
{
	m_isCollidedMonsterChangeDir = isChangeDir;
	m_collidedMonster = monster;
	BlockBase::SetCollidedMonster(monster, isChangeDir);
}

void InteractionBlock::SetCollidedItem(Item* item, bool isChangeDir)
{
	m_isCollidedItemChangeDir = isChangeDir;
	m_collidedItem = item;
	BlockBase::SetCollidedItem(item, isChangeDir);
}

void InteractionBlock::LateUpdateBlock(float deltaTime)
{
	m_isCollidedMonsterChangeDir = false;
	m_collidedMonster = NULL;
	m_isCollidedItemChangeDir = false;
	m_collidedItem = NULL;
	BlockBase::LateUpdateBlock(deltaTime);
}

void InteractionBlock::SetBackZOrder()
{
	this->setZOrder(this->getZOrder() - 1);
}

void InteractionBlock::SetJumpCollidedObj()
{
	if (m_collidedItem != NULL)
	{
		if (m_isCollidedItemChangeDir)
		{
			m_collidedItem->ChangeMoveDir();
		}
		m_collidedItem->SetJump();
	}

	if (m_collidedMonster != NULL)
	{
		m_collidedMonster->SetInstantaneousDestroy(m_isCollidedItemChangeDir);
	}
}

void InteractionBlock::ReleaseObj()
{
	m_attackedAction->release();
	m_attackedActionWithEndCallBack->release();
}

InteractionBlock::InteractionBlock()
{
}


InteractionBlock::~InteractionBlock()
{
}
