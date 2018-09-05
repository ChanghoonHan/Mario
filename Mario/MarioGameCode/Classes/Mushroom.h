#pragma once
#include "Item.h"

class Mushroom : public Item
{

public:
	bool init();

	virtual void Spawn(InteractionBlock* interBlock = NULL);
	virtual void UpdateItem(float deltaTime);
	virtual cocos2d::Rect GetCollider();
	virtual void ChangeItemToFlower();

	CREATE_FUNC(Mushroom);

	Mushroom();
	~Mushroom();
};

