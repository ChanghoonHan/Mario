#pragma once
#include "EnvironmentBlock.h"

class Castle : public EnvironmentBlock
{
	cocos2d::Sprite* m_flagSprite;
	cocos2d::Sprite* m_fireworkSprite;

	cocos2d::Animation* m_fireworkAnimation;
	cocos2d::Animate* m_fireworkAnimate;

	cocos2d::CallFuncN* m_setNoVisibleFireworkCallback;

	float m_fireworkReloadTime;
	float m_fireworkReloadDeltaTime;

	std::vector<cocos2d::Vec2> m_fireworkPositions;

	int m_fireworkPosIdx;

	bool m_isStartCong;
	bool m_isFinishCong;

	void SetFirework(cocos2d::Vec2 pos);
	void SetNoVisibleFireworkCallback(cocos2d::Node* obj);

public:
	virtual bool init();
	void SetBlock(ENVIRONMENT_BLOCK_TYPE blockType, int sortOrder);

	void SetCongratulation(float deltaTime);
	void ReleaseObj();

	bool IsFinishCong();

	CREATE_FUNC(Castle);

	Castle();
	~Castle();
};

