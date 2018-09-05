#pragma once
#include "BlockBase.h"

class GroundBlock : public BlockBase
{
	enum GROUND_BLOCK_TYPE;
	GROUND_BLOCK_TYPE m_groundType;
	int m_floorCount;

public:
	enum GROUND_BLOCK_TYPE
	{
		NULL_GROUND_BLOCK,
		GROUND,
		FLOOR,
	};

	virtual bool init();
	void SetBlock(GROUND_BLOCK_TYPE blockType, int sortOrder, int floorCount = 0);
	virtual void SetBlockPos(cocos2d::Vec2 pos);

	CREATE_FUNC(GroundBlock);

	GroundBlock();
	~GroundBlock();
};