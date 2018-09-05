#include "Block.h"

bool Block::init()
{
	m_blockStr = "";
	this->setAnchorPoint(Vec2::ZERO);
	m_scale = 5.0f;
	this->setScale(m_scale);
	m_blockSprites.clear();
	m_itemSprite = NULL;
	m_monsterSprite = NULL;
	m_infoLabel = Label::createWithSystemFont("0 false", "", 10, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	m_infoLabel->setPosition(Vec2(19, 8));
	m_infoLabel->setVisible(false);
	m_infoLabel->setColor(Color3B::BLACK);
	this->addChild(m_infoLabel, 3);
	m_curBlockType = BLOCK_TYPE::NULL_BLOCK;
	m_curItemType = ITEM_TYPE::NULL_ITEM;
	m_curMonsterType = MONSTER_TYPE::NULL_MONSTER;
	m_stackNum = 0;
	m_sortOrder = 0;
	m_isFollowMouseBlock = false;
	m_isEntranceStack = false;
	return true;
}

void Block::SetBlockType(BLOCK_TYPE blockType, int stackNum, bool isEntranceStack)
{
	if (m_monsterSprite != NULL)
	{
		this->removeChild(m_monsterSprite);
		m_monsterSprite = NULL;
		m_curMonsterType = MONSTER_TYPE::NULL_MONSTER;
	}
	
	if (m_itemSprite != NULL)
	{
		this->removeChild(m_itemSprite);
		m_itemSprite = NULL;
		m_curItemType = ITEM_TYPE::NULL_ITEM;
		m_infoLabel->setVisible(false);
	}

	if (stackNum != 0)
	{
		m_stackNum = stackNum;
		m_isEntranceStack = isEntranceStack;
	}
	else
	{
		m_stackNum = 0;
		m_isEntranceStack = false;
	}

	SetStackLabel();

	for (auto blockSprite : m_blockSprites)
	{
		this->removeChild(blockSprite);
	}
	m_blockSprites.clear();

	Sprite* blockSpriteTemp;

	switch (blockType)
	{
	case BLOCK_TYPE::BLOCK:
		blockSpriteTemp = Sprite::create("./Enviroment/Block.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 2;
		break;
	case BLOCK_TYPE::GROUND:
		blockSpriteTemp = Sprite::create("./Enviroment/Ground.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 2;
		break;
	case BLOCK_TYPE::FLOOR:
		blockSpriteTemp = Sprite::create("./Enviroment/Floor.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 2;
		break;
	case BLOCK_TYPE::QUESTION:
		blockSpriteTemp = Sprite::create("./Enviroment/EmptyQuestionBox.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 2;
		break;
	case BLOCK_TYPE::STACK_LEFT:
		m_stackNum = stackNum;
		m_isEntranceStack = isEntranceStack;
		blockSpriteTemp = Sprite::create("./Enviroment/StackLeft.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 2;
		break;
	case BLOCK_TYPE::STACK_TOP:
		m_stackNum = stackNum;
		m_isEntranceStack = isEntranceStack;
		blockSpriteTemp = Sprite::create("./Enviroment/StackTop.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 2;
		break;
	case BLOCK_TYPE::STACK_PILLAR:
		blockSpriteTemp = Sprite::create("./Enviroment/StackPillar.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 1;
		break;
	case BLOCK_TYPE::FLAG:
		blockSpriteTemp = Sprite::create("./Enviroment/Floor.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);

		for (size_t i = 0; i < 9; i++)
		{
			blockSpriteTemp = Sprite::create("./Enviroment/FlagPillar.png");
			blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
			blockSpriteTemp->setPosition(Vec2(7, 16 * (i + 1)));
			m_blockSprites.push_back(blockSpriteTemp);
		}

		blockSpriteTemp = Sprite::create("./Enviroment/FlagTop.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2(4, (16) * 10));
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 2;
		break;
	case BLOCK_TYPE::COIN_OUT:
		blockSpriteTemp = Sprite::create("./Enviroment/CoinOut1.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2(3, 2));
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 2;
		break;
	case BLOCK_TYPE::GRASS_LEFT:
		blockSpriteTemp = Sprite::create("./Enviroment/GrassLeft.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::GRASS_RIGHT:
		blockSpriteTemp = Sprite::create("./Enviroment/GrassRight.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::GRASS_CENTER:
		blockSpriteTemp = Sprite::create("./Enviroment/GrassCenter.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::CLOUD_LEFT:
		blockSpriteTemp = Sprite::create("./Enviroment/CloudLeft.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::CLOUD_RIGHT:
		blockSpriteTemp = Sprite::create("./Enviroment/CloudRight.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::CLOUD_CENTER:
		blockSpriteTemp = Sprite::create("./Enviroment/CloudCenter.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::CLOUD_TOP:
		blockSpriteTemp = Sprite::create("./Enviroment/CloudTop.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::MOUNTAIN_LEFT:
		blockSpriteTemp = Sprite::create("./Enviroment/MountainLeft.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::MOUNTAIN_RIGHT:
		blockSpriteTemp = Sprite::create("./Enviroment/MountainRight.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::MOUNTAIN_CENTER1:
		blockSpriteTemp = Sprite::create("./Enviroment/MountainCenter1.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::MOUNTAIN_CENTER2:
		blockSpriteTemp = Sprite::create("./Enviroment/MountainCenter2.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::MOUNTAIN_CENTER3:
		blockSpriteTemp = Sprite::create("./Enviroment/MountainCenter3.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case BLOCK_TYPE::MOUNTAIN_TOP:
		blockSpriteTemp = Sprite::create("./Enviroment/MountainTop.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	case CASTLE:
		blockSpriteTemp = Sprite::create("./Enviroment/Castle.png");
		blockSpriteTemp->setAnchorPoint(Vec2::ZERO);
		blockSpriteTemp->setPosition(Vec2::ZERO);
		m_blockSprites.push_back(blockSpriteTemp);
		m_sortOrder = 0;
		break;
	default:
		break;
	}


	for (auto blockSprite : m_blockSprites)
	{
		this->addChild(blockSprite, 0);
		this->setZOrder(m_sortOrder);
	}

	m_curBlockType = blockType;
}

void Block::SetItemType(ITEM_TYPE itemType)
{
	if (m_isFollowMouseBlock)
	{
		for (auto blockSprite : m_blockSprites)
		{
			this->removeChild(blockSprite);
		}
		m_blockSprites.clear();
		m_curBlockType = BLOCK_TYPE::NULL_BLOCK;

		if (m_monsterSprite != NULL)
		{
			this->removeChild(m_monsterSprite);
			m_monsterSprite = NULL;
			m_curMonsterType = MONSTER_TYPE::NULL_MONSTER;
		}
	}
	else
	{
		if (m_curItemType == itemType || (m_curBlockType != BLOCK_TYPE::QUESTION && m_curBlockType != BLOCK_TYPE::BLOCK))
		{
			if (itemType == ITEM_TYPE::COIN_IN)
			{
				m_coinCount++;
				String coinCountCocosStr = "";
				coinCountCocosStr.initWithFormat("%d", m_coinCount);
				m_infoLabel->setString(coinCountCocosStr._string);
			}

			return;
		}
	}

	m_coinCount = 0;
	if (itemType == ITEM_TYPE::COIN_IN)
	{
		m_coinCount++;
		m_infoLabel->setVisible(true);
		String coinCountCocosStr = "";
		coinCountCocosStr.initWithFormat("%d", m_coinCount);
		m_infoLabel->setString(coinCountCocosStr._string);
		m_infoLabel->setPosition(Vec2(8, 8));
	}
	else
	{
		m_infoLabel->setVisible(false);
	}

	if (m_itemSprite != NULL)
	{
		this->removeChild(m_itemSprite);
		m_itemSprite = NULL;
	}

	switch (itemType)
	{
	case NULL_ITEM:
		break;
	case COIN_IN:
		m_itemSprite = Sprite::create("./Enviroment/CoinIn1.png");
		m_itemSprite->setAnchorPoint(Vec2::ZERO);
		m_itemSprite->setPosition(Vec2(4, 1));
		break;
	case MUSHROOM:
		m_itemSprite = Sprite::create("./Enviroment/Mushroom.png");
		m_itemSprite->setAnchorPoint(Vec2::ZERO);
		m_itemSprite->setPosition(Vec2::ZERO);
		break;
	case STAR:
		m_itemSprite = Sprite::create("./Enviroment/Star1.png");
		m_itemSprite->setAnchorPoint(Vec2::ZERO);
		m_itemSprite->setPosition(Vec2(1, 0));
		break;
	default:
		break;
	}

	this->addChild(m_itemSprite, 1);
	m_curItemType = itemType;
}

void Block::GetAllType(BLOCK_TYPE* blockType, ITEM_TYPE* itemType, MONSTER_TYPE* monsterType)
{
	*blockType = m_curBlockType;
	*itemType = m_curItemType;
	*monsterType = m_curMonsterType;
}

BLOCK_TYPE Block::getBlockType()
{
	return m_curBlockType;
}

ITEM_TYPE Block::getItemType()
{
	return m_curItemType;
}

MONSTER_TYPE Block::getMosnterType()
{
	return m_curMonsterType;
}

void Block::SetFollowMouseBlock()
{
	m_isFollowMouseBlock = true;
}

void Block::IncreaseStackNum()
{
	if (m_curBlockType != BLOCK_TYPE::STACK_TOP && m_curBlockType != BLOCK_TYPE::STACK_LEFT)
	{
		return;
	}

	m_stackNum++;
	if (m_stackNum > 1000)
	{
		m_stackNum = 1000;
	}

	SetStackLabel();
}

void Block::DecreaseStackNum()
{
	if (m_curBlockType != BLOCK_TYPE::STACK_TOP && m_curBlockType != BLOCK_TYPE::STACK_LEFT)
	{
		return;
	}

	m_stackNum--;
	if (m_stackNum < 0)
	{
		m_stackNum = 0;
	}

	SetStackLabel();
}

void Block::SetStackEntrance()
{
	if (m_curBlockType != BLOCK_TYPE::STACK_TOP && m_curBlockType != BLOCK_TYPE::STACK_LEFT)
	{
		return;
	}

	m_isEntranceStack = !m_isEntranceStack;

	SetStackLabel();
}

void Block::GetStackNumAndIsEntrance(int* stackNum, bool* isEntrance)
{
	*stackNum = m_stackNum;
	*isEntrance = m_isEntranceStack;
}

void Block::SetStackLabel()
{
	if (m_stackNum != 0)
	{
		std::string stackLabelStr = "";
		String stackLabelCocosStr = "";
		stackLabelCocosStr.initWithFormat("%d", m_stackNum);

		if (m_isEntranceStack)
		{
			stackLabelStr = stackLabelCocosStr._string + "I";
		}
		else
		{
			stackLabelStr = stackLabelCocosStr._string + "O";
		}

		m_infoLabel->setString(stackLabelStr);
		m_infoLabel->setVisible(true);
	}
	else
	{
		m_infoLabel->setVisible(false);
	}
}

void Block::SetMonsterType(MONSTER_TYPE monsterType)
{
	if (m_isFollowMouseBlock)
	{
		for (auto blockSprite : m_blockSprites)
		{
			this->removeChild(blockSprite);
		}
		m_blockSprites.clear();
		m_curBlockType = BLOCK_TYPE::NULL_BLOCK;

		if (m_itemSprite != NULL)
		{
			this->removeChild(m_itemSprite);
			m_itemSprite = NULL;
			m_curItemType = ITEM_TYPE::NULL_ITEM;
		}

		m_infoLabel->setVisible(false);
	}
	else
	{
		if (m_curBlockType != BLOCK_TYPE::NULL_BLOCK && m_curBlockType < COIN_OUT)
		{
			return;
		}
	}

	if (m_curMonsterType == MONSTER_TYPE::NULL_MONSTER)
	{
		m_monsterSprite = Sprite::create();
		m_monsterSprite->setAnchorPoint(Vec2(0, 0));
		m_monsterSprite->setPosition(Vec2(0, 0));
		this->addChild(m_monsterSprite);
	}

	switch (monsterType)
	{
	case MONSTER_TYPE::FLOWER:
		m_monsterSprite->setPosition(Vec2(8, 0));
		m_monsterSprite->setTexture("./Enviroment/Flower1.png");
		break;
	case MONSTER_TYPE::KURI:
		m_monsterSprite->setPosition(Vec2(0, 0));
		m_monsterSprite->setTexture("./Enviroment/KuriIdle1.png");
		break;
	case MONSTER_TYPE::NORI:
		m_monsterSprite->setPosition(Vec2(0, 0));
		m_monsterSprite->setTexture("./Enviroment/NoriIdle1.png");
		break;
	default:
		break;
	}

	m_curMonsterType = monsterType;
}

std::string Block::GetBlockString()
{
	std::string blockString = "";

	switch (m_curBlockType)
	{
	case NULL_BLOCK:
		break;
	case BLOCK:
		blockString += "B";
		switch (m_curItemType)
		{
		case NULL_ITEM:
			break;
		case COIN_IN:
		{
			String coinCountCocosStr = "";
			coinCountCocosStr.initWithFormat("%d", m_coinCount);
			m_infoLabel->setString(coinCountCocosStr._string);
			blockString += "C" + coinCountCocosStr._string;
		}
			break;
		case MUSHROOM:
			blockString += "M";
			break;
		case STAR:
			blockString += "S";
			break;
		default:
			break;
		}
		break;
	case GROUND:
		blockString += "G";
		break;
	case FLOOR:
		blockString += "F";
		break;
	case QUESTION:
		blockString += "Q";
		switch (m_curItemType)
		{
		case NULL_ITEM:
			break;
		case COIN_IN:
			{
				String coinCountCocosStr = "";
				coinCountCocosStr.initWithFormat("%d", m_coinCount);
				m_infoLabel->setString(coinCountCocosStr._string);
				blockString += "C" + coinCountCocosStr._string;
			}
			break;
		case MUSHROOM:
			blockString += "M";
			break;
		case STAR:
			blockString += "S";
			break;
		default:
			break;
		}
		break;
	case STACK_LEFT:
		blockString += "SL";
		if (m_stackNum != 0)
		{
			blockString += m_infoLabel->getString();
		}
		break;
	case STACK_TOP:
		blockString += "ST";
		if (m_stackNum != 0)
		{
			blockString += m_infoLabel->getString();
		}
		break;
	case STACK_PILLAR:
		blockString += "SP";
		break;
	case FLAG:
		blockString += "FL";
		break;
	case COIN_OUT:
		blockString += "CO";
		break;
	case GRASS_LEFT:
		blockString += "GL";
		break;
	case GRASS_RIGHT:
		blockString += "GR";
		break;
	case GRASS_CENTER:
		blockString += "GC";
		break;
	case CLOUD_LEFT:
		blockString += "CL";
		break;
	case CLOUD_RIGHT:
		blockString += "CR";
		break;
	case CLOUD_CENTER:
		blockString += "CC";
		break;
	case CLOUD_TOP:
		blockString += "CT";
		break;
	case MOUNTAIN_LEFT:
		blockString += "ML";
		break;
	case MOUNTAIN_RIGHT:
		blockString += "MR";
		break;
	case MOUNTAIN_CENTER1:
		blockString += "MC1";
		break;
	case MOUNTAIN_CENTER2:
		blockString += "MC2";
		break;
	case MOUNTAIN_CENTER3:
		blockString += "MC3";
		break;
	case MOUNTAIN_TOP:
		blockString += "MT";
		break;
	case CASTLE:
		blockString += "C";
		break;
	default:
		break;
	}

	switch (m_curMonsterType)
	{
	case NULL_MONSTER:
		break;
	case FLOWER:
		blockString += "_F";
		break;
	case KURI:
		blockString += "_K";
		break;
	case NORI:
		blockString += "_N";
		break;
	default:
		break;
	}

	blockString += " ";
	return blockString;
}

Block::Block()
{
}


Block::~Block()
{
}
