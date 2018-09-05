#include "Star.h"

USING_NS_CC;

bool Star::init()
{
	Item::init();
	m_itemType = ITEM_TYPE::STAR;
	m_itemSprite = Sprite::create("./Item/Star1.png");
	m_itemSprite->setScale(m_scale);
	m_itemSprite->setVisible(false);
	this->addChild(m_itemSprite);
	m_itemSize = Size(60, 80);

	Animation* starAnimation = Animation::create();
	starAnimation->addSpriteFrameWithFileName("./Item/Star1.png");
	starAnimation->addSpriteFrameWithFileName("./Item/Star2.png");
	starAnimation->addSpriteFrameWithFileName("./Item/Star3.png");
	starAnimation->addSpriteFrameWithFileName("./Item/Star4.png");
	starAnimation->setDelayPerUnit(0.03f);
	Animate* starAnimate = Animate::create(starAnimation);
	m_itemSprite->runAction(RepeatForever::create(starAnimate));

	m_xVelocity = 300;
	m_minYVelocity = -800;
	m_yVelocity = 0;
	m_gravity = -3200;
	m_jumpVelocity = 1000;

	m_score = 1000;
	String scoreStr;
	scoreStr.initWithFormat("%d", m_score);
	m_scoreLabel->setString(scoreStr._string);
	m_scoreLabel->setVisible(false);

	return true;
}

void Star::UpdateItem(float deltaTime)
{
	switch (m_curItemState)
	{
	case Item::STAY:
		break;
	case Item::IDLE:
		m_yVelocity += m_gravity * deltaTime;
		if (m_yVelocity < m_minYVelocity)
		{
			m_yVelocity = m_minYVelocity;
		}

		this->setPositionX(this->getPositionX() + m_xVelocity * deltaTime);
		this->setPositionY(this->getPositionY() + m_yVelocity * deltaTime);
		if (this->getPositionY() < -100)
		{
			ChangeState(Item::DESTROYED);
		}
		break;
	case Item::DESTROYED:
		break;
	default:
		break;
	}
}

void Star::Spawn(InteractionBlock* interBlock)
{
	m_itemSprite->setVisible(true);
	CallFuncN* scoreLabelActionEndCallback = CallFuncN::create(CC_CALLBACK_1(Star::ChangeStateCallback, this, ITEM_STATE::IDLE));
	this->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, 90)), scoreLabelActionEndCallback, NULL));
}

Rect Star::GetCollider()
{
	Vec2 thisPos = this->getPosition();
	m_collider.setRect(thisPos.x - m_itemSize.width / 2, thisPos.y - m_itemSize.height / 2, m_itemSize.width, m_itemSize.height);

	return m_collider;
}

void Star::SetYPositionBecauseColl(float deltaY)
{
	if (m_curItemState != ITEM_STATE::IDLE)
	{
		return;
	}

	this->setPositionY(this->getPositionY() + deltaY);
	if (0 < deltaY)
	{
		SetJump();
	}
}


Star::Star()
{
}


Star::~Star()
{
}
