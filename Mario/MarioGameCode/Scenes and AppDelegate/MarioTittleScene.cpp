#include "MarioTittleScene.h"
#include "proj.win32\MarioGameScene.h"
#include "proj.win32\GameMiddleScene.h"
#include "SimpleAudioEngine.h"
#include "proj.win32\ScoreBoard.h"
#include "proj.win32\MapAnchor.h"
#include "proj.win32\Mario.h"

USING_NS_CC;

Scene* MarioTittleScene::createScene()
{
    return MarioTittleScene::create();
}

// on "init" you need to initialize your instance
bool MarioTittleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_mapAnchor = MapAnchor::create();
	m_mapAnchor->SetMap("./MapText/TittleMap.txt");
	this->addChild(m_mapAnchor);

	m_mainTittleSprite = Sprite::create("./UI/MainTittle.png");
	m_mainTittleSprite->setScale(5.0f);
	m_mainTittleSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height -400));

	int m_fontSize = 40;
	std::string m_fontPath = "./fonts/Super Mario Bros. NES.ttf";
	LabelTTF* player1Label = LabelTTF::create("1 PLAYER GAME", m_fontPath, m_fontSize);
	LabelTTF* player2Label = LabelTTF::create("2 PLAYER GAME", m_fontPath, m_fontSize);
	LabelTTF* topScoreLabel = LabelTTF::create("TOP- 000000", m_fontPath, m_fontSize);
	this->addChild(player1Label);
	this->addChild(player2Label);
	this->addChild(topScoreLabel);
	player1Label->setPosition(Vec2(visibleSize.width / 2, 425));
	player2Label->setPosition(Vec2(visibleSize.width / 2, 325));
	topScoreLabel->setPosition(Vec2(visibleSize.width / 2, 200));

	Sprite* cursorUISprite = Sprite::create("./UI/Cursor.png");
	cursorUISprite->setScale(5.0f);
	this->addChild(cursorUISprite);
	cursorUISprite->setPosition(Vec2(310, 425));

	this->addChild(m_mainTittleSprite);

	ScoreBoard* scoreBoard = ScoreBoard::GetInstance();
	this->addChild(scoreBoard);

	auto keyBoardEventListener = EventListenerKeyboard::create();
	keyBoardEventListener->onKeyPressed = CC_CALLBACK_2(MarioTittleScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardEventListener, this);

    return true;
}

void MarioTittleScene::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_Z)
	{
		ScoreBoard::Destroy();
		/*MarioGameScene* marioGameScene = MarioGameScene::create();
		marioGameScene->SetScoreBoard();*/
		GameMiddleScene* gameMiddleScene = GameMiddleScene::create();
		gameMiddleScene->SetScoreBoard();
		Director::getInstance()->replaceScene(gameMiddleScene);
	}
}

MarioTittleScene::MarioTittleScene()
{
	
}

MarioTittleScene::~MarioTittleScene()
{

}

