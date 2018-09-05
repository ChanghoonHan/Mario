#include "Mushroom.h"

USING_NS_CC;

bool Mushroom::init()
{
	Item::init();
	m_itemType = ITEM_TYPE::MUSHROOM;
	m_itemSprite = Sprite::create("./Item/Mushroom.png");
	m_itemSprite->setScale(m_scale);
	m_itemSprite->setVisible(false);
	this->addChild(m_itemSprite);
	m_itemSize = Size(60, 80);
	m_xVelocity = 300;
	
	m_score = 1000;
	String scoreStr;
	scoreStr.initWithFormat("%d", m_score);
	m_scoreLabel->setString(scoreStr._string);
	m_scoreLabel->setVisible(false);

	return true;
}

void Mushroom::UpdateItem(float deltaTime)
{
	switch (m_curItemState)
	{
	case Item::STAY:
		break;
	case Item::IDLE:
		if (m_itemType != ITEM_TYPE::MUSHROOM)
		{
			break;
		}

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

void Mushroom::Spawn(InteractionBlock* interBlock)
{
	m_itemSprite->setVisible(true);
	CallFuncN* scoreLabelActionEndCallback = CallFuncN::create(CC_CALLBACK_1(Mushroom::ChangeStateCallback, this, ITEM_STATE::IDLE));
	this->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, 80)), scoreLabelActionEndCallback, NULL));
}

Rect Mushroom::GetCollider()
{
	Vec2 thisPos = this->getPosition();
	m_collider.setRect(thisPos.x - m_itemSize.width/2, thisPos.y - m_itemSize.height/2, m_itemSize.width, m_itemSize.height);

	return m_collider;
}

void Mushroom::ChangeItemToFlower()
{
	m_itemType = ITEM_TYPE::FLOWER;
	m_itemSprite->setTexture("./Item/Flower1.png");

	Animation* flowerAnimation = Animation::create();
	flowerAnimation->addSpriteFrameWithFileName("./Item/Flower1.png");
	flowerAnimation->addSpriteFrameWithFileName("./Item/Flower2.png");
	flowerAnimation->addSpriteFrameWithFileName("./Item/Flower3.png");
	flowerAnimation->addSpriteFrameWithFileName("./Item/Flower4.png");
	flowerAnimation->setDelayPerUnit(0.03f);
	Animate* animate = Animate::create(flowerAnimation);
	m_itemSprite->runAction(RepeatForever::create(animate));

	return;
}

Mushroom::Mushroom()
{
}


Mushroom::~Mushroom()
{
}
