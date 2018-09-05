#include "Castle.h"

USING_NS_CC;

bool Castle::init()
{
	EnvironmentBlock::init();
	
	m_flagSprite = Sprite::create("Environment/CastleFlag.png");
	m_flagSprite->setScale(m_scale);
	this->addChild(m_flagSprite, -1);
	m_flagSprite->setPosition(Vec2(200, 335));

	m_fireworkSprite = Sprite::create("Mario/FireExplode1.png");
	m_fireworkSprite->setScale(m_scale);
	this->addChild(m_fireworkSprite, -1);
	m_fireworkSprite->setVisible(false);

	m_fireworkAnimation = Animation::create();
	m_fireworkAnimation->addSpriteFrameWithFileName("Mario/FireExplode1.png");
	m_fireworkAnimation->addSpriteFrameWithFileName("Mario/FireExplode2.png");
	m_fireworkAnimation->addSpriteFrameWithFileName("Mario/FireExplode3.png");
	m_fireworkAnimation->setDelayPerUnit(0.1f);
	m_fireworkAnimation->retain();

	m_fireworkAnimate = Animate::create(m_fireworkAnimation);
	m_fireworkAnimate->retain();

	m_setNoVisibleFireworkCallback = CallFuncN::create(CC_CALLBACK_1(Castle::SetNoVisibleFireworkCallback, this));
	m_setNoVisibleFireworkCallback->retain();

	m_fireworkPosIdx = 0;
	m_fireworkReloadTime = 0.8f;
	m_fireworkReloadDeltaTime = m_fireworkReloadTime;

	m_fireworkPositions.push_back(Vec2(200, 800));
	m_fireworkPositions.push_back(Vec2(600, 500));
	m_fireworkPositions.push_back(Vec2(-100, 600));
	m_fireworkPositions.push_back(Vec2(500, 700));
	m_fireworkPositions.push_back(Vec2(-150, 500));
	m_fireworkPositions.push_back(Vec2(400, 600));

	m_isStartCong = false;
	m_isFinishCong = false;


	return true;
}

void Castle::SetBlock(ENVIRONMENT_BLOCK_TYPE blockType, int sortOrder)
{
	EnvironmentBlock::SetBlock(blockType, sortOrder);
}

void Castle::SetCongratulation(float deltaTime)
{
	if (m_isFinishCong)
	{
		return;
	}

	if (m_isStartCong)
	{
		m_fireworkReloadDeltaTime += deltaTime;
		if (m_fireworkReloadTime < m_fireworkReloadDeltaTime)
		{
			m_fireworkReloadDeltaTime = 0;
			SetFirework(m_fireworkPositions[m_fireworkPosIdx]);
			m_fireworkPosIdx++;
			if (m_fireworkPosIdx == m_fireworkPositions.size())
			{
				m_isFinishCong = true;
			}
		}
	}
	else
	{
		m_isStartCong = true;
		m_flagSprite->runAction(MoveBy::create(1.0f, Vec2(0, 100)));
	}
}

bool Castle::IsFinishCong()
{
	return m_isFinishCong;
}

void Castle::SetFirework(Vec2 pos)
{
	m_fireworkSprite->setTexture("Mario/FireExplode1.png");
	m_fireworkSprite->setPosition(pos);
	m_fireworkSprite->setVisible(true);
	m_fireworkSprite->runAction(Sequence::create(m_fireworkAnimate, m_setNoVisibleFireworkCallback, NULL));
}

void Castle::SetNoVisibleFireworkCallback(Node* obj)
{
	m_fireworkSprite->setVisible(false);
}

void Castle::ReleaseObj()
{
	m_fireworkAnimate->release();
	m_fireworkAnimation->release();
	m_setNoVisibleFireworkCallback->release();
}

Castle::Castle()
{
}


Castle::~Castle()
{
}
