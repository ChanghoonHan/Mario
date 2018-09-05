#include "MarioMapEditorMainScene.h"
#include "SimpleAudioEngine.h"
#include "proj.win32\MapAnchor.h"
#include "proj.win32\Block.h"

USING_NS_CC;

Scene* MarioMapEditorMainScene::createScene()
{
    return MarioMapEditorMainScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MarioMapEditorMainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    m_visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_curMapAnchorMoveSpeedPerSecond = 0;
	m_mapAnchorMoveSpeedPerSecond = 3000;
	float scale = 3;

	MenuItemFont* saveMenuFont = MenuItemFont::create("SAVE MAP", CC_CALLBACK_1(MarioMapEditorMainScene::SaveMap, this));

	MenuItemImage* selectBlockIcon[23];
	MenuItemImage* selectItemIcon[3];
	MenuItemImage* selectMonsterIcon[3];

	selectBlockIcon[0] = MenuItemImage::create(
        "./Enviroment/Block.png",
        "./Enviroment/Block.png",
        CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::BLOCK));
	selectBlockIcon[1] = MenuItemImage::create(
		"./Enviroment/Ground.png",
		"./Enviroment/Ground.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::GROUND));
	selectBlockIcon[2] = MenuItemImage::create(
		"./Enviroment/EmptyQuestionBox.png",
		"./Enviroment/EmptyQuestionBox.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::QUESTION));
	selectBlockIcon[3] = MenuItemImage::create(
		"./Enviroment/Floor.png",
		"./Enviroment/Floor.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::FLOOR));
	selectBlockIcon[4] = MenuItemImage::create(
		"./Enviroment/CoinOut1.png",
		"./Enviroment/CoinOut1.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::COIN_OUT));
	selectBlockIcon[5] = MenuItemImage::create(
		"./Enviroment/Flag.png",
		"./Enviroment/Flag.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::FLAG));
	selectBlockIcon[6] = MenuItemImage::create(
		"./Enviroment/CloudLeft.png",
		"./Enviroment/CloudLeft.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::CLOUD_LEFT));
	selectBlockIcon[7] = MenuItemImage::create(
		"./Enviroment/CloudRight.png",
		"./Enviroment/CloudRight.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::CLOUD_RIGHT));
	selectBlockIcon[8] = MenuItemImage::create(
		"./Enviroment/CloudCenter.png",
		"./Enviroment/CloudCenter.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::CLOUD_CENTER));
	selectBlockIcon[9] = MenuItemImage::create(
		"./Enviroment/CloudTop.png",
		"./Enviroment/CloudTop.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::CLOUD_TOP));
	selectBlockIcon[10] = MenuItemImage::create(
		"./Enviroment/GrassLeft.png",
		"./Enviroment/GrassLeft.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::GRASS_LEFT));
	selectBlockIcon[11] = MenuItemImage::create(
		"./Enviroment/GrassRight.png",
		"./Enviroment/GrassRight.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::GRASS_RIGHT));
	selectBlockIcon[12] = MenuItemImage::create(
		"./Enviroment/GrassCenter.png",
		"./Enviroment/GrassCenter.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::GRASS_CENTER));
	selectBlockIcon[13] = MenuItemImage::create(
		"./Enviroment/MountainLeft.png",
		"./Enviroment/MountainLeft.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::MOUNTAIN_LEFT));
	selectBlockIcon[14] = MenuItemImage::create(
		"./Enviroment/MountainRight.png",
		"./Enviroment/MountainRight.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::MOUNTAIN_RIGHT));
	selectBlockIcon[15] = MenuItemImage::create(
		"./Enviroment/MountainCenter1.png",
		"./Enviroment/MountainCenter1.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::MOUNTAIN_CENTER1));
	selectBlockIcon[16] = MenuItemImage::create(
		"./Enviroment/MountainCenter2.png",
		"./Enviroment/MountainCenter2.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::MOUNTAIN_CENTER2));
	selectBlockIcon[17] = MenuItemImage::create(
		"./Enviroment/MountainCenter3.png",
		"./Enviroment/MountainCenter3.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::MOUNTAIN_CENTER3));
	selectBlockIcon[18] = MenuItemImage::create(
		"./Enviroment/MountainTop.png",
		"./Enviroment/MountainTop.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::MOUNTAIN_TOP));
	selectBlockIcon[19] = MenuItemImage::create(
		"./Enviroment/StackLeft.png",
		"./Enviroment/StackLeft.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::STACK_LEFT));
	selectBlockIcon[20] = MenuItemImage::create(
		"./Enviroment/StackTop.png",
		"./Enviroment/StackTop.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::STACK_TOP));
	selectBlockIcon[21] = MenuItemImage::create(
		"./Enviroment/StackPillar.png",
		"./Enviroment/StackPillar.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::STACK_PILLAR));
	selectBlockIcon[22] = MenuItemImage::create(
		"./Enviroment/Castle.png",
		"./Enviroment/Castle.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlock, this, BLOCK_TYPE::CASTLE));

	Vec2 buttonPos = Vec2::ZERO;
	for (size_t i = 0; i < 23; i++)
	{
		selectBlockIcon[i]->setScale(scale);
		selectBlockIcon[i]->setPosition(Vec2::ZERO);
		buttonPos.x = 1330 + ((i % 6) * 136);
		buttonPos.y = m_visibleSize.height - 150 - ((i / 6) * 136);
		SetMenuButton(selectBlockIcon[i], buttonPos);
		if (i == 22)
		{
			selectBlockIcon[i]->setScale(0.7f);
		}
	}

	selectItemIcon[0] = MenuItemImage::create(
		"./Enviroment/CoinIn1.png",
		"./Enviroment/CoinIn1.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlockToItem, this, ITEM_TYPE::COIN_IN));
	selectItemIcon[1] = MenuItemImage::create(
		"./Enviroment/Mushroom.png",
		"./Enviroment/Mushroom.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlockToItem, this, ITEM_TYPE::MUSHROOM));
	selectItemIcon[2] = MenuItemImage::create(
		"./Enviroment/Star1.png",
		"./Enviroment/Star1.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlockToItem, this, ITEM_TYPE::STAR));
	for (size_t i = 0; i < 3; i++)
	{
		selectItemIcon[i]->setScale(scale);
		selectItemIcon[i]->setPosition(Vec2::ZERO);
		buttonPos.x = 1330 + (i * 136);
		buttonPos.y = m_visibleSize.height - 750;
		SetMenuButton(selectItemIcon[i], buttonPos);
	}


	selectMonsterIcon[0] = MenuItemImage::create(
		"./Enviroment/Flower1.png",
		"./Enviroment/Flower1.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlockToMonster, this, MONSTER_TYPE::FLOWER));
	selectMonsterIcon[1] = MenuItemImage::create(
		"./Enviroment/KuriIdle1.png",
		"./Enviroment/KuriIdle1.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlockToMonster, this, MONSTER_TYPE::KURI));
	selectMonsterIcon[2] = MenuItemImage::create(
		"./Enviroment/NoriIdle1.png",
		"./Enviroment/NoriIdle1.png",
		CC_CALLBACK_1(MarioMapEditorMainScene::ChangeFollowMouseBlockToMonster, this, MONSTER_TYPE::NORI));
	for (size_t i = 0; i < 3; i++)
	{
		selectMonsterIcon[i]->setScale(scale);
		selectMonsterIcon[i]->setPosition(Vec2::ZERO);
		buttonPos.x = 1330 + (i * 136);
		buttonPos.y = m_visibleSize.height - 850;
		SetMenuButton(selectMonsterIcon[i], buttonPos);
	}


	buttonPos.x = 1400;
	buttonPos.y = m_visibleSize.height - 1050;
	SetMenuButton(saveMenuFont, buttonPos);

	m_mapAnchor = MapAnchor::create();
	this->addChild(m_mapAnchor);

	m_followMouseBlock = Block::create();
	m_followMouseBlock->SetFollowMouseBlock();
	m_followMouseBlock->SetBlockType(BLOCK_TYPE::GROUND);
	this->addChild(m_followMouseBlock);

	DrawNode* itemBoxDraw = DrawNode::create();
	itemBoxDraw->drawSolidRect(Vec2(1280, 0), Vec2(m_visibleSize.width, m_visibleSize.height), Color4F::BLACK);
	itemBoxDraw->drawRect(Vec2(1280, 0), Vec2(m_visibleSize.width, m_visibleSize.height), Color4F::RED);
	this->addChild(itemBoxDraw, 1000);

	auto mouseEvent = EventListenerMouse::create();
	mouseEvent->onMouseDown = CC_CALLBACK_1(MarioMapEditorMainScene::onMouseDown, this);
	mouseEvent->onMouseUp = CC_CALLBACK_1(MarioMapEditorMainScene::onMouseUp, this);
	mouseEvent->onMouseMove = CC_CALLBACK_1(MarioMapEditorMainScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEvent, this);

	auto keyBoardEvent = EventListenerKeyboard::create();
	keyBoardEvent->onKeyPressed = CC_CALLBACK_2(MarioMapEditorMainScene::onKeyPressed, this);
	keyBoardEvent->onKeyReleased = CC_CALLBACK_2(MarioMapEditorMainScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardEvent, this);

	scheduleUpdate();

    return true;
}

void MarioMapEditorMainScene::update(float deltaTime)
{
	m_mapAnchor->UpdateAnchor(deltaTime);
	Vec2 pos = m_mapAnchor->getPosition();
	pos.x += m_curMapAnchorMoveSpeedPerSecond * deltaTime;
	if (pos.x > 0)
	{
		pos.x = 0;
	}

	m_mapAnchor->setPosition(pos);
}

void MarioMapEditorMainScene::onMouseDown(Event* event)
{
	auto mouseEvent = (EventMouse*)event;

	if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
	{
		m_leftMouseButtonDown = true;
		m_rightMouseButtonDown = false;
	}
	else
	{
		m_leftMouseButtonDown = false;
		m_rightMouseButtonDown = true;
	}

	if (mouseEvent->getCursorX() < 0 || mouseEvent->getCursorX() > 1280 ||
		mouseEvent->getCursorY() < 0 || mouseEvent->getCursorY() > m_visibleSize.height)
	{
		return;
	}

	if (m_leftMouseButtonDown)
	{
		BLOCK_TYPE blockType = BLOCK_TYPE::NULL_BLOCK;
		ITEM_TYPE itemType = ITEM_TYPE::NULL_ITEM;
		MONSTER_TYPE monsterType = MONSTER_TYPE::NULL_MONSTER;

		int stackNum = 0;
		bool isEntrance = false;

		m_followMouseBlock->GetAllType(&blockType, &itemType, &monsterType);

		if (blockType == BLOCK_TYPE::STACK_LEFT || blockType == BLOCK_TYPE::STACK_TOP)
		{
			m_followMouseBlock->GetStackNumAndIsEntrance(&stackNum, &isEntrance);
		}

		if (blockType != BLOCK_TYPE::NULL_BLOCK)
		{
			m_mapAnchor->AddBlock(Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY()), blockType, stackNum, isEntrance);
		}
		
		if (itemType != ITEM_TYPE::NULL_ITEM)
		{
			m_mapAnchor->SetItemInBlock(Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY()), itemType);
		}

		if (monsterType != MONSTER_TYPE::NULL_MONSTER)
		{
			m_mapAnchor->SetMonster(Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY()), monsterType);
		}
	}
	
	if(m_rightMouseButtonDown)
	{
		m_mapAnchor->RemoveBlock(Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY()));
	}
}

void MarioMapEditorMainScene::onMouseMove(Event* event)
{
	auto mouseEvent = (EventMouse*)event;

	if (mouseEvent->getCursorX() < 0 || mouseEvent->getCursorX() > 1280 ||
		mouseEvent->getCursorY() < 0 || mouseEvent->getCursorY() > m_visibleSize.height)
	{
		return;
	}

	m_followMouseBlock->setPosition(Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY()));

	if (m_leftMouseButtonDown)
	{
		BLOCK_TYPE blockType = BLOCK_TYPE::NULL_BLOCK;
		ITEM_TYPE itemType = ITEM_TYPE::NULL_ITEM;
		MONSTER_TYPE monsterType = MONSTER_TYPE::NULL_MONSTER;

		int stackNum = 0;
		bool isEntrance = false;

		m_followMouseBlock->GetAllType(&blockType, &itemType, &monsterType);
		if (blockType == BLOCK_TYPE::STACK_LEFT || blockType == BLOCK_TYPE::STACK_TOP)
		{
			m_followMouseBlock->GetStackNumAndIsEntrance(&stackNum, &isEntrance);
		}

		if (blockType != BLOCK_TYPE::NULL_BLOCK)
		{
			m_mapAnchor->AddBlock(Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY()), blockType, stackNum, isEntrance);
		}

		if (itemType != ITEM_TYPE::NULL_ITEM)
		{
			m_mapAnchor->SetItemInBlock(Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY()), itemType);
		}

		if (monsterType != MONSTER_TYPE::NULL_MONSTER)
		{
			m_mapAnchor->SetMonster(Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY()), monsterType);
		}
	}

	if (m_rightMouseButtonDown)
	{
		m_mapAnchor->RemoveBlock(Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY()));
	}
}

void MarioMapEditorMainScene::onMouseUp(Event* event)
{
	auto mouseEvent = (EventMouse*)event;

	if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
	{
		m_leftMouseButtonDown = false;
	}
	
	if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
	{
		m_rightMouseButtonDown = false;
	}
}

void MarioMapEditorMainScene::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	switch (keycode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		m_curMapAnchorMoveSpeedPerSecond += m_mapAnchorMoveSpeedPerSecond;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		m_curMapAnchorMoveSpeedPerSecond -= m_mapAnchorMoveSpeedPerSecond;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Q:
		m_mapAnchor->setPosition(Vec2(0, 40));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_P:
		m_mapAnchor->Clear();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		m_followMouseBlock->IncreaseStackNum();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		m_followMouseBlock->DecreaseStackNum();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F:
		m_followMouseBlock->SetStackEntrance();
		break;
	default:
		break;
	}
}

void MarioMapEditorMainScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	switch (keycode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		m_curMapAnchorMoveSpeedPerSecond -= m_mapAnchorMoveSpeedPerSecond;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		m_curMapAnchorMoveSpeedPerSecond += m_mapAnchorMoveSpeedPerSecond;
		break;
	default:
		break;
	}
}

void MarioMapEditorMainScene::SetMenuButton(MenuItem* menuItemImage, Vec2 pos)
{
	Menu* selectBlockChangeMenu = Menu::create(menuItemImage, NULL);
	selectBlockChangeMenu->setPosition(pos);
	this->addChild(selectBlockChangeMenu, 1001);
}

void MarioMapEditorMainScene::ChangeFollowMouseBlockToItem(cocos2d::Ref* pSender, ITEM_TYPE itemType)
{
	m_followMouseBlock->SetItemType(itemType);
}

void MarioMapEditorMainScene::ChangeFollowMouseBlock(cocos2d::Ref* pSender, BLOCK_TYPE blockType)
{
	m_followMouseBlock->SetBlockType(blockType);
}

void MarioMapEditorMainScene::ChangeFollowMouseBlockToMonster(cocos2d::Ref* pSender, MONSTER_TYPE itemType)
{
	m_followMouseBlock->SetMonsterType(itemType);
}

void MarioMapEditorMainScene::SaveMap(cocos2d::Ref* pSender)
{
	std::string mapString = m_mapAnchor->GetMapToString();

	FILE *file;
	std::string fname = "../map.txt";

	file = fopen(fname.c_str(), "wt");                
	fprintf(file, "%s", mapString.c_str());
	fclose(file);
}