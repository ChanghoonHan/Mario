#pragma once
#include "BlockBase.h"
#include "InCoin.h"

class InCoin;
class Monster;

class InteractionBlock : public BlockBase
{
	enum INTERACTION_BLOCK_TYPE;
	enum INTERACTION_BLOCK_ITEM_TYPE;

	INTERACTION_BLOCK_ITEM_TYPE m_innerItemType;
	int m_coinCount;
	INTERACTION_BLOCK_TYPE m_interType;
	cocos2d::Sequence* m_attackedActionWithEndCallBack;
	cocos2d::Sequence* m_attackedAction;
	cocos2d::Color4F m_backgroundColor;
	cocos2d::DrawNode* m_background;

	bool m_isCollidedItemChangeDir;
	bool m_isCollidedMonsterChangeDir;

	Item* m_collidedItem;
	Monster* m_collidedMonster;

	void EndAttackAction(Node* obj);
	void InstatiateBlockChip();

	std::vector<Item*> m_inItems;
	int m_inItemsIdx;

	void SetJumpCollidedObj();

public:
	enum INTERACTION_BLOCK_TYPE
	{
		NULL_INTERACTION_BLOCK,
		BLOCK,
		QUESTION_BOX,
		FAKE_QUESTION_BOX,
	};

	enum INTERACTION_BLOCK_ITEM_TYPE
	{
		NULL_ITEM,
		COIN,
		MUSHROOM,
		STAR,
	};

	virtual bool init();
	virtual void AttackedFromBelow(bool isBigMario);

	void SetBackZOrder();
	void SetBlock(INTERACTION_BLOCK_TYPE blockType, int sortOrder);
	void SetItem(INTERACTION_BLOCK_ITEM_TYPE itemType, Item* item, int coinCount = 0);
	virtual void SetCollidedItem(Item* item, bool isChangeDir);
	virtual void SetCollidedMonster(Monster* monster, bool isChangeDir);
	virtual void SetBlockPos(cocos2d::Vec2 pos);
	virtual void LateUpdateBlock(float deltaTime);
	virtual void ReleaseObj();


	CREATE_FUNC(InteractionBlock);

	InteractionBlock();
	~InteractionBlock();
};

