#pragma once
#include "BlockBase.h"

class Mario;

class Flag : public BlockBase
{
	enum FLAG_STATE
	{
		IDLE,
		DONWING,
		FINISH
	};

	int m_score;
	int m_flagHeight;

	Mario* m_mario;

	FLAG_STATE m_curFlagState;

	cocos2d::Rect m_flagCollider;
	cocos2d::Rect m_flagTopCollider;

	cocos2d::Sprite* m_flagSprite;
	cocos2d::LabelTTF* m_scoreLabel;


	void DownFlag();
	void EndDownFlagCallback(Node* obj);

public:

	virtual bool init();

	virtual void SetBlockPos(cocos2d::Vec2 pos);

	bool CheckCollFlagWithMario(Mario* mario);

	CREATE_FUNC(Flag);

	Flag();
	~Flag();
};

