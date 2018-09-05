#pragma once
#include "BlockBase.h"

class EnvironmentBlock : public BlockBase
{
	enum ENVIRONMENT_BLOCK_TYPE;
	ENVIRONMENT_BLOCK_TYPE m_envType;


public:
	enum ENVIRONMENT_BLOCK_TYPE
	{
		NULL_ENVIRONMENT_BLOCK,
		CASTLE,
		CLOUD_LEFT,
		CLOUD_RIGHT,
		CLOUD_CENTER,
		CLOUD_TOP,
		GRASS_LEFT,
		GRASS_RIGHT,
		GRASS_CENTER,
		MOUNTAIN_LEFT,
		MOUNTAIN_RIGHT,
		MOUNTAIN_CENTER1,
		MOUNTAIN_CENTER2,
		MOUNTAIN_CENTER3,
		MOUNTAIN_TOP,
	};

	virtual bool init();
	virtual bool CheckColl(cocos2d::Rect collider, cocos2d::Rect* collisionRect = NULL, bool* getDemage = NULL);
	void SetBlock(ENVIRONMENT_BLOCK_TYPE blockType, int sortOrder);

	CREATE_FUNC(EnvironmentBlock);

	EnvironmentBlock();
	~EnvironmentBlock();
};

