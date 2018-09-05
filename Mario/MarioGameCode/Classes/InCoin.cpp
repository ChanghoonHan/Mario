#include "InCoin.h"
#include "InteractionBlock.h"
#include "ScoreBoard.h"

USING_NS_CC;

bool InCoin::init()
{
	Item::init();
	m_itemType = ITEM_TYPE::COIN;
	m_itemSprite = Sprite::create("./Item/CoinIn1.png");
	m_itemSprite->setScale(m_scale);
	m_itemSprite->setVisible(false);

	m_score = 200;
	String scoreStr;
	scoreStr.initWithFormat("%d", m_score);
	m_scoreLabel->setString(scoreStr._string);
	m_scoreLabel->setVisible(false);

	this->addChild(m_itemSprite);

	return true;
}

void InCoin::Spawn(InteractionBlock* interBlock)
{
	m_parentBlock = interBlock;
	ScoreBoard::GetInstance()->IncreaseCoinCount();

	m_itemSprite->setVisible(true);
	Animation* inCoinAnimation = Animation::create();
	inCoinAnimation->addSpriteFrameWithFileName("./Item/CoinIn1.png");
	inCoinAnimation->addSpriteFrameWithFileName("./Item/CoinIn2.png");
	inCoinAnimation->addSpriteFrameWithFileName("./Item/CoinIn3.png");
	inCoinAnimation->addSpriteFrameWithFileName("./Item/CoinIn4.png");
	inCoinAnimation->setDelayPerUnit(0.05f);
	Animate* inCoinAnimate = Animate::create(inCoinAnimation);
	RepeatForever* inCoinAnimateRepeatForever = RepeatForever::create(inCoinAnimate);
	JumpBy* jumpByAction = JumpBy::create(0.5f, Vec2(0, 160), 200, 1);
	CallFuncN* scoreLabelCallback = CallFuncN::create(CC_CALLBACK_1(InCoin::DestroyCallback, this));

	m_itemSprite->runAction(inCoinAnimateRepeatForever);
	this->runAction(Sequence::create(jumpByAction, scoreLabelCallback, NULL));
}

void InCoin::DestroyCallback(cocos2d::Node* obj)
{
	Item::DestroyCallback(obj);
	m_parentBlock->SetBackZOrder();
}

InCoin::InCoin()
{
}

InCoin::~InCoin()
{
}
