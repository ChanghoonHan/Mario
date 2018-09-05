#include "ScoreBoard.h"

USING_NS_CC;

ScoreBoard* ScoreBoard::S = NULL;

bool ScoreBoard::init()
{
	m_visibleSize = Director::getInstance()->getVisibleSize();

	this->setAnchorPoint(Vec2(0, 1));
	this->setPosition(Vec2(0, m_visibleSize.height));

	m_isTouchSavePoint = false;

	m_changeTimeToScoreTime = 0.02f;
	m_changeTimeToScoreDeltaTime = 0;

	m_score = 0;
	m_coin = 0;
	m_worldLevel = 1;
	m_dungeonLevel = 1;
	m_life = 3;
	m_scorePerSec = 50;
	InitTime();

	m_fontSize = 40;
	m_fontPath = "./fonts/Super Mario Bros. NES.ttf";

	m_scoreBoardLabels[LABEL_TYPE::MARIO] = LabelTTF::create("MARIO", m_fontPath, m_fontSize);
	m_scoreBoardLabels[LABEL_TYPE::SCORE] = LabelTTF::create(String::createWithFormat("%06d", 0)->_string, m_fontPath, m_fontSize);
	m_scoreBoardLabels[LABEL_TYPE::COIN] = LabelTTF::create(String::createWithFormat("%02d", 0)->_string, m_fontPath, m_fontSize);
	m_scoreBoardLabels[LABEL_TYPE::WORLD] = LabelTTF::create("WORLD", m_fontPath, m_fontSize);
	m_scoreBoardLabels[LABEL_TYPE::WORLDNUM] = LabelTTF::create(String::createWithFormat("%d-%d", m_worldLevel, m_dungeonLevel)->_string, m_fontPath, m_fontSize);
	m_scoreBoardLabels[LABEL_TYPE::TIME] = LabelTTF::create("TIME", m_fontPath, m_fontSize);
	m_scoreBoardLabels[LABEL_TYPE::TIMENUM] = LabelTTF::create("", m_fontPath, m_fontSize);

	for (size_t i = 0; i < LABEL_TYPE::ENDOFENUM; i++)
	{
		this->addChild(m_scoreBoardLabels[i]);
		m_scoreBoardLabels[i]->setAnchorPoint(Vec2(0, 1));
	}

	float scale = 5.0f;
	m_coinUISprite = Sprite::create("./UI/CoinUI1.png");
	m_coinUISprite->setAnchorPoint(Vec2(0, 1));
	m_coinUISprite->setScale(scale);
	this->addChild(m_coinUISprite);
	Animation* coinUIAnimation = Animation::create();
	coinUIAnimation->addSpriteFrameWithFileName("./UI/CoinUI3.png");
	coinUIAnimation->addSpriteFrameWithFileName("./UI/CoinUI2.png");
	coinUIAnimation->addSpriteFrameWithFileName("./UI/CoinUI1.png");
	coinUIAnimation->addSpriteFrameWithFileName("./UI/CoinUI1.png");
	coinUIAnimation->addSpriteFrameWithFileName("./UI/CoinUI2.png");
	coinUIAnimation->setDelayPerUnit(0.15f);
	Animate* coinUIAnimate = Animate::create(coinUIAnimation);
	m_coinUISprite->runAction(RepeatForever::create(coinUIAnimate));

	m_xUISprite = Sprite::create("./UI/X.png");
	m_xUISprite->setAnchorPoint(Vec2(0, 1));
	m_xUISprite->setScale(scale);
	this->addChild(m_xUISprite);

	m_scoreBoardLabels[LABEL_TYPE::MARIO]->setPosition(Vec2(70, -50));
	m_scoreBoardLabels[LABEL_TYPE::SCORE]->setPosition(Vec2(70, -100));
	m_coinUISprite->setPosition(Vec2(410, -100));
	m_xUISprite->setPosition(Vec2(450, -110));
	m_scoreBoardLabels[LABEL_TYPE::COIN]->setPosition(Vec2(490, -100));
	m_scoreBoardLabels[LABEL_TYPE::WORLD]->setPosition(Vec2(720, -50));
	m_scoreBoardLabels[LABEL_TYPE::WORLDNUM]->setAnchorPoint(Vec2(0.5, 1));
	m_scoreBoardLabels[LABEL_TYPE::WORLDNUM]->setPosition(Vec2(820, -100));
	m_scoreBoardLabels[LABEL_TYPE::TIME]->setPosition(Vec2(1050, -50));
	m_scoreBoardLabels[LABEL_TYPE::TIMENUM]->setAnchorPoint(Vec2(0.5, 1));
	m_scoreBoardLabels[LABEL_TYPE::TIMENUM]->setPosition(Vec2(1135, -100));

	return true;
}

void ScoreBoard::UpdateScoreBoard(float deltaTime)
{
	m_time -= deltaTime;
	if (m_time < 0)
	{
		m_time = 0;
	}
	String timeStr;
	timeStr.initWithFormat("%03d", (int)m_time);
	m_scoreBoardLabels[LABEL_TYPE::TIMENUM]->setString(timeStr._string);
}

void ScoreBoard::InitTime()
{
	m_time = 15;
}

float ScoreBoard::GetTime()
{
	return m_time;
}

void ScoreBoard::SetTimeBlank()
{
	m_scoreBoardLabels[LABEL_TYPE::TIMENUM]->setString("");
}

ScoreBoard* ScoreBoard::GetInstance()
{
	if (S == NULL)
	{
		S = ScoreBoard::create();
		S->retain();
	}

	return S;
}

void ScoreBoard::Destroy()
{
	if (S == NULL)
	{
		return;
	}

	S->getParent()->removeChild(S);
	S->release();
	S = NULL;
}

void ScoreBoard::IncreaseScore(int score)
{
	m_score += score;
	String scoreStr;
	scoreStr.initWithFormat("%06d", m_score);
	m_scoreBoardLabels[LABEL_TYPE::SCORE]->setString(scoreStr._string);
}

void ScoreBoard::IncreaseCoinCount()
{
	m_coin++;
	String coinStr;
	coinStr.initWithFormat("%02d", m_coin);
	m_scoreBoardLabels[LABEL_TYPE::COIN]->setString(coinStr._string);
}

void ScoreBoard::ChangeTimeToScore(float deltaTime)
{
	m_changeTimeToScoreDeltaTime += deltaTime;
	if (m_changeTimeToScoreTime < m_changeTimeToScoreDeltaTime && 0 < m_time)
	{
		m_time -= 1;
		if (m_time < 0)
		{
			m_time = 0;
		}

		String timeStr;
		timeStr.initWithFormat("%03d", (int)m_time);
		m_scoreBoardLabels[LABEL_TYPE::TIMENUM]->setString(timeStr._string);
		m_changeTimeToScoreDeltaTime = 0;

		IncreaseScore(m_scorePerSec);
	}
}

int ScoreBoard::GetLife()
{
	return m_life;
}

void ScoreBoard::DecreaseLife()
{
	m_life--;
	if (m_life < 0)
	{
		m_life = 0;
	}
}

void ScoreBoard::TouchSavePoint()
{
	m_isTouchSavePoint = true;
}

bool ScoreBoard::IsTouchSavePoint()
{
	return m_isTouchSavePoint;
}

void SetFontSize()
{

}

ScoreBoard::ScoreBoard()
{
}


ScoreBoard::~ScoreBoard()
{
}
