#include "Flag.h"
#include "ScoreBoard.h"
#include "Mario.h"

USING_NS_CC;

bool Flag::init()
{
	BlockBase::init();
	m_mario = NULL;
	m_flagHeight = 9;
	m_score = 0;

	m_curFlagState = FLAG_STATE::IDLE;

	Sprite* spriteTemp;

	std::string labelPath = "./fonts/Super Mario Bros. NES.ttf";
	int labelSize = 20;
	m_scoreLabel = LabelTTF::create("", labelPath, labelSize);
	m_scoreLabel->setVisible(false);
	m_scoreLabel->setPosition(Vec2(100, 100));
	this->addChild(m_scoreLabel);

	spriteTemp = Sprite::create("Interaction/Floor.png");
	spriteTemp->setAnchorPoint(Vec2::ZERO);
	spriteTemp->setPosition(Vec2::ZERO);
	spriteTemp->setScale(m_scale);
	this->addChild(spriteTemp);

	for (int i = 0; i < m_flagHeight; i++)
	{
		spriteTemp = Sprite::create("Interaction/FlagPillar.png");
		spriteTemp->setAnchorPoint(Vec2::ZERO);
		spriteTemp->setPosition(Vec2(35, m_blockSize * (i + 1)));
		spriteTemp->setScale(m_scale);
		this->addChild(spriteTemp);
	}

	spriteTemp = Sprite::create("Interaction/FlagTop.png");
	spriteTemp->setAnchorPoint(Vec2::ZERO);
	spriteTemp->setPosition(Vec2(20, m_blockSize * (m_flagHeight + 1)));
	spriteTemp->setScale(m_scale);
	this->addChild(spriteTemp);

	m_flagSprite = Sprite::create("Interaction/Flag.png");
	m_flagSprite->setAnchorPoint(Vec2::ZERO);
	m_flagSprite->setPosition(Vec2(-40, m_blockSize * m_flagHeight));
	m_flagSprite->setScale(m_scale);
	this->addChild(m_flagSprite);

	m_flagCollider = Rect::ZERO;
	m_flagTopCollider = Rect::ZERO;

	return true;
}

bool Flag::CheckCollFlagWithMario(Mario* mario)
{
	if (m_curFlagState != FLAG_STATE::IDLE)
	{
		return false;
	}

	m_mario = mario;
	Rect marioCollider = m_mario->GetCollider();

	if (m_flagTopCollider.intersectsRect(marioCollider))
	{
		m_curFlagState = FLAG_STATE::DONWING;
		m_score = 5000;
		DownFlag();
		return true;
	}

	if (m_flagCollider.intersectsRect(marioCollider))
	{
		float score = (marioCollider.getMinY() - this->getPositionY() - 80) / ((m_flagHeight - 1) * 80 - 40);
		score *= 100;
		score = (int)score;
		score = (int)(score / 5);
		m_score = score * 100;
		if (m_score < 100)
		{
			m_score = 100;
		}
		m_curFlagState = FLAG_STATE::DONWING;
		DownFlag();
		return true;
	}

	return false;
}

void Flag::DownFlag()
{
	String scoreStr;
	scoreStr.initWithFormat("%d", m_score);
	m_scoreLabel->setString(scoreStr._string);

	CallFuncN* endDownCallback = CallFuncN::create(CC_CALLBACK_1(Flag::EndDownFlagCallback, this));
	m_flagSprite->runAction(Sequence::create(MoveBy::create(2.0f, Vec2(0, -80 * (m_flagHeight - 1))), endDownCallback, NULL));
	m_scoreLabel->setVisible(true);
	m_scoreLabel->runAction(MoveBy::create(2.0f, Vec2(0, 80 * (m_flagHeight - 1) + 40)));
}

void Flag::EndDownFlagCallback(Node* obj)
{
	ScoreBoard::GetInstance()->IncreaseScore(m_score);
	m_mario->SetGoToCastle(this->getPositionX() + 80 * 6);
}

void Flag::SetBlockPos(cocos2d::Vec2 pos)
{
	BlockBase::SetBlockPos(pos);

	m_flagCollider.setRect(this->getPositionX() + 30, this->getPositionY() + 80, 20, m_blockSize * m_flagHeight);
	m_flagTopCollider.setRect(this->getPositionX() + 20, this->getPositionY() + 80 + m_blockSize * m_flagHeight, 40, 40);
}

Flag::Flag()
{
}


Flag::~Flag()
{
}
