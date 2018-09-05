#include "GameMiddleScene.h"
#include "MarioGameScene.h"
#include "ScoreBoard.h"
#include "MarioTittleScene.h"

USING_NS_CC;


Scene* GameMiddleScene::createScene()
{
	return GameMiddleScene::create();
}

bool GameMiddleScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	m_changeSceneTime = 3.0f;
	m_changeSceneDeltaTime = 0;
	m_isGameClear = false;
	scheduleUpdate();

	return true;
}

void GameMiddleScene::SetScoreBoard()
{
	ScoreBoard::GetInstance()->SetTimeBlank();
	this->addChild(ScoreBoard::GetInstance());

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float scale = 5.0f;

	if (m_isGameClear)
	{
		int m_fontSize = 40;
		std::string m_fontPath = "./fonts/Super Mario Bros. NES.ttf";
		LabelTTF* gameClearLabel = LabelTTF::create("GAME CLEAR!", m_fontPath, m_fontSize);
		gameClearLabel->setAnchorPoint(Vec2(0.5f, 0));
		gameClearLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(gameClearLabel);
	}
	else if (ScoreBoard::GetInstance()->GetLife() == 0)
	{
		int m_fontSize = 40;
		std::string m_fontPath = "./fonts/Super Mario Bros. NES.ttf";
		LabelTTF* gameOverLabel = LabelTTF::create("GAME OVER", m_fontPath, m_fontSize);
		gameOverLabel->setAnchorPoint(Vec2(0.5f, 0));
		gameOverLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(gameOverLabel);
	}
	else
	{
		Sprite* marioSprite = Sprite::create("Mario/IdleSmall.png");
		marioSprite->setScale(scale);
		marioSprite->setAnchorPoint(Vec2(0.5f, 0));
		marioSprite->setPosition(Vec2(visibleSize.width / 2 - 80, visibleSize.height / 2));
		this->addChild(marioSprite);

		Sprite* xUISprite = Sprite::create("UI/X.png");
		xUISprite->setScale(scale);
		xUISprite->setAnchorPoint(Vec2(0.5f, 0));
		xUISprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(xUISprite);

		int m_fontSize = 40;
		std::string m_fontPath = "./fonts/Super Mario Bros. NES.ttf";
		LabelTTF* worldLabel = LabelTTF::create("WORLD 1-1", m_fontPath, m_fontSize);
		worldLabel->setAnchorPoint(Vec2(0.5f, 0));
		worldLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 160));
		this->addChild(worldLabel);

		String lifeString;
		lifeString.initWithFormat("%d", ScoreBoard::GetInstance()->GetLife());
		LabelTTF* lifeLabel = LabelTTF::create(lifeString._string, m_fontPath, m_fontSize);
		lifeLabel->setAnchorPoint(Vec2(0.5f, 0));
		lifeLabel->setPosition(Vec2(visibleSize.width / 2 + 120, visibleSize.height / 2));
		this->addChild(lifeLabel);
	}
}

void GameMiddleScene::update(float deltaTime)
{
	m_changeSceneDeltaTime += deltaTime;
	if (m_changeSceneTime < m_changeSceneDeltaTime)
	{
		if (ScoreBoard::GetInstance()->GetLife() == 0 || m_isGameClear)
		{
			ScoreBoard::Destroy();
			MarioTittleScene* marioTittleScene = MarioTittleScene::create();
			Director::getInstance()->replaceScene(marioTittleScene);
		}
		else
		{
			this->removeChild(ScoreBoard::GetInstance(), false);
			MarioGameScene* marioGameScene = MarioGameScene::create();
			ScoreBoard::GetInstance()->DecreaseLife();
			ScoreBoard::GetInstance()->InitTime();
			marioGameScene->SetScoreBoard();
			if (ScoreBoard::GetInstance()->IsTouchSavePoint())
			{
				marioGameScene->LoadSavePoint();
			}
			Director::getInstance()->replaceScene(marioGameScene);
		}
	}
}

void GameMiddleScene::SetGameClear()
{
	m_isGameClear = true;
}

GameMiddleScene::GameMiddleScene()
{
}


GameMiddleScene::~GameMiddleScene()
{
}
