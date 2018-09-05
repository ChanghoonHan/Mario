#include "StackBlock.h"

USING_NS_CC;

bool StackBlock::init()
{
	BlockBase::init();
	m_stackType = STACK_TYPE::NULL_STACK;

	return true;
}

void StackBlock::SetBlock(STACK_TYPE stackType, int sortOrder, int pillarCount)
{
	m_sortOrder = sortOrder;
	Sprite* tempSprite = NULL;
	m_pillarCount = pillarCount;

	switch (stackType)
	{
	case STACK_TYPE::NULL_STACK:
		break;
	case STACK_TYPE::STACK_LEFT:
		tempSprite = Sprite::create("./Interaction/StackLeft.png");
		m_blockSprites.push_back(tempSprite);
		break;
	case STACK_TYPE::STACK_TOP:
		for (size_t i = 0; i < m_pillarCount; i++)
		{
			tempSprite = Sprite::create("./Interaction/StackPillar.png");
			m_blockSprites.push_back(tempSprite);
		}
		tempSprite = Sprite::create("./Interaction/StackTop.png");
		m_blockSprites.push_back(tempSprite);
		break;
	default:
		break;
	}

	for (int idx = 0; idx < m_blockSprites.size(); idx++)
	{
		m_blockSprites[idx]->setAnchorPoint(Vec2::ZERO);
		m_blockSprites[idx]->setPosition(Vec2(0, idx * m_blockSize));
		m_blockSprites[idx]->setScale(m_scale);
		this->addChild(m_blockSprites[idx], m_sortOrder);
	}

	m_stackType = stackType;
}

void StackBlock::SetBlockPos(cocos2d::Vec2 pos)
{
	switch (m_stackType)
	{
	case STACK_TYPE::NULL_STACK:
		break;
	case STACK_TYPE::STACK_LEFT:
		BlockBase::SetBlockPos(pos);
		m_collider.setRect(pos.x, pos.y, m_blockSize * 3, m_blockSize * 2);
		break;
	case STACK_TYPE::STACK_TOP:
		pos.y -= m_blockSize * m_pillarCount;
		this->setPosition(pos);
		m_collider.setRect(pos.x, pos.y, m_blockSize * 2, m_blockSize * (m_pillarCount + 1));
		break;
	default:
		break;
	}
}

StackBlock::StackBlock()
{
}


StackBlock::~StackBlock()
{
}
