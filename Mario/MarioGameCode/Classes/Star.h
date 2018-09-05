#pragma once
#include "Item.h"

class Star : public Item
{

public:
	bool init();

	virtual void Spawn(InteractionBlock* interBlock = NULL);
	virtual void UpdateItem(float deltaTime);
	virtual cocos2d::Rect GetCollider();
	virtual void SetYPositionBecauseColl(float deltaY);

	CREATE_FUNC(Star);

	Star();
	~Star();
};
