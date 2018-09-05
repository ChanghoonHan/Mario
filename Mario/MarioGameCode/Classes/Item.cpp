#include "Item.h"
#include "MapAnchor.h"
#include "ScoreBoard.h"

USING_NS_CC;

bool Item::init()
{
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2::ZERO);
	m_itemSize = Size::ZERO;
	m_xVelocity = 300;
	m_minYVelocity = -800;
	m_yVelocity = 0;
	m_gravity = -3200;
	m_jumpVelocity = 1000;
	m_itemType = ITEM_TYPE::NULL_TYPE;
	m_collider = Rect(0, 0, 80, 80);
	m_scale = 5.0f;
	ChangeState(ITEM_STATE::STAY);
	m_labelPath = "./fonts/Super Mario Bros. NES.ttf";
	m_labelSize = 20;
	m_scoreLabel = LabelTTF::create("", m_labelPath, m_labelSize);

	this->addChild(m_scoreLabel);

	m_score = 0;

	return true;
}

bool Item::CheckColl(Rect collider)
{
	if (m_curItemState != ITEM_STATE::IDLE)
	{
		return false;
	}

	Vec2 thisPos = this->getPosition();

	m_collider.setRect(thisPos.x, thisPos.y, m_colliderSize.width, m_colliderSize.height);

	if (m_collider.intersectsRect(collider))
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

void Item::ChangeMoveDir()
{
	m_xVelocity *= -1;
}

void Item::DestroyCallback(Node* obj)
{
	ChangeState(ITEM_STATE::DESTROY);
	ScoreBoard::GetInstance()->IncreaseScore(m_score);
	CallFuncN* destroyActionEndCallback = CallFuncN::create(CC_CALLBACK_1(Item::ChangeStateCallback, this, ITEM_STATE::DESTROYED));
	m_scoreLabel->runAction(Sequence::create(MoveBy::create(0.7f, Vec2(0, 100)), destroyActionEndCallback, NULL));
}

void Item::ChangeStateCallback(Node* obj, ITEM_STATE itemState)
{
	ChangeState(itemState);
};

void Item::Spawn(InteractionBlock* interBlock)
{
	return;
}

void Item::SetJump()
{
	m_yVelocity = m_jumpVelocity;
}

Item::ITEM_TYPE Item::GetItemType()
{
	return m_itemType;
}

void Item::ChangeState(ITEM_STATE itemState)
{
	m_curItemState = itemState;

	switch (m_curItemState)
	{
	case Item::STAY:
		break;
	case Item::IDLE:
		this->setZOrder(MapAnchor::SORT_ORDER::ITEM_IDLE);
		break;
	case Item::DESTROY:
		m_itemSprite->setVisible(false);
		m_scoreLabel->setVisible(true);
		m_scoreLabel->setPosition(Vec2(20, 20));
		break;
	case Item::DESTROYED:
		m_itemSprite->setVisible(false);
		m_scoreLabel->setVisible(false);
		break;
	default:
		break;
	}
}

void Item::UpdateItem(float deltaTime)
{

}

void Item::SetYPositionBecauseColl(float deltaY)
{
	if (m_curItemState != ITEM_STATE::IDLE)
	{
		return;
	}

	m_yVelocity = 0;

	this->setPositionY(this->getPositionY() + deltaY);
}

void Item::SetXPositionBecauseColl(float deltaX)
{
	if (m_curItemState != ITEM_STATE::IDLE)
	{
		return;
	}

	ChangeMoveDir();
	this->setPositionX(this->getPositionX() + deltaX);
}

bool Item::CheckCollWithMario(Rect marioCollider, ITEM_TYPE* itemType)
{
	if (m_curItemState != ITEM_STATE::IDLE)
	{
		return false;
	}

	GetCollider();//set m_collider;
	if (m_collider.intersectsRect(marioCollider))
	{
		DestroyCallback(this);
		*itemType = m_itemType;
		return true;
	}
	else
	{
		*itemType = ITEM_TYPE::NULL_TYPE;
		return false;
	}

	return false;
}

void Item::ChangeItemToFlower()
{
	return;
}

Rect Item::GetCollider()
{
	return Rect::ZERO;
}

Item::Item()
{
}

Item::~Item()
{
}
