#pragma once
#include <cocos2d.h>

class InteractionBlock;

class Item : public cocos2d::Node
{
protected:
	enum ITEM_STATE;
	enum ITEM_TYPE;

	ITEM_TYPE m_itemType;

	cocos2d::LabelTTF* m_scoreLabel;
	std::string m_labelPath;
	int m_labelSize;
	cocos2d::Size m_itemSize;

	float m_xVelocity;
	float m_yVelocity;
	float m_gravity;
	float m_minYVelocity;
	float m_jumpVelocity;

	int m_score;
	ITEM_STATE m_curItemState;
	float m_scale;
	int m_sortOrder;
	cocos2d::Size m_colliderSize;
	cocos2d::Rect m_collider;
	cocos2d::Sprite* m_itemSprite;

	virtual void DestroyCallback(cocos2d::Node* obj);
	virtual void ChangeState(ITEM_STATE itemState);
	void ChangeStateCallback(Node* obj, ITEM_STATE itemState);

public:
	enum ITEM_STATE
	{
		STAY,
		IDLE,
		DESTROY,
		DESTROYED,
	};

	enum ITEM_TYPE
	{
		NULL_TYPE,
		COIN,
		STAR,
		MUSHROOM,
		FLOWER,
	};

	virtual void SetYPositionBecauseColl(float deltaY);
	void SetXPositionBecauseColl(float deltaX);
	void SetJump();

	virtual void Spawn(InteractionBlock* interBlock = NULL);
	virtual bool init();
	virtual void UpdateItem(float deltaTime);
	virtual void ChangeMoveDir();
	virtual void ChangeItemToFlower();
	virtual cocos2d::Rect GetCollider();

	bool CheckCollWithMario(cocos2d::Rect marioCollider, ITEM_TYPE* itemType);

	ITEM_TYPE GetItemType();

	virtual bool CheckColl(cocos2d::Rect collider);

	Item();
	~Item();
};