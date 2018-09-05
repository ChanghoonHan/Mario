#include "EnvironmentBlock.h"

USING_NS_CC;

bool EnvironmentBlock::init()
{
	BlockBase::init();
	m_scale = 5.0f;
	m_envType = ENVIRONMENT_BLOCK_TYPE::NULL_ENVIRONMENT_BLOCK;

	return true;
}

bool EnvironmentBlock::CheckColl(Rect collider, Rect* collisionRect, bool* getDemage)
{
	return false;
}

void EnvironmentBlock::SetBlock(ENVIRONMENT_BLOCK_TYPE blockType, int sortOrder)
{
	m_sortOrder = sortOrder;
	Sprite* tempSprite = NULL;

	switch (blockType)
	{
	case EnvironmentBlock::NULL_ENVIRONMENT_BLOCK:
		break;
	case EnvironmentBlock::CASTLE:
		tempSprite = Sprite::create("./Environment/Castle.png");
		break;
	case EnvironmentBlock::CLOUD_LEFT:
		tempSprite = Sprite::create("./Environment/CloudLeft.png");
		break;
	case EnvironmentBlock::CLOUD_RIGHT:
		tempSprite = Sprite::create("./Environment/CloudRight.png");
		break;
	case EnvironmentBlock::CLOUD_CENTER:
		tempSprite = Sprite::create("./Environment/CloudCenter.png");
		break;
	case EnvironmentBlock::CLOUD_TOP:
		tempSprite = Sprite::create("./Environment/CloudTop.png");
		break;
	case EnvironmentBlock::GRASS_LEFT:
		tempSprite = Sprite::create("./Environment/GrassLeft.png");
		break;
	case EnvironmentBlock::GRASS_RIGHT:
		tempSprite = Sprite::create("./Environment/GrassRight.png");
		break;
	case EnvironmentBlock::GRASS_CENTER:
		tempSprite = Sprite::create("./Environment/GrassCenter.png");
		break;
	case EnvironmentBlock::MOUNTAIN_LEFT:
		tempSprite = Sprite::create("./Environment/MountainLeft.png");
		break;
	case EnvironmentBlock::MOUNTAIN_RIGHT:
		tempSprite = Sprite::create("./Environment/MountainRight.png");
		break;
	case EnvironmentBlock::MOUNTAIN_CENTER1:
		tempSprite = Sprite::create("./Environment/MountainCenter1.png");
		break;
	case EnvironmentBlock::MOUNTAIN_CENTER2:
		tempSprite = Sprite::create("./Environment/MountainCenter2.png");
		break;
	case EnvironmentBlock::MOUNTAIN_CENTER3:
		tempSprite = Sprite::create("./Environment/MountainCenter3.png");
		break;
	case EnvironmentBlock::MOUNTAIN_TOP:
		tempSprite = Sprite::create("./Environment/MountainTop.png");
		break;
	default:
		break;
	}

	m_envType = blockType;
	m_blockSprites.push_back(tempSprite);
	tempSprite->setAnchorPoint(Vec2::ZERO);
	tempSprite->setPosition(Vec2::ZERO);
	tempSprite->setScale(m_scale);
	this->addChild(tempSprite, m_sortOrder);
}

EnvironmentBlock::EnvironmentBlock()
{
}

EnvironmentBlock::~EnvironmentBlock()
{
}
