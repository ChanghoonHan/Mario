#include "MarioGameScene.h"
#include "SimpleAudioEngine.h"
#include "proj.win32\ScoreBoard.h"
#include "proj.win32\MapAnchor.h"
#include "proj.win32\Mario.h"

USING_NS_CC;

Scene* MarioGameScene::createScene()
{
	return MarioGameScene::create();
}

bool MarioGameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_mapAnchor = MapAnchor::create();
	m_mapAnchor->SetMap("./MapText/GameMapTest.txt");
	this->addChild(m_mapAnchor);

	m_scoreBoard = NULL;

	auto keyBoardEventListener = EventListenerKeyboard::create();
	keyBoardEventListener->onKeyPressed = CC_CALLBACK_2(MarioGameScene::onKeyPressed, this);
	keyBoardEventListener->onKeyReleased = CC_CALLBACK_2(MarioGameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardEventListener, this);

	scheduleUpdate();

	return true;
}

void MarioGameScene::SetScoreBoard()
{
	m_scoreBoard = ScoreBoard::GetInstance();
	this->addChild(ScoreBoard::GetInstance());

	return;
}

void MarioGameScene::update(float deltaTime)
{
	if (deltaTime > 1.0f / 30)
	{
		return;
	}

	if (m_scoreBoard != NULL)
	{
		m_mapAnchor->UpdateMapAnchor(deltaTime);
	}
}

void MarioGameScene::LoadSavePoint()
{
	m_mapAnchor->LoadSavePoint();
}

void MarioGameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	m_mapAnchor->onKeyPressed(keycode, event);
}

void MarioGameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	m_mapAnchor->onKeyReleased(keycode, event);
}

MarioGameScene::MarioGameScene()
{
}


MarioGameScene::~MarioGameScene()
{
}
