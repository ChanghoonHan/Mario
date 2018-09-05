#pragma once
#include "cocos2d.h"

USING_NS_CC;

#define MAX_BLOCK_HEIGHT 14
class Block;
enum BLOCK_TYPE;
enum ITEM_TYPE;
enum MONSTER_TYPE;

class MapAnchor : public Node
{
	DrawNode* m_drawNode;
	std::vector<std::vector<Block*>> m_blockArrayVector;
	Size m_visibleSize;

	void BackgroundUpdate();
	void UpdateMapVector();

public:
	virtual bool init();

	std::string GetMapToString();
	void UpdateAnchor(float deltaTime);
	void AddBlock(Vec2 mousePos, BLOCK_TYPE blockType, int stackNum = 0, bool isEntranceStack = false);
	void SetItemInBlock(Vec2 mousePos, ITEM_TYPE itemType);
	void SetMonster(Vec2 mousePos, MONSTER_TYPE monsterType);
	void RemoveBlock(Vec2 mousePos);
	void Clear();

	CREATE_FUNC(MapAnchor);

	MapAnchor();
	~MapAnchor();
};

