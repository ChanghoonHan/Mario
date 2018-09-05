#pragma once
#include "Item.h"
class InCoin : public Item
{	
	InteractionBlock* m_parentBlock;

	virtual void DestroyCallback(cocos2d::Node* obj);

public:
	bool init();
	virtual void Spawn(InteractionBlock* interBlock = NULL);

	CREATE_FUNC(InCoin);

	InCoin();
	~InCoin();
};

