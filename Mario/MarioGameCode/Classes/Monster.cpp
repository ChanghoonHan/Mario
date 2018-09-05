#include "Monster.h"
#include "MapAnchor.h"
#include "ScoreBoard.h"

USING_NS_CC;

bool Monster::init()
{
	SetTextureAndMonsterSize();

	m_monsterSprite = Sprite::create();
	m_monsterSprite->setScale(5.0f);
	this->setAnchorPoint(Vec2::ZERO);
	m_monsterSprite->setAnchorPoint(Vec2::ZERO);
	this->addChild(m_monsterSprite);

	m_idleSpriteChangeDeltaTime = 0;
	m_idleSpriteChangeTime = 0.15f;

	ChangeState(MONSTER_STATE::STAY);

	m_collider = Rect::ZERO;

	m_spawnDist = 80 * 10;

	m_curMoveDir = -1;
	m_monsterSize.width = 70;
	m_monsterSize.height = 75;

	m_xVelocity = 200;
	m_yVelocity = 0;
	m_minYVelocity = -800;
	m_gravity = -3200;

	m_instantaneousDestroy = false;
	m_changeStateToDestroyedCallback = CallFuncN::create(CC_CALLBACK_1(Monster::ChangeStateCallback, this, MONSTER_STATE::DESTROYED));
	m_changeStateToDestroyedCallback->retain();

	m_endScoreLabelActionCallback = CallFuncN::create(CC_CALLBACK_1(Monster::EndLabelActionCallback, this));
	m_endScoreLabelActionCallback->retain();

	m_labelMoveByAction = MoveBy::create(0.7f, Vec2(0, 100));
	m_labelMoveByAction->retain();

	std::string labelPath = "./fonts/Super Mario Bros. NES.ttf";
	int labelSize = 20;
	m_scoreLabel = LabelTTF::create("", labelPath, labelSize);
	m_scoreLabel->retain();

	this->addChild(m_scoreLabel);

	return true;
}

void Monster::UpdateMonster(float deltaTime)
{
	switch (m_curState)
	{
	case MONSTER_STATE::STAY:
		break;
	case MONSTER_STATE::IDLE:
		m_yVelocity += m_gravity * deltaTime;
		if (m_yVelocity < m_minYVelocity)
		{
			m_yVelocity = m_minYVelocity;
		}
		this->setPositionX(this->getPositionX() + m_xVelocity * m_curMoveDir * deltaTime);
		this->setPositionY(this->getPositionY() + m_yVelocity * deltaTime);

		if (this->getPositionY() < -100)
		{
			ChangeState(MONSTER_STATE::DESTROYED);
		}
		break;
	case MONSTER_STATE::DESTROY:

		break;
	case MONSTER_STATE::DESTROYED:
		break;
	default:
		break;
	}
}

void Monster::ChangeState(MONSTER_STATE state, bool instantaneousDestroy)
{
	switch (state)
	{
	case MONSTER_STATE::STAY:
		break;
	case MONSTER_STATE::IDLE:
		break;
	case MONSTER_STATE::DESTROY:
		StartScoreLabelAction();
		if (instantaneousDestroy)
		{
			m_instantaneousDestroy = instantaneousDestroy;
			m_monsterSprite->setFlippedY(true);
			float destY = this->getPositionY() + 100;
			float durationU = (destY + 200) / 460;
			this->runAction(Sequence::create(
				JumpBy::create(durationU / 2, Vec2(150 * m_curMoveDir * durationU, -destY), 100 + (destY / 2), 1),
				m_changeStateToDestroyedCallback, NULL)); 
		}
		break;
	case MONSTER_STATE::DESTROYED:
		m_monsterSprite->setVisible(false);
		break;
	default:
		break;
	}

	m_curState = state;
	SetTexture();
}

bool Monster::CheckCollWithMario(cocos2d::Rect marioCollider, bool marioIsInvincible, bool marioIsDroping, bool* attackedUpside, Monster* neighborMonster)
{
	*attackedUpside = false;

	switch (m_curState)
	{
	case MONSTER_STATE::STAY:
	{
		float xDistToMario;
		xDistToMario = this->getPositionX() - marioCollider.getMinX();
		if (xDistToMario < m_spawnDist)
		{
			ChangeState(MONSTER_STATE::IDLE);
			float neighborDist = neighborMonster->getPositionX() - this->getPositionX();
			if (neighborMonster != NULL && neighborDist < 100)
			{
				neighborMonster->setPositionX(neighborMonster->getPositionX() + 20);
				neighborMonster->Spawn();
			}
		}
	}
		break;
	case MONSTER_STATE::IDLE:
		SetCollider();
		if (m_collider.intersectsRect(marioCollider))
		{
			if (marioIsInvincible)
			{
				if (marioCollider.getMidX() < m_collider.getMidX())
				{
					AttackedFromBullet(-1);//leftAttack
				}
				else
				{
					AttackedFromBullet(1);//RightAttack
				}
				return false;
			}
			else
			{
				Rect collRect = CalCollRect(m_collider, marioCollider);
				if (marioIsDroping &&
					collRect.size.height < collRect.size.width + 40)
				{
					*attackedUpside = true;
					ChangeState(MONSTER_STATE::DESTROY);
				}

				return true;
			}
		}
		break;

	case MONSTER_STATE::DESTROY:
	
		break;

	case MONSTER_STATE::DESTROYED:

		break;
	default:
		break;
	}

	return false;
}

void Monster::Spawn()
{
	if (m_curState != MONSTER_STATE::STAY)
	{
		return;
	}

	ChangeState(MONSTER_STATE::IDLE);
}

void Monster::SetYPositionBecauseColl(float deltaY)
{
	if (m_curState != MONSTER_STATE::IDLE)
	{
		return;
	}

	m_yVelocity = 0;
	this->setPositionY(this->getPositionY() + deltaY);
}

void Monster::SetXPositionBecauseColl(float deltaX)
{
	if (m_curState != MONSTER_STATE::IDLE)
	{
		return;
	}

	m_curMoveDir *= -1;
	this->setPositionX(this->getPositionX() + deltaX);
}

void Monster::SetInstantaneousDestroy(bool changeDir)
{
	if (changeDir)
	{
		m_curMoveDir *= -1;
	}

	m_instantaneousDestroy = true;
	ChangeState(MONSTER_STATE::DESTROY, true);
}

void Monster::SetCollider()
{
	Vec2 thisPos = this->getPosition();

	m_collider = Rect(thisPos.x + (80 - m_monsterSize.width) / 2, thisPos.y, m_monsterSize.width, m_monsterSize.height);
}

Rect Monster::GetCollider(bool UseCollWithMonster)
{
	if (m_curState == MONSTER_STATE::DESTROY && UseCollWithMonster && !m_instantaneousDestroy)
	{
		SetCollider();

		return m_collider;
	}

	if (m_curState != MONSTER_STATE::IDLE)
	{
		return Rect::ZERO;
	}

	SetCollider();

	return m_collider;
}

void Monster::SetTexture()
{
	Size textureSize = m_curTexture->getContentSize();
	m_monsterSprite->setTexture(m_curTexture);
	m_monsterSprite->setTextureRect(Rect(0, 0, textureSize.width, textureSize.height));
	m_monsterSprite->setContentSize(textureSize);
}

Rect Monster::CalCollRect(Rect rect1, Rect rect2)
{
	float minX = rect1.getMinX() < rect2.getMinX() ? rect2.getMinX() : rect1.getMinX();
	float minY = rect1.getMinY() < rect2.getMinY() ? rect2.getMinY() : rect1.getMinY();
	float maxX = rect1.getMaxX() < rect2.getMaxX() ? rect1.getMaxX() : rect2.getMaxX();
	float maxY = rect1.getMaxY() < rect2.getMaxY() ? rect1.getMaxY() : rect2.getMaxY();

	return Rect(minX, minY, maxX - minX, maxY - minY);
}

void Monster::ChangeStateCallback(cocos2d::Node* obj, MONSTER_STATE state)
{
	ChangeState(state);
}

void Monster::AttackedFromBullet(int attackedDir)
{
	if (m_curState == MONSTER_STATE::STAY || m_curState == MONSTER_STATE::DESTROYED || m_instantaneousDestroy)
	{
		return;
	}

	m_curMoveDir = attackedDir * -1;

	ChangeState(MONSTER_STATE::DESTROY, true);
}

bool Monster::IsCanTeamKill()
{
	return false;
}

void Monster::SetScoreLabel()
{
	String scoreStr;
	scoreStr.initWithFormat("%d", m_score);
	m_scoreLabel->setString(scoreStr._string);
	m_scoreLabel->setVisible(false);
}

void Monster::StartScoreLabelAction()
{
	m_scoreLabel->stopAllActions();
	if (this->getParent() == m_scoreLabel->getParent())
	{
		EndLabelActionCallback(this);
	}

	ScoreBoard::GetInstance()->IncreaseScore(m_score);

	m_scoreLabel->setVisible(true);
	m_scoreLabel->setPosition(Vec2(this->getPositionX() + 60, this->getPositionY() + 60));
	m_scoreLabel->runAction(Sequence::create(m_labelMoveByAction, m_endScoreLabelActionCallback, NULL));
	this->removeChild(m_scoreLabel, false);
	this->getParent()->addChild(m_scoreLabel, MapAnchor::SORT_ORDER::MONSETER_BLOCK);
}

void Monster::EndLabelActionCallback(Node* obj)
{
	this->getParent()->removeChild(m_scoreLabel, false);
	this->addChild(m_scoreLabel);
	m_scoreLabel->setVisible(false);
}

void Monster::ReleaseObj()
{
	for (auto& texture : m_textures)
	{
		Director::getInstance()->getTextureCache()->removeTexture(texture);
	}
	m_textures.clear();

	m_changeStateToDestroyedCallback->release();
	m_labelMoveByAction->release();
	m_endScoreLabelActionCallback->release();
	m_scoreLabel->release();
}

Monster::Monster()
{
}


Monster::~Monster()
{
}
