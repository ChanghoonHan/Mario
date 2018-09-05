#include "FireBall.h"

USING_NS_CC;

bool FireBall::init()
{
	m_curState = FIRE_BALL_STATE::STAY;
	m_fireBallSize.width = 40;
	m_fireBallSize.height = 40;

	m_jumpVelocity = 1000;
	m_gravity = -6000;
	m_yMinVelocity = -1000;
	m_xVelocity = 1000;
	m_yVelocity = m_yMinVelocity;

	m_fireBallTexture = Director::getInstance()->getTextureCache()->addImage("Mario/FireBall.png");

	m_sprite = Sprite::create();
	Size textureSize = m_fireBallTexture->getContentSize();
	m_sprite->setTexture(m_fireBallTexture);
	m_sprite->setTextureRect(Rect(0, 0, textureSize.width, textureSize.height));
	m_sprite->setContentSize(textureSize);
	m_sprite->setScale(5.0f);
	m_sprite->setVisible(false);

	m_fireBallRotateByAction = RotateBy::create(0.0f, 90.0f);
	m_fireBallRotateByAction->retain();
	m_DelayTimeAction = DelayTime::create(0.10f);
	m_DelayTimeAction->retain();
	m_fireBallRotateBySequenceAction = Sequence::create(m_DelayTimeAction, m_fireBallRotateByAction, NULL);
	m_fireBallRotateBySequenceAction->retain();
	m_fireBallRotateByRepeatForeverAction = RepeatForever::create(m_fireBallRotateBySequenceAction);
	m_fireBallRotateByRepeatForeverAction->retain();

	m_explodeAnimation = Animation::create();
	m_explodeAnimation->addSpriteFrameWithFileName("Mario/FireExplode1.png");
	m_explodeAnimation->addSpriteFrameWithFileName("Mario/FireExplode2.png");
	m_explodeAnimation->addSpriteFrameWithFileName("Mario/FireExplode3.png");
	m_explodeAnimation->setDelayPerUnit(0.05f);
	m_explodeAnimation->retain();
	m_explodeAnimate = Animate::create(m_explodeAnimation);
	m_explodeAnimate->retain();
	
	m_changeStateToStayCallback = CallFuncN::create(CC_CALLBACK_1(FireBall::ChangeStateToStayCallback, this));
	m_changeStateToStayCallback->retain();

	m_destroySequence = Sequence::create(m_explodeAnimate, m_changeStateToStayCallback, NULL);
	
	m_destroySequence->retain();

	this->addChild(m_sprite);

	return true;
}

void FireBall::UpdateFireBall(float deltaTime, float m_mapAnchorXEnd)
{
	switch (m_curState)
	{
	case FIRE_BALL_STATE::STAY:
		break;
	case FIRE_BALL_STATE::FIRE:
		m_yVelocity += m_gravity * deltaTime;
		if (m_yVelocity < m_yMinVelocity)
		{
			m_yVelocity = m_yMinVelocity;
		}

		if (this->getPositionY() < 0 || m_mapAnchorXEnd < this->getPositionX())
		{
			ChangeState(FIRE_BALL_STATE::DESTROY);
			break;
		}

		this->setPositionX(this->getPositionX() + m_xVelocity * deltaTime);
		this->setPositionY(this->getPositionY() + m_yVelocity * deltaTime);
		break;
	case FIRE_BALL_STATE::DESTROY:
		break;
	default:
		break;
	}

}

cocos2d::Rect FireBall::GetCollider()
{
	if (m_curState != FIRE_BALL_STATE::FIRE)
	{
		return Rect(-1000000, -1000000, 0, 0);
	}

	return Rect(this->getPositionX() - m_fireBallSize.width / 2, this->getPositionY() - m_fireBallSize.height / 2, m_fireBallSize.width, m_fireBallSize.height);
}

bool FireBall::Fire(Vec2 startPos, int dir)
{
	if (m_curState != FIRE_BALL_STATE::STAY)
	{
		return false;
	}
	
	m_fireStartPos = startPos;
	m_fireDir = dir;

	ChangeState(FireBall::FIRE);
	return true;
}

void FireBall::ChangeState(FIRE_BALL_STATE state)
{
	switch (state)
	{
	case FireBall::STAY:
	{
		Size textureSize = m_fireBallTexture->getContentSize();
		m_sprite->setTexture(m_fireBallTexture);
		m_sprite->setTextureRect(Rect(0, 0, textureSize.width, textureSize.height));
		m_sprite->setContentSize(textureSize);
		m_yVelocity = m_yMinVelocity;
		m_sprite->setVisible(false);
	}
		break;
	case FireBall::FIRE:
		this->setPosition(m_fireStartPos);
		m_xVelocity = m_fireDir * abs(m_xVelocity);
		m_sprite->setVisible(true);
		m_sprite->runAction(m_fireBallRotateByRepeatForeverAction);
		break;
	case FireBall::DESTROY:
		m_sprite->stopAllActions();
		m_sprite->runAction(m_destroySequence);
		break;
	default:
		break;
	}

	m_curState = state;
}

void FireBall::SetPositionYBecauseCallWithBlock(float blockTopYPos, float yDelta)
{
	if (m_curState != FIRE_BALL_STATE::FIRE)
	{
		return;
	}

	if (yDelta < m_fireBallSize.height && blockTopYPos - 5 < this->getPositionY())
	{
		this->setPositionY(this->getPositionY() + yDelta);
		m_yVelocity = m_jumpVelocity;
	}
	else
	{
		SetDestroy();
	}
}

void FireBall::SetDestroy()
{
	if (m_curState != FIRE_BALL_STATE::FIRE)
	{
		return;
	}

	ChangeState(FIRE_BALL_STATE::DESTROY);
}

void FireBall::DestroyRetainObj()
{
	Director::getInstance()->getTextureCache()->removeTexture(m_fireBallTexture);
	m_fireBallRotateByAction->release();
	m_DelayTimeAction->release();
	m_fireBallRotateBySequenceAction->release();
	m_fireBallRotateByRepeatForeverAction->release();
	m_explodeAnimation->release();
	m_explodeAnimate->release();
	m_changeStateToStayCallback->release();
	m_destroySequence->release();
}

void FireBall::ChangeStateToStayCallback(Node* obj)
{
	ChangeState(FIRE_BALL_STATE::STAY);
}


FireBall::FireBall()
{
}


FireBall::~FireBall()
{
}
