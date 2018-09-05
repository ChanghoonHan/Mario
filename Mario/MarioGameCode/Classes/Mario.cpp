#include "Mario.h"
#include "Item.h"
#include "MapAnchor.h"

USING_NS_CC;

bool Mario::init()
{
	LoadTexture();

	m_InfoLable = LabelTTF::create("00", "", 30);
	m_InfoLable->setPosition(Vec2(40, 180));
	this->addChild(m_InfoLable);
	m_InfoLable->setVisible(false);

	m_curState = MARIO_STATE::IDLE;
	m_prevState = MARIO_STATE::IDLE;

	m_marioSizeForTexture = MARIO_SIZE::SMALL_MARIO;
	m_marioStateForTexture = MARIO_TEXTURE::IDLE_TEXTURE;

	m_marioSize = MARIO_SIZE::SMALL_MARIO;

	m_growOrDiminishAnimationChangeTextureTime = 0.065f;
	m_growOrDiminishAnimationChangeTextureDeltaTime = 0;
	m_growOrDiminishAnimationTextureInfoIdx = 0;

	m_changeToFireInvinChangeDeltaTime = 0;
	m_changeToFireInvinchangeTime = 0.060f;

	m_invincibleSustainmentTime = 10;
	m_invincibleSustainmentDeltaTime = 0;

	m_invincibleTextureChangeTimeFast = 0.03f;
	m_invincibleTextureChangeTimeSlow = 0.08f;
	m_invincibleTextureChangeTime = m_invincibleTextureChangeTimeFast;
	m_invincibleTextureChangeDeltaTime = 0;

	m_changeFlagTextureTime = 0.05f;
	m_changeFlagTextureDeltaTime = 0;

	m_changeGoToCastleDeltaTime = 0;
	m_changeGoToCastleTime = 0.5f;

	m_flagDownSpeed = 360;

	m_changeToFireInvinCycleCount = 0;
	m_invincibleCount = 0;

	m_releaseTurboKey = true;
	m_isSit = false;
	m_isHaveObstacle = false;
	m_canGoToCastle = false;
	m_isInvincible = false;
	m_isBlinking = false;
	m_curSpeed = 0;
	m_timeToMaxSpeed = 1;
	m_timeToZeroSpeed = 0.7f;
	m_timeToZeroSpeedForChangeDir = 0.4f;
	m_maxSpeedOrigin = 400;
	m_maxSpeedDash = 800;
	m_maxSpeed = m_maxSpeedOrigin;
	m_yMinVelocity = -1000;
	m_jumpVelocity = 1000;
	m_gravity = -4500;
	m_yVelocity = m_yMinVelocity;
	m_jumpKeyInputMaxTime = 0.23f;
	m_jumpKeyInputDeltaTime = 0;
	m_isPressedJumpKey = false;
	m_isJumping = false;

	m_prevPos = Vec2::ZERO;

	m_curMoveDir = 0;
	m_prevMoveDir = 0;
	m_breakDir = 0;

	m_reloadTime = 0.1f;
	m_reloadDeltaTime = m_reloadTime;

	m_changeWalkTextureDeltaTime = 0;
	m_changeWalkTextureTimeMaxOrigin = 0.2f;
	m_changeWalkTextureTimeMaxDash = 0.1f;
	m_changeWalkTextureTimeMax = m_changeWalkTextureTimeMaxOrigin;
	m_changeWalkTextureTimeMinOrigin = 0.08f;
	m_changeWalkTextureTimeMinDash = 0.04f;
	m_changeWalkTextureTimeMin = m_changeWalkTextureTimeMinOrigin;

	this->setAnchorPoint(Vec2(0, 0));
	this->setPosition(Vec2(200, 160));
	m_marioSprite = Sprite::create();
	m_marioSprite->setAnchorPoint(Vec2(0.5f, 0));
	m_marioSprite->setPosition(Vec2(40, 0));
	m_marioSprite->setScale(5.0f);
	this->addChild(m_marioSprite);

	SetTexture();
	LoadAnimationTextureInfo();

	return true;
}

void Mario::UpdateMario(float deltaTime, float* marioMoveXdist)
{
	if (m_releaseTurboKey)
	{
		m_maxSpeed -= ((m_maxSpeedDash - m_maxSpeedOrigin) * deltaTime);
		if (m_maxSpeed < m_maxSpeedOrigin)
		{
			m_maxSpeed = m_maxSpeedOrigin;
		}

		m_changeWalkTextureTimeMin += ((m_changeWalkTextureTimeMinOrigin - m_changeWalkTextureTimeMinDash) * deltaTime);
		if (m_changeWalkTextureTimeMinOrigin < m_changeWalkTextureTimeMin)
		{
			m_changeWalkTextureTimeMin = m_changeWalkTextureTimeMinOrigin;
		}

		m_changeWalkTextureTimeMax += ((m_changeWalkTextureTimeMaxOrigin - m_changeWalkTextureTimeMaxDash) * deltaTime);
		if (m_changeWalkTextureTimeMaxOrigin < m_changeWalkTextureTimeMax)
		{
			m_changeWalkTextureTimeMax = m_changeWalkTextureTimeMaxOrigin;
		}
	}
	else
	{
		m_maxSpeed += ((m_maxSpeedDash - m_maxSpeedOrigin) * deltaTime);
		if (m_maxSpeedDash < m_maxSpeed)
		{
			m_maxSpeed = m_maxSpeedDash;
		}

		m_changeWalkTextureTimeMin -= ((m_changeWalkTextureTimeMinOrigin - m_changeWalkTextureTimeMinDash) * deltaTime);
		if (m_changeWalkTextureTimeMin < m_changeWalkTextureTimeMinDash)
		{
			m_changeWalkTextureTimeMin = m_changeWalkTextureTimeMinDash;
		}

		m_changeWalkTextureTimeMax -= ((m_changeWalkTextureTimeMaxOrigin - m_changeWalkTextureTimeMaxDash) * deltaTime);
		if (m_changeWalkTextureTimeMax < m_changeWalkTextureTimeMaxDash)
		{
			m_changeWalkTextureTimeMax = m_changeWalkTextureTimeMaxDash;
		}
	}

	if (IsChanging())
	{
		*marioMoveXdist = 0;

		switch (m_curState)
		{
		case MARIO_STATE::CHANGING_TO_BIG:
			m_growOrDiminishAnimationChangeTextureDeltaTime += deltaTime;
			if (m_growOrDiminishAnimationChangeTextureTime < m_growOrDiminishAnimationChangeTextureDeltaTime)
			{
				if (m_growOrDiminishAnimationTextureInfoIdx == m_changeAnimationTextureInfo[0].size())
				{
					m_growOrDiminishAnimationChangeTextureDeltaTime = 0;
					m_growOrDiminishAnimationTextureInfoIdx = 0;
					m_marioSize = MARIO_SIZE::BIG_MARIO;
					ChangeToPrevState();
				}
				else
				{
					m_growOrDiminishAnimationChangeTextureDeltaTime = 0;
					m_marioSizeForTexture = m_changeAnimationTextureInfo[0][m_growOrDiminishAnimationTextureInfoIdx].marioSize;
					m_marioStateForTexture = m_changeAnimationTextureInfo[0][m_growOrDiminishAnimationTextureInfoIdx].marioTexture;
					m_growOrDiminishAnimationTextureInfoIdx++;
				}
			}

			InvincibleUpdate(deltaTime, true);
			break;
		case MARIO_STATE::CHANGING_TO_FIRE:
			m_changeToFireInvinChangeDeltaTime += deltaTime;
			if (m_changeToFireInvinchangeTime < m_changeToFireInvinChangeDeltaTime)
			{
				m_invincibleCount++;
				m_changeToFireInvinChangeDeltaTime = 0;
				if (m_changeToFireInvinCycleCount == 4)
				{
					m_changeToFireInvinChangeDeltaTime = 0;
					m_changeToFireInvinCycleCount = 0;
					m_invincibleCount = 0;
					m_marioSize = MARIO_SIZE::FIRE_MARIO;
					ChangeToPrevState();
				}
				else if (m_invincibleCount == 4)
				{
					m_invincibleCount = 0;
					m_changeToFireInvinCycleCount++;
				}
			}
			break;
		case MARIO_STATE::CHANGING_TO_SMALL:
			m_growOrDiminishAnimationChangeTextureDeltaTime += deltaTime;
			if (m_growOrDiminishAnimationChangeTextureTime < m_growOrDiminishAnimationChangeTextureDeltaTime)
			{
				if (m_growOrDiminishAnimationTextureInfoIdx == m_changeAnimationTextureInfo[1].size())
				{
					m_growOrDiminishAnimationChangeTextureDeltaTime = 0;
					m_growOrDiminishAnimationTextureInfoIdx = 0;
					m_marioSize = MARIO_SIZE::SMALL_MARIO;
					ChangeToPrevState();
				}
				else
				{
					m_growOrDiminishAnimationChangeTextureDeltaTime = 0;
					m_marioSizeForTexture = m_changeAnimationTextureInfo[1][m_growOrDiminishAnimationTextureInfoIdx].marioSize;
					m_marioStateForTexture = m_changeAnimationTextureInfo[1][m_growOrDiminishAnimationTextureInfoIdx].marioTexture;
					m_growOrDiminishAnimationTextureInfoIdx++;
				}
			}

			InvincibleUpdate(deltaTime, true);
			break;
		case MARIO_STATE::DIE:
		case MARIO_STATE::DIED:
			InvincibleUpdate(deltaTime, true);
			break;
		case MARIO_STATE::FLAG_DOWNING:
			this->setPositionY(this->getPositionY() - m_flagDownSpeed * deltaTime);
			if (this->getPositionY() < 240)
			{
				m_marioStateForTexture = MARIO_TEXTURE::FLAG1_TEXTURE;
				this->setPositionY(240);
			}
			else
			{
				m_changeFlagTextureDeltaTime += deltaTime;
				if (m_changeFlagTextureTime < m_changeFlagTextureDeltaTime)
				{
					m_changeFlagTextureDeltaTime = 0;
					m_marioStateForTexture = (MARIO_TEXTURE)(m_marioStateForTexture + 1);
					if (m_marioStateForTexture == MARIO_TEXTURE::GROW_TEXTURE)
					{
						m_marioStateForTexture = MARIO_TEXTURE::FLAG1_TEXTURE;
					}
				}
			}
			InvincibleUpdate(deltaTime, true);
			break;
		case MARIO_STATE::GO_END:
			if (m_canGoToCastle)
			{
				ChangeWalkTexture(deltaTime);
				this->setPositionX(this->getPositionX() + m_curSpeed * deltaTime);
				if (m_castleDoorXPos < this->getPositionX())
				{
					this->setPositionX(m_castleDoorXPos);
					ChangeMarioState(MARIO_STATE::CLEAR);
				}
				*marioMoveXdist = m_curSpeed * deltaTime;
				this->setPositionY(this->getPositionY() + m_yVelocity * deltaTime);
				if (this->getPositionY() < 160)
				{
					this->setPositionY(160);
				}
			}
			else
			{
				m_changeGoToCastleDeltaTime += deltaTime;
				if (m_changeGoToCastleTime < m_changeGoToCastleDeltaTime)
				{
					m_marioSprite->setFlippedX(false);
					m_isJumping = false;
					m_curSpeed = 350;
					m_yVelocity = -600;
					m_canGoToCastle = true;
					m_marioStateForTexture = MARIO_TEXTURE::WALK1_TEXTURE;
				}
			}
			InvincibleUpdate(deltaTime, true);
			break;
		default:
			break;
		}

		SetTexture();
		return;
	}
	else
	{
		InvincibleUpdate(deltaTime);
	}

	m_reloadDeltaTime += deltaTime;
	if (m_reloadTime < m_reloadDeltaTime)
	{
		m_reloadDeltaTime = m_reloadTime;
	}

	String speed;

	if (!m_isJumping)
	{
		if (0 < m_curMoveDir)
		{
			m_marioSprite->setFlipX(false);
		}
		
		if (m_curMoveDir < 0)
		{
			m_marioSprite->setFlipX(true);
		}
	}

	if (m_isPressedJumpKey)
	{
		m_jumpKeyInputDeltaTime += deltaTime;
		if (m_jumpKeyInputDeltaTime < m_jumpKeyInputMaxTime)
		{
			m_yVelocity = m_jumpVelocity;
		}
		else
		{
			m_isPressedJumpKey = false;
		}
	}
	else
	{
		m_yVelocity += m_gravity * deltaTime;
		if (m_yVelocity < m_yMinVelocity)
		{
			m_yVelocity = m_yMinVelocity;
		}
	}

	this->setPositionY(this->getPositionY() + m_yVelocity * deltaTime);
	if (this->getPositionY() < - 150)
	{
		ChangeMarioState(MARIO_STATE::DIED);
	}

	switch (m_curState)
	{
	case Mario::IDLE:
		if (!m_isJumping)
		{
			if (m_isSit && IsBigMario())
			{
				m_marioStateForTexture = MARIO_TEXTURE::SIT_TEXTURE;
			}
			else
			{
				m_marioStateForTexture = MARIO_TEXTURE::IDLE_TEXTURE;
			}
		}
		break;

	case Mario::WALK:
		ChangeWalkTexture(deltaTime);

		if (m_isHaveObstacle)
		{
			*marioMoveXdist = 0;
			m_curSpeed = 100 * m_curMoveDir;
		}
		else
		{
			m_prevXPos = this->getPositionX();

			m_curSpeed += m_curMoveDir * (m_maxSpeed / m_timeToMaxSpeed) * deltaTime;
			if (abs(m_curSpeed) > m_maxSpeed)
			{
				m_curSpeed = m_maxSpeed * m_curMoveDir;
			}
			*marioMoveXdist = m_curSpeed * deltaTime;
		}

		this->setPositionX(this->getPositionX() + m_curSpeed * deltaTime);
	
		if (m_curSpeed > 0)
		{
			m_breakDir = -1;
		}
		else
		{
			m_breakDir = 1;
		}
		break;

	case Mario::BRAKE:
		m_curSpeed += m_breakDir * (m_maxSpeed / m_timeToZeroSpeed) * deltaTime;

		if (m_prevXPos == this->getPositionX())
		{
			*marioMoveXdist = 0;
		}
		else
		{
			*marioMoveXdist = m_curSpeed * deltaTime;
		}

		this->setPositionX(this->getPositionX() + m_curSpeed * deltaTime);

		if (!m_isJumping)
		{
			if (m_isSit && IsBigMario())
			{
				m_marioStateForTexture = MARIO_TEXTURE::SIT_TEXTURE;
			}
			else
			{
				if (m_marioStateForTexture == MARIO_TEXTURE::SIT_TEXTURE)
				{
					m_marioStateForTexture = MARIO_TEXTURE::WALK1_TEXTURE;
				}
				ChangeWalkTexture(deltaTime);
			}
		}

		switch (m_breakDir)
		{
		case 0:
			if (m_curSpeed == 0)
			{
				ChangeMarioState(MARIO_STATE::IDLE);
			}
			break;
		case 1:
			if (m_curSpeed >= 0)
			{
				ChangeMarioState(MARIO_STATE::IDLE);
			}
			break;
		case -1:
			if (m_curSpeed <= 0)
			{
				ChangeMarioState(MARIO_STATE::IDLE);
			}
			break;
		default:
			break;
		}
		break;

	case Mario::BRAKE_FOR_CHANG_DIR:
		if (m_isSit && IsBigMario())
		{
			m_marioStateForTexture = MARIO_TEXTURE::SIT_TEXTURE;
			ChangeMarioState(MARIO_STATE::IDLE);
			m_curSpeed = 0;
			break;
		}

		m_curSpeed += m_breakDir * (m_maxSpeed / m_timeToZeroSpeedForChangeDir) * deltaTime;

		this->setPositionX(this->getPositionX() + m_curSpeed * deltaTime);
		if (m_prevXPos == this->getPositionX())
		{
			*marioMoveXdist = 0;
		}
		else
		{
			*marioMoveXdist = m_curSpeed * deltaTime;
		}

		switch (m_breakDir)
		{
		case 1:
			if (m_curSpeed > 0)
			{
				ChangeMarioState(MARIO_STATE::WALK);
			}
			break;
		case -1:
			if (m_curSpeed < 0)
			{
				ChangeMarioState(MARIO_STATE::WALK);
			}
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}

	SetInfoString();
}

void Mario::SetFlagDowning()
{
	ChangeMarioState(MARIO_STATE::FLAG_DOWNING);
}

void Mario::SetGoToCastle(float castleDoorXPos)
{
	m_castleDoorXPos = castleDoorXPos;
	ChangeMarioState(MARIO_STATE::GO_END);
}

void Mario::LateUpdateMario()
{
	if (IsChanging())
	{
		return;
	}

	if (m_prevPos.x == this->getPositionX())
	{
		m_isHaveObstacle = true;
	}
	else
	{
		m_isHaveObstacle = false;
	}

	if (m_prevPos.y == this->getPositionY())
	{
		SetJumpEnd();
	}
	else
	{
		SetJump();
	}

	m_prevPos = this->getPosition();
	SetTexture();
}

void Mario::SetJumpBlocked()
{
	m_yVelocity = 0;
	m_jumpKeyInputDeltaTime = m_jumpKeyInputMaxTime;
}

void Mario::onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	switch (keycode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_X:
		if (!(m_isSit && m_marioStateForTexture == MARIO_TEXTURE::SIT_TEXTURE))
		{
			FireFireBall();
		}
		m_releaseTurboKey = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Z:
		if (!m_isJumping)
		{
			m_jumpKeyInputDeltaTime = 0;
			m_isPressedJumpKey = true;
		}
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		m_curMoveDir -= 1;
		CheckMoveState();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		m_curMoveDir += 1;
		CheckMoveState();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		m_isSit = true;
		break;

	//Test Key@@@@@@@@@@@@@@@@@@
	case cocos2d::EventKeyboard::KeyCode::KEY_Q:
		EatItem(Item::ITEM_TYPE::MUSHROOM);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		EatItem(Item::ITEM_TYPE::FLOWER);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_E:
		EatItem(Item::ITEM_TYPE::STAR);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_R:
		AttackedFromMonster();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_T:
		SetDie();
		break;
	default:
		break;
	}
}

void Mario::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	switch (keycode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_X:
		m_releaseTurboKey = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Z:
		m_jumpKeyInputDeltaTime = 0;
		m_isPressedJumpKey = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		m_curMoveDir += 1;
		CheckMoveState();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		m_curMoveDir -= 1;
		CheckMoveState();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		m_isSit = false;
		break;
	default:
		break;
	}
}

void Mario::SetDie()
{
	ChangeMarioState(MARIO_STATE::DIE);
}

void Mario::AttackedFromMonster()
{
	if (m_isBlinking)
	{
		return;
	}

	if (m_marioSize != MARIO_SIZE::SMALL_MARIO)
	{
		m_isBlinking = true;
		if (m_isJumping)
		{
			m_isChangeWhenJumping = true;
		}

		m_marioSprite->runAction(Sequence::create(Blink::create(4.0f, 20), CallFuncN::create(CC_CALLBACK_1(Mario::EndBlinkCallback, this)), NULL));
		ChangeMarioState(MARIO_STATE::CHANGING_TO_SMALL);
	}
	else
	{
		ChangeMarioState(MARIO_STATE::DIE);
	}
}

void Mario::EndBlinkCallback(Node* obj)
{
	m_isBlinking = false;
}

void Mario::CheckMoveState()
{
	if (IsChanging())
	{
		return;
	}

	if (m_curMoveDir == 0)
	{
		ChangeMarioState(MARIO_STATE::BRAKE);
	}
	else
	{
		if (m_breakDir != 0 && m_curMoveDir == m_breakDir)
		{
			ChangeMarioState(MARIO_STATE::BRAKE_FOR_CHANG_DIR);
		}
		else
		{
			ChangeMarioState(MARIO_STATE::WALK);
		}
	}
}

void Mario::ChangeToPrevState()
{
	ChangeMarioState(m_prevState);
	CheckMoveState();
}

void Mario::ChangeMarioState(MARIO_STATE state)
{
	if (state == m_curState)
	{
		return;
	}

	switch (state)
	{
	case MARIO_STATE::IDLE:
		if (!m_isJumping)
		{
			if (m_isSit && IsBigMario())
			{
				m_marioStateForTexture = MARIO_TEXTURE::SIT_TEXTURE;
			}
			else
			{
				m_marioStateForTexture = MARIO_TEXTURE::IDLE_TEXTURE;
			}
		}

		m_prevXPos = -1;
		m_curSpeed = 0;
		m_breakDir = 0;
		break;
	case MARIO_STATE::WALK:
		if (!m_isJumping)
		{
			m_marioStateForTexture = MARIO_TEXTURE::WALK1_TEXTURE;
		}

		break;
	case MARIO_STATE::BRAKE:

		break;
	case MARIO_STATE::BRAKE_FOR_CHANG_DIR:
		if (!m_isJumping)
		{
			m_marioStateForTexture = MARIO_TEXTURE::BRAKE_TEXTURE;
		}
		break;
	case MARIO_STATE::CHANGING_TO_BIG:
	{
		if (m_marioSize != MARIO_SIZE::SMALL_MARIO)
		{
			return;
		}

		m_jumpKeyInputDeltaTime = m_jumpKeyInputMaxTime;
		m_yVelocity = 0;
		m_growOrDiminishAnimationChangeTextureDeltaTime = 0;
		m_growOrDiminishAnimationTextureInfoIdx = 0;
		m_marioSizeForTexture = MARIO_SIZE::BIG_MARIO;
	}
		break;
	case MARIO_STATE::CHANGING_TO_FIRE:
	{
		if (m_marioSize == MARIO_SIZE::FIRE_MARIO)
		{
			return;
		}

		m_jumpKeyInputDeltaTime = m_jumpKeyInputMaxTime;
		m_yVelocity = 0;
		if (m_marioSize == MARIO_SIZE::SMALL_MARIO)
		{
			state = MARIO_STATE::CHANGING_TO_BIG;
			m_growOrDiminishAnimationChangeTextureDeltaTime = 0;
			m_growOrDiminishAnimationTextureInfoIdx = 0;
			m_marioSizeForTexture = MARIO_SIZE::BIG_MARIO;
		}
		else
		{
			m_marioSizeForTexture = MARIO_SIZE::FIRE_MARIO;
			m_changeToFireInvinChangeDeltaTime = 0;
			m_changeToFireInvinCycleCount = 0;
			m_invincibleCount = 0;
		}
	}
		break;
	case MARIO_STATE::CHANGING_TO_SMALL:
	{
		m_jumpKeyInputDeltaTime = m_jumpKeyInputMaxTime;
		m_yVelocity = 0;
		m_marioSize = MARIO_SIZE::SMALL_MARIO;
		m_growOrDiminishAnimationChangeTextureDeltaTime = 0;
		m_growOrDiminishAnimationTextureInfoIdx = 0;
		m_marioSizeForTexture = MARIO_SIZE::SMALL_MARIO;
	}
		break;
	case MARIO_STATE::DIE:
	{
		float destY = this->getPositionY() + 100;
		float durationU = (destY + 560) / 820;
		m_marioStateForTexture = MARIO_TEXTURE::DIE_TEXTURE;
		CallFuncN* changeStateToDiedCallback = CallFuncN::create(CC_CALLBACK_1(Mario::ChangeMarioStateCallback, this, MARIO_STATE::DIED));
		this->runAction(Sequence::create(DelayTime::create(0.5f), JumpBy::create(durationU, Vec2(0, -destY), 280 + (destY / 2), 1), changeStateToDiedCallback, NULL));
		SetTexture();
	}
		break;
	case MARIO_STATE::DIED:
	
		break;
	case MARIO_STATE::FLAG_DOWNING:
		m_marioStateForTexture = MARIO_TEXTURE::FLAG1_TEXTURE;
		SetTexture();
		break;
	case MARIO_STATE::GO_END:
		m_marioStateForTexture = MARIO_TEXTURE::FLAG1_TEXTURE;
		this->setPositionX(this->getPositionX() + 60);
		m_marioSprite->setFlipX(true);
		SetTexture();
		break;
	case MARIO_STATE::CLEAR:
		m_marioSprite->setVisible(false);
		break;
	default:
		break;
	}

	m_prevState = m_curState;
	m_curState = state;
	SetInfoString();
}

void Mario::ChangeMarioStateCallback(Node* obj, MARIO_STATE state)
{
	ChangeMarioState(state);
}

void Mario::SetInfoString()
{
	switch (m_curState)
	{
	case MARIO_STATE::IDLE:
		m_InfoLable->setString("IDLE\n");
		break;
	case MARIO_STATE::WALK:
		m_InfoLable->setString("WALK\n");
		break;
	case MARIO_STATE::BRAKE:
		m_InfoLable->setString("BREAK\n");
		break;
	case MARIO_STATE::BRAKE_FOR_CHANG_DIR:
		m_InfoLable->setString("BRAKE_FOR_CHANG_DIR\n");
		break;
	case MARIO_STATE::CHANGING_TO_BIG:
		m_InfoLable->setString("CHANGING_TO_BIG\n");
		break;
	case MARIO_STATE::CHANGING_TO_FIRE:
		m_InfoLable->setString("CHANGING_TO_FIRE\n");
		break;
	case MARIO_STATE::CHANGING_TO_SMALL:
		m_InfoLable->setString("CHANGING_TO_SMALL\n");
		break;
	case MARIO_STATE::FLAG_DOWNING:
		m_InfoLable->setString("FLAG_DOWNING\n");
		break;
	default:
		break;
	}

	if (m_isJumping)
	{
		m_InfoLable->setString(m_InfoLable->getString() + "Jumping\n");
	}
	else
	{
		m_InfoLable->setString(m_InfoLable->getString() + "NotJumping\n");
	}

	String speed;
	speed.initWithFormat("%.2f", m_curSpeed);
	m_InfoLable->setString(m_InfoLable->getString() + speed._string);
}

void Mario::SetJumpBecauseCollWithMonster()
{
	SetJump();
	m_jumpKeyInputDeltaTime = m_jumpKeyInputMaxTime;
	m_yVelocity = m_jumpVelocity - 100;
}

void Mario::SetJumpEnd()
{
	if (!m_isJumping)
	{
		return;
	}

	m_isChangeWhenJumping = false;
	m_isJumping = false;
	switch (m_curState)
	{
	case Mario::IDLE:
		if (m_isSit && IsBigMario())
		{
			m_marioStateForTexture = MARIO_TEXTURE::SIT_TEXTURE;
		}
		else
		{
			m_marioStateForTexture = MARIO_TEXTURE::IDLE_TEXTURE;
		}
		break;
	case Mario::WALK:
		m_marioStateForTexture = MARIO_TEXTURE::WALK1_TEXTURE;
		break;
	case Mario::BRAKE:
		if (m_isSit && IsBigMario())
		{
			m_marioStateForTexture = MARIO_TEXTURE::SIT_TEXTURE;
		}
		else
		{
			m_marioStateForTexture = MARIO_TEXTURE::WALK1_TEXTURE;
		}
		break;
	case Mario::BRAKE_FOR_CHANG_DIR:
		if (m_isSit && IsBigMario())
		{
			ChangeMarioState(MARIO_STATE::IDLE);
			m_marioStateForTexture = MARIO_TEXTURE::SIT_TEXTURE;
		}
		else
		{
			m_marioStateForTexture = MARIO_TEXTURE::BRAKE_TEXTURE;
		}
		break;
	case Mario::CHANGING_TO_BIG:
		break;
	case Mario::CHANGING_TO_FIRE:
		break;
	case Mario::CHANGING_TO_SMALL:
		break;
	default:
		break;
	}

}

void Mario::SetJump()
{
	if (!m_isJumping)
	{
		if (m_isSit && IsBigMario())
		{
			m_marioStateForTexture = MARIO_TEXTURE::SIT_TEXTURE;
		}
		else
		{
			m_marioStateForTexture = MARIO_TEXTURE::JUMP_TEXTURE;
		}
	}

	m_isJumping = true;
}

Rect Mario::GetCollider()
{
	if (m_curState == MARIO_STATE::DIE)
	{
		return Rect::ZERO;
	}

	Vec2 thisPos = this->getPosition();

	if (m_marioSize != MARIO_SIZE::SMALL_MARIO && !(m_isSit && m_marioStateForTexture == MARIO_TEXTURE::SIT_TEXTURE))
	{
		return Rect(thisPos.x + 10, thisPos.y, 60, 160);
	}
	else
	{
		return Rect(thisPos.x + 10, thisPos.y, 60, 80);
	}
	return Rect(thisPos.x + 10, thisPos.y, 60, 80);
}

void Mario::SetYPosBecauseColl(float y)
{
	if (0 < y)
	{
		if (m_yVelocity < 0)
		{
			m_yVelocity = 0;
			this->setPositionY(this->getPositionY() + y);
			SetJumpEnd();
		}
	}
	else
	{
		this->setPositionY(this->getPositionY() + y);
		SetJumpBlocked();
	}
}

void Mario::EatItem(int itemType)
{
	switch (itemType)
	{
	case Item::NULL_TYPE:
		break;
	case Item::COIN:
		break;
	case Item::STAR:
		m_isInvincible = true;
		m_invincibleCount = 0;
		m_invincibleTextureChangeDeltaTime = 0;
		m_invincibleSustainmentDeltaTime = 0;
		m_invincibleTextureChangeTime = m_invincibleTextureChangeTimeFast;
		break;
	case Item::MUSHROOM:
		ChangeMarioState(MARIO_STATE::CHANGING_TO_BIG);
		break;
	case Item::FLOWER:
		if (m_marioSize != MARIO_SIZE::FIRE_MARIO)
		{
			ChangeMarioState(MARIO_STATE::CHANGING_TO_FIRE);
		}
		break;
	default:
		break;
	}
}

bool Mario::IsBigMario()
{
	return m_marioSize != MARIO_SIZE::SMALL_MARIO;
}

bool Mario::IsInvincible()
{
	return m_isInvincible;
}

bool Mario::IsJumping()
{
	if (0 < m_yVelocity)
	{
		return true;
	}

	return false;
}

bool Mario::IsDroping()
{
	if (m_yVelocity < 0 && m_isJumping && !m_isChangeWhenJumping)
	{
		return true;
	}

	return false;
}

void Mario::ChangeWalkTexture(float deltaTime)
{
	if (m_isJumping)
	{
		return;
	}

	m_changeWalkTextureDeltaTime += deltaTime;
	float u = abs(m_curSpeed) / m_maxSpeed;
	float changeTextureTime = u * m_changeWalkTextureTimeMin + (1 - u) * m_changeWalkTextureTimeMax;

	if (changeTextureTime < m_changeWalkTextureDeltaTime && m_marioStateForTexture != MARIO_TEXTURE::FIRE_TEXTURE)
	{
		m_changeWalkTextureDeltaTime = 0;

		if (m_marioStateForTexture == MARIO_TEXTURE::WALK3_TEXTURE)
		{
			m_marioStateForTexture = MARIO_TEXTURE::WALK1_TEXTURE;
		}
		else
		{
			m_marioStateForTexture = (MARIO_TEXTURE)(m_marioStateForTexture + 1);
		}
	}
}

void Mario::LoadTexture()
{
	Texture2D* textureTemp;
	std::string texturePath;
	String invinStr;

	for (size_t marioSizeIdx = 0; marioSizeIdx < MARIO_SIZE::MARIO_SIZE_END; marioSizeIdx++)
	{
		for (size_t marioTextureIdx = 0; marioTextureIdx < MARIO_TEXTURE::MARIO_TEXTURE_END; marioTextureIdx++)
		{
			for (size_t invinIdx = 0; invinIdx < INVIN_COUNT; invinIdx++)
			{
				if (marioSizeIdx == MARIO_SIZE::SMALL_MARIO && MARIO_TEXTURE::GROW_TEXTURE <= marioTextureIdx)
				{
					continue;
				}

				if (invinIdx > 0 && marioSizeIdx == MARIO_SIZE::FIRE_MARIO)
				{
					continue;
				}

				texturePath = "Mario/";

				switch (marioTextureIdx)
				{
				case MARIO_TEXTURE::IDLE_TEXTURE:
					texturePath += "Idle";
					break;
				case MARIO_TEXTURE::WALK1_TEXTURE:
					texturePath += "Walk1";
					break;
				case MARIO_TEXTURE::WALK2_TEXTURE:
					texturePath += "Walk2";
					break;
				case MARIO_TEXTURE::WALK3_TEXTURE:
					texturePath += "Walk3";
					break;
				case MARIO_TEXTURE::BRAKE_TEXTURE:
					texturePath += "Brake";
					break;
				case MARIO_TEXTURE::JUMP_TEXTURE:
					texturePath += "Jump";
					break;
				case MARIO_TEXTURE::DIE_TEXTURE:
					texturePath += "Die";
					break;
				case MARIO_TEXTURE::FLAG1_TEXTURE:
					texturePath += "Flag1";
					break;
				case MARIO_TEXTURE::FLAG2_TEXTURE:
					texturePath += "Flag2";
					break;
				case MARIO_TEXTURE::GROW_TEXTURE:
					texturePath += "Grow";
					break;
				case MARIO_TEXTURE::FIRE_TEXTURE:
					texturePath += "Fire";
					break;
				case MARIO_TEXTURE::SIT_TEXTURE:
					texturePath += "Sit";
					break;
				default:
					break;
				}

				if (invinIdx > 0)
				{
					invinStr.initWithFormat("Invin%d", invinIdx);
					texturePath += invinStr._string;
				}

				switch (marioSizeIdx)
				{
				case MARIO_SIZE::SMALL_MARIO:
					texturePath += "Small";
					break;
				case MARIO_SIZE::BIG_MARIO:
					texturePath += "Big";
					break;
				case MARIO_SIZE::FIRE_MARIO:
					texturePath += "Fire";
					break;
				default:
					break;
				}

				texturePath += ".png";
				textureTemp = Director::getInstance()->getTextureCache()->addImage(texturePath);
				m_marioTextures[marioSizeIdx][marioTextureIdx].push_back(textureTemp);
			}
		}
	}
}

void Mario::LoadAnimationTextureInfo()
{
	ChangeAnimationTextureInfo tempTextureInfo;
	tempTextureInfo.marioSize = MARIO_SIZE::SMALL_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::GROW_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::SMALL_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::GROW_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::SMALL_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::GROW_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::SMALL_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::GROW_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::SMALL_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[0].push_back(tempTextureInfo);


	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::GROW_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::GROW_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::GROW_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::SMALL_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::GROW_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::SMALL_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::BIG_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
	tempTextureInfo.marioSize = MARIO_SIZE::SMALL_MARIO;
	tempTextureInfo.marioTexture = MARIO_TEXTURE::IDLE_TEXTURE;
	m_changeAnimationTextureInfo[1].push_back(tempTextureInfo);
}

void Mario::SetTexture()
{
	Texture2D* textureTemp;

	if (m_curState != MARIO_STATE::FLAG_DOWNING)
	{
		if (m_reloadDeltaTime < m_reloadTime)
		{
			m_marioStateForTexture = MARIO_TEXTURE::FIRE_TEXTURE;
		}
		else if (m_marioStateForTexture == MARIO_TEXTURE::FIRE_TEXTURE)
		{
			switch (m_curState)
			{
			case MARIO_STATE::IDLE:
				m_marioStateForTexture = MARIO_TEXTURE::IDLE_TEXTURE;
				break;
			case MARIO_STATE::WALK:
				m_marioStateForTexture = MARIO_TEXTURE::WALK1_TEXTURE;
				break;
			case MARIO_STATE::BRAKE:
				m_marioStateForTexture = MARIO_TEXTURE::WALK1_TEXTURE;
				break;
			case MARIO_STATE::BRAKE_FOR_CHANG_DIR:
				m_marioStateForTexture = MARIO_TEXTURE::BRAKE_TEXTURE;
				break;
			default:
				break;
			}
		}
	}

	if (m_invincibleCount > 0)
	{
		if (m_marioStateForTexture == MARIO_TEXTURE::DIE_TEXTURE)
		{
			textureTemp = m_marioTextures[MARIO_SIZE::SMALL_MARIO][m_marioStateForTexture][m_invincibleCount];
		}
		else if (m_marioSizeForTexture == MARIO_SIZE::FIRE_MARIO)
		{
			textureTemp = m_marioTextures[MARIO_SIZE::BIG_MARIO][m_marioStateForTexture][m_invincibleCount];
		}
		else
		{
			textureTemp = m_marioTextures[m_marioSizeForTexture][m_marioStateForTexture][m_invincibleCount];
		}
	}
	else
	{
		textureTemp = m_marioTextures[m_marioSizeForTexture][m_marioStateForTexture][m_invincibleCount];
	}

	Size textureSize = textureTemp->getContentSize();

	m_marioSprite->setTexture(textureTemp);
	m_marioSprite->setContentSize(textureSize);
	m_marioSprite->setTextureRect(Rect(0, 0, textureSize.width, textureSize.height));
}

void Mario::Destroy()
{
	Director::getInstance()->getTextureCache()->removeAllTextures();
}

void Mario::InvincibleUpdate(float deltaTime, bool isChanging)
{
	if (m_isInvincible)
	{
		if (!isChanging)
		{
			m_invincibleSustainmentDeltaTime += deltaTime;
		}
		
		if (m_invincibleSustainmentDeltaTime < m_invincibleSustainmentTime)
		{
			if (m_invincibleSustainmentDeltaTime > 7)
			{
				m_invincibleTextureChangeTime = m_invincibleTextureChangeTimeSlow;
			}

			m_invincibleTextureChangeDeltaTime += deltaTime;
			if (m_invincibleTextureChangeTime < m_invincibleTextureChangeDeltaTime)
			{
				m_invincibleCount++;
				m_invincibleTextureChangeDeltaTime = 0;
				m_invincibleCount %= 4;
			}
		}
		else
		{
			m_isInvincible = false;
			m_invincibleCount = 0;
			m_invincibleTextureChangeDeltaTime = 0;
			m_invincibleSustainmentDeltaTime = 0;
			m_invincibleTextureChangeTime = m_invincibleTextureChangeTimeFast;
		}
	}
}

void Mario::FireFireBall()
{
	if (m_marioSize != MARIO_SIZE::FIRE_MARIO || IsChanging())
	{
		return;
	}

	if (m_reloadDeltaTime < m_reloadTime)
	{
		return;
	}

	Vec2 firePos = this->getPosition();
	int dir = 0;
	firePos.y += 120;
	if (m_marioSprite->isFlippedX())
	{
		dir = -1;
	}
	else
	{
		dir = 1;
		firePos.x += 80;
	}
	

	if (m_mapAnchor->Fire(firePos, dir))
	{
		m_reloadDeltaTime = 0;
	}
}

void Mario::SetMapAnchore(MapAnchor* mapAnchor)
{
	m_mapAnchor = mapAnchor;
}

Mario::MARIO_STATE Mario::GetMarioState()
{
	return m_curState;
}

bool Mario::IsChanging()
{
	if (m_curState == MARIO_STATE::CHANGING_TO_BIG ||
		m_curState == MARIO_STATE::CHANGING_TO_FIRE ||
		m_curState == MARIO_STATE::CHANGING_TO_SMALL ||
		m_curState == MARIO_STATE::DIE ||
		m_curState == MARIO_STATE::DIED ||
		m_curState == MARIO_STATE::FLAG_DOWNING ||
		m_curState == MARIO_STATE::GO_END ||
		m_curState == MARIO_STATE::CLEAR)
	{
		return true;
	}

	return false;
}

Mario::Mario()
{
}


Mario::~Mario()
{
}
