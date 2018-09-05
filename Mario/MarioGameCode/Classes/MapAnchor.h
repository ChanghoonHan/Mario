#pragma once
#include <cocos2d.h>

class BlockBase;
class Mario;
class Item;
class Monster;
class FireBall;
class Flag;
class Castle;

class MapAnchor : public cocos2d::Node
{
#define MAX_FIRE_BALL_COUNT 2

	enum SORT_ORDER;

	struct AdjoinBlock
	{
		enum ADJOIN_BLOCK_STATE
		{
			VERTICAL,
			HORIZONTAL,
		};
		BlockBase* block;
		cocos2d::Rect collRect;
	};

	struct ItemInfo
	{
		Item* item;
		AdjoinBlock adjoinBlock[2];
	};

	struct MonsterInfo
	{
		Monster* monster;
		AdjoinBlock adjoinBlock[2];
	};

	struct FireBallInfo
	{
		FireBall* fireBall;
		AdjoinBlock adjoinBlock[2];
	};

	AdjoinBlock m_mariosAdjoinBlock[2];
	Mario* m_mario;
	FireBallInfo m_fireBallInfos[MAX_FIRE_BALL_COUNT];
	std::vector<MonsterInfo> m_monsterInfos;

	Flag* m_flag;
	Castle* m_castle;

	int m_fireBallIdx;
	int m_pillarCount;
	int m_floorCount;
	int m_checkCollStartIdx;
	int m_visibleBlockWidthCount;
	int m_overCheckCollLineCount;

	int m_oneBlockSize;
	float m_mapAnchorEndXpos;
	float m_savePoint;

	float m_changeSceneTime;
	float m_changeSceneDeltaTime;

	std::vector<std::vector<BlockBase*>> m_blocks;
	std::vector<ItemInfo> m_itemInfos;
	cocos2d::Size m_visibleSize;

	cocos2d::DrawNode* m_background;
	cocos2d::Color4F m_backgroundColor;

	void DrawBackground();

	void MoveMapAnchor(float dist);
	void SetCheckCollStartIdx();
	void StringSplit(std::string strOrigin, char strTok, std::vector<std::string>* resultStringVec);
	void CheckCollBlocksWithOtherObj();
	void SetAdjoinBlock(BlockBase* block, cocos2d::Rect blockCollRect, AdjoinBlock* adjoinBlock, int widthClearance = 20);
	void ClearAdjoinBlock();
	void ReleaseObj();

public:
	enum SORT_ORDER
	{
		BACKGROUND,
		ENVIRONMENT_BLOCK,
		GROUND_BLOCK,
		ITEM_STAY,
		MONSETER_BLOCK,
		INTERACTION_BLOCK,
		MARIO,
		ITEM_IDLE,
		FIRE_BALL,
	};

	virtual bool init();

	void SetMap(std::string mapTextFilePath);
	void LoadSavePoint();

	void UpdateMapAnchor(float deltaTime);

	bool Fire(cocos2d::Vec2 startPos, int dir);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	CREATE_FUNC(MapAnchor);

	MapAnchor();
	~MapAnchor();
};

