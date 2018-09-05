#pragma once
#include <cocos2d.h>

USING_NS_CC;

enum BLOCK_TYPE
{
	NULL_BLOCK,
	BLOCK,
	GROUND,
	FLOOR,
	QUESTION,
	STACK_LEFT,
	STACK_TOP,
	STACK_PILLAR,
	FLAG,
	COIN_OUT,
	GRASS_LEFT,
	GRASS_RIGHT,
	GRASS_CENTER,
	CLOUD_LEFT,
	CLOUD_RIGHT,
	CLOUD_CENTER,
	CLOUD_TOP,
	MOUNTAIN_LEFT,
	MOUNTAIN_RIGHT,
	MOUNTAIN_CENTER1,
	MOUNTAIN_CENTER2,
	MOUNTAIN_CENTER3,
	MOUNTAIN_TOP,
	CASTLE
};

enum ITEM_TYPE
{
	NULL_ITEM,
	COIN_IN,
	MUSHROOM,
	STAR,
};

enum MONSTER_TYPE
{
	NULL_MONSTER,
	FLOWER,
	KURI,
	NORI,
};

class Block : public Node
{
	float m_scale;
	int m_sortOrder;
	int m_stackNum;
	int m_coinCount;
	bool m_isEntranceStack;
	bool m_isFollowMouseBlock;
	std::string m_blockStr;
	std::vector<Sprite*> m_blockSprites;
	Label* m_infoLabel;
	Sprite* m_itemSprite;
	Sprite* m_monsterSprite;
	BLOCK_TYPE m_curBlockType;
	ITEM_TYPE m_curItemType;
	MONSTER_TYPE m_curMonsterType;

	void SetStackLabel();

public:

	bool init();
	void SetFollowMouseBlock();
	
	void SetBlockType(BLOCK_TYPE blockType, int stackNum = 0, bool isEntranceStack = false);
	void SetItemType(ITEM_TYPE itemType);
	void SetMonsterType(MONSTER_TYPE monsterType);

	void GetAllType(BLOCK_TYPE* blockType, ITEM_TYPE* itemType = NULL, MONSTER_TYPE* monsterType = NULL);
	BLOCK_TYPE getBlockType();
	ITEM_TYPE getItemType();
	MONSTER_TYPE getMosnterType();

	void GetStackNumAndIsEntrance(int* stackNum, bool* isEntrance);

	void IncreaseStackNum();
	void DecreaseStackNum();
	void SetStackEntrance();
	std::string GetBlockString();
	
	CREATE_FUNC(Block);

	Block();
	~Block();
};