#include "BlockBase.h"

USING_NS_CC;

bool BlockBase::init()
{
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2::ZERO);
	m_collider = Rect(0, 0, 80, 80);
	m_scale = 5.0f;
	m_blockSize = 80;
	m_blockSprites.clear();
	m_curBlockState = BLOCK_STATE::IDLE;

	return true;
}

void BlockBase::SetBlockPos(Vec2 pos)
{
	this->setPosition(pos);
	m_collider = Rect(pos.x, pos.y, m_blockSize, m_blockSize);
}

Rect BlockBase::CalCollRect(Rect rect1, Rect rect2)
{
	float minX = rect1.getMinX() < rect2.getMinX() ? rect2.getMinX() : rect1.getMinX();
	float minY = rect1.getMinY() < rect2.getMinY() ? rect2.getMinY() : rect1.getMinY();
	float maxX = rect1.getMaxX() < rect2.getMaxX() ? rect1.getMaxX() : rect2.getMaxX();
	float maxY = rect1.getMaxY() < rect2.getMaxY() ? rect1.getMaxY() : rect2.getMaxY();

	return Rect(minX, minY, maxX - minX, maxY - minY);
}

bool BlockBase::CheckColl(Rect collider, Rect* collisionRect)
{
	if (m_curBlockState == BLOCK_STATE::DESTROYED)
	{
		return false;
	}

	Vec2 thisPos = this->getPosition();

	if (m_collider.intersectsRect(collider))
	{
		*collisionRect = CalCollRect(m_collider, collider);
		return true;
	}
	else
	{
		*collisionRect = Rect::ZERO;
		return false;
	}
	return false;
}

void BlockBase::SetCollidedItem(Item* item, bool isChangeDir)
{
	
}

void BlockBase::SetCollidedMonster(Monster* monster, bool isChangeDir)
{

}

float BlockBase::GetTopPosY()
{
	return this->getPositionY() + m_collider.size.height;
}

void BlockBase::AttackedFromBelow(bool isBigMario)
{
	return;
}

void BlockBase::LateUpdateBlock(float deltaTime)
{
	
}

void BlockBase::ReleaseObj()
{

}

BlockBase::BlockBase()
{
}

BlockBase::~BlockBase()
{
}
