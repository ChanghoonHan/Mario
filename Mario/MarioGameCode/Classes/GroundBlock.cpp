#include "GroundBlock.h"

USING_NS_CC;

bool GroundBlock::init()
{
	BlockBase::init();
	m_groundType = GROUND_BLOCK_TYPE::NULL_GROUND_BLOCK;

	return true;
}

void GroundBlock::SetBlock(GROUND_BLOCK_TYPE blockType, int sortOrder, int floorCount)
{
	m_sortOrder = sortOrder;
	Sprite* tempSprite = NULL;

	switch (blockType)
	{
	case GroundBlock::NULL_GROUND_BLOCK:
		break;
	case GroundBlock::GROUND:
		tempSprite = Sprite::create("./Interaction/Ground.png");
		m_blockSprites.push_back(tempSprite);
		break;
	case GroundBlock::FLOOR:
		m_floorCount = floorCount;
		for (size_t i = 0; i < floorCount; i++)
		{
			tempSprite = Sprite::create("./Interaction/Floor.png");
			m_blockSprites.push_back(tempSprite);
		}
		break;
	default:
		break;
	}

	for (size_t idx = 0; idx < m_blockSprites.size(); idx++)
	{
		m_blockSprites[idx]->setAnchorPoint(Vec2::ZERO);
		m_blockSprites[idx]->setPosition(Vec2(0, idx * m_blockSize));
		m_blockSprites[idx]->setScale(m_scale);
		this->addChild(m_blockSprites[idx], m_sortOrder);
	}

	m_groundType = blockType;
}

void GroundBlock::SetBlockPos(cocos2d::Vec2 pos)
{
	switch (m_groundType)
	{
	case GroundBlock::NULL_GROUND_BLOCK:
		break;
	case GroundBlock::GROUND:
		BlockBase::SetBlockPos(pos);
		break;
	case GroundBlock::FLOOR:
		pos.y -= m_blockSize * (m_floorCount - 1);
		this->setPosition(pos);
		m_collider.setRect(pos.x, pos.y, m_blockSize, m_blockSize * m_floorCount);
		break;
	default:
		break;
	}
}

GroundBlock::GroundBlock()
{
}


GroundBlock::~GroundBlock()
{
}
