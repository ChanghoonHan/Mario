#pragma once
#include "BlockBase.h"

class StackBlock : public BlockBase
{
	enum STACK_TYPE;
	STACK_TYPE m_stackType;
	int m_pillarCount;

public:
	enum STACK_TYPE
	{
		NULL_STACK,
		STACK_LEFT,
		STACK_TOP,
	};

	virtual bool init();
	void SetBlock(STACK_TYPE stackType, int sortOrder, int pillarCount = 0);
	virtual void SetBlockPos(cocos2d::Vec2 pos);

	CREATE_FUNC(StackBlock);

	StackBlock();
	~StackBlock();
};