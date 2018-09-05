#pragma once
#include <cocos2d.h>

class Item;
class Monster;

class BlockBase : public cocos2d::Node
{
protected:
	enum BLOCK_STATE;

	BLOCK_STATE m_curBlockState;

	float m_scale;
	int m_sortOrder;
	int m_blockSize;

	cocos2d::Rect m_collider;
	std::vector<cocos2d::Sprite*> m_blockSprites;

	cocos2d::Rect CalCollRect(cocos2d::Rect rect1, cocos2d::Rect rect2);

public:
	enum BLOCK_STATE
	{
		IDLE,
		ATTACKED,
		DESTROYED,
	};

	virtual bool init();

	virtual bool CheckColl(cocos2d::Rect collider, cocos2d::Rect* collisionRect = NULL);
	virtual void SetCollidedItem(Item* item, bool isChangeDir);
	virtual void SetCollidedMonster(Monster* monster, bool isChangeDir);
	virtual void SetBlockPos(cocos2d::Vec2 pos);
	virtual void AttackedFromBelow(bool m_isBigMairo = false);

	float GetTopPosY();

	virtual void LateUpdateBlock(float deltaTime);
	virtual void ReleaseObj();

	BlockBase();
	~BlockBase();
};