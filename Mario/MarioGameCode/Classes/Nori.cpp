#include "Nori.h"
#include "ScoreBoard.h"

USING_NS_CC;

bool Nori::init()
{
	Monster::init();

	m_isDieMoving = false;
	m_gracePeriodTime = 0.5f;
	m_gracePeriodDeltaTime = 0;

	m_resurrectionTime = 7.0f;
	m_semiResurrectionTime = 5.0f;
	m_resurrectionDeltaTime = 0;

	m_dieTextureChangeTime = 0.1f;
	m_dieTextureChangeDeltaTime = 0;

	m_score = 200;
	SetScoreLabel();

	return true;
}

void Nori::UpdateMonster(float deltaTime)
{
	if (m_curMoveDir == 1)
	{
		m_monsterSprite->setFlipX(true);
	}
	else
	{
		m_monsterSprite->setFlipX(false);
	}

	switch (m_curState)
	{
	case MONSTER_STATE::STAY:
		break;
	case MONSTER_STATE::IDLE:
		m_idleSpriteChangeDeltaTime += deltaTime;
		if (m_idleSpriteChangeTime < m_idleSpriteChangeDeltaTime)
		{
			m_idleSpriteChangeDeltaTime = 0;
			m_curNoriTextureIdx = (NORI_TEXTURE)((m_curNoriTextureIdx + 1) % 2);
			SetTexture();
		}
		break;
	case MONSTER_STATE::DESTROY:
		if (!m_instantaneousDestroy)
		{
			if (m_isDieMoving)
			{
				m_resurrectionDeltaTime = 0;
				m_gracePeriodDeltaTime += deltaTime;
				if (m_gracePeriodTime <= m_gracePeriodDeltaTime)
				{
					m_gracePeriodDeltaTime = m_gracePeriodTime;
				}

				m_yVelocity += m_gravity * deltaTime;
				if (m_yVelocity < m_minYVelocity)
				{
					m_yVelocity = m_minYVelocity;
				}
				this->setPositionX(this->getPositionX() + m_xVelocity * m_curMoveDir * deltaTime * 4.5f);
				this->setPositionY(this->getPositionY() + m_yVelocity * deltaTime);
			}
			else
			{
				m_gracePeriodDeltaTime = 0;
				m_resurrectionDeltaTime += deltaTime;
				if (m_semiResurrectionTime < m_resurrectionDeltaTime)
				{
					m_dieTextureChangeDeltaTime += deltaTime;
					if (m_dieTextureChangeTime < m_dieTextureChangeDeltaTime)
					{
						m_dieTextureChangeDeltaTime = 0;
						m_curNoriTextureIdx = (NORI_TEXTURE)(((m_curNoriTextureIdx + 1) % 2) + 2);
						SetTexture();
					}

					if (m_resurrectionTime < m_resurrectionDeltaTime)
					{
						m_resurrectionDeltaTime = 0;
						ChangeState(MONSTER_STATE::IDLE);
					}
				}
			}
		}
		break;
	case MONSTER_STATE::DESTROYED:
		break;
	default:
		break;
	}

	Monster::UpdateMonster(deltaTime);
}

void Nori::ChangeState(MONSTER_STATE state, bool instantaneousDestroy)
{
	switch (state)
	{
	case Monster::STAY:
		m_curNoriTextureIdx = NORI_TEXTURE::IDLE1;
		break;
	case Monster::IDLE:
		m_monsterSize.width = 80;
		m_monsterSize.height = 120;
		m_curNoriTextureIdx = NORI_TEXTURE::IDLE1;
		break;
	case Monster::DESTROY:
		m_curNoriTextureIdx = NORI_TEXTURE::DIE;
		m_monsterSize.width = 80;
		m_monsterSize.height = 80;
		break;
	default:
		break;
	}

	Monster::ChangeState(state, instantaneousDestroy);
}

bool Nori::CheckCollWithMario(cocos2d::Rect marioCollider, bool marioIsInvincible, bool marioIsDroping, bool* attackedUpside, Monster* neighborMonster)
{
	if (Monster::CheckCollWithMario(marioCollider, marioIsInvincible, marioIsDroping, attackedUpside, neighborMonster))
	{
		return true;
	};

	switch (m_curState)
	{
	case MONSTER_STATE::STAY:
	
		break;
	case MONSTER_STATE::IDLE:

		break;

	case MONSTER_STATE::DESTROY:
		SetCollider();
		if (m_collider.intersectsRect(marioCollider))
		{
			m_curNoriTextureIdx = NORI_TEXTURE::DIE;
			SetTexture();

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
					m_isDieMoving = !m_isDieMoving;
					StartScoreLabelAction();
					if (marioCollider.getMidX() < m_collider.getMidX())
					{
						m_curMoveDir = 1;//leftAttack
					}
					else
					{
						m_curMoveDir = -1;//RightAttack
					}
				}
				else if (!m_isDieMoving)
				{
					m_isDieMoving = !m_isDieMoving;
					StartScoreLabelAction();
					if (marioCollider.getMidX() < m_collider.getMidX())
					{
						m_curMoveDir = 1;//leftAttack
					}
					else
					{
						m_curMoveDir = -1;//RightAttack
					}

					return false;
				}
				else
				{
					if (m_gracePeriodDeltaTime < m_gracePeriodTime)
					{
						return false;
					}
				}

				return true;
			}
		}
		break;

	case MONSTER_STATE::DESTROYED:

		break;
	default:
		break;
	}

	return false;
}

void Nori::SetTextureAndMonsterSize()
{
	m_monsterSize.width = 80;
	m_monsterSize.height = 80;

	m_textures.push_back(Director::getInstance()->getTextureCache()->addImage("Monsters/NoriIdle1.png"));
	m_textures.push_back(Director::getInstance()->getTextureCache()->addImage("Monsters/NoriIdle2.png"));
	m_textures.push_back(Director::getInstance()->getTextureCache()->addImage("Monsters/NoriDie.png"));
	m_textures.push_back(Director::getInstance()->getTextureCache()->addImage("Monsters/NoriNoneDie.png"));
	m_curNoriTextureIdx = NORI_TEXTURE::IDLE1;
}

void Nori::SetYPositionBecauseColl(float deltaY)
{
	if (m_curState == MONSTER_STATE::STAY || m_curState == MONSTER_STATE::DESTROYED || m_instantaneousDestroy)
	{
		return;
	}

	m_yVelocity = 0;
	this->setPositionY(this->getPositionY() + deltaY);
}

void Nori::SetXPositionBecauseColl(float deltaX)
{
	if (m_curState == MONSTER_STATE::STAY || m_curState == MONSTER_STATE::DESTROYED || m_instantaneousDestroy)
	{
		return;
	}

	m_curMoveDir *= -1;
	this->setPositionX(this->getPositionX() + deltaX);
}

Rect Nori::GetCollider(bool UseCollWithMonster)
{
	if (m_curState == MONSTER_STATE::STAY || m_curState == MONSTER_STATE::DESTROYED || m_instantaneousDestroy)
	{
		return Rect::ZERO;
	}

	SetCollider();

	return m_collider;
}

void Nori::SetTexture()
{
	m_curTexture = m_textures[m_curNoriTextureIdx];
	Monster::SetTexture();
}

bool Nori::IsCanTeamKill()
{
	return m_isDieMoving;
}

Nori::Nori()
{
}


Nori::~Nori()
{
}