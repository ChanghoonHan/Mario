#pragma once
#include <cocos2d.h>

class FireBall : public cocos2d::Node
{
	enum FIRE_BALL_STATE
	{
		STAY,
		FIRE,
		DESTROY,
	};

	cocos2d::Size m_fireBallSize;
	FIRE_BALL_STATE m_curState;

	cocos2d::Texture2D* m_fireBallTexture;

	cocos2d::Sprite* m_sprite;
	cocos2d::RotateBy* m_fireBallRotateByAction;
	cocos2d::DelayTime* m_DelayTimeAction;
	cocos2d::Sequence* m_fireBallRotateBySequenceAction;
	cocos2d::RepeatForever* m_fireBallRotateByRepeatForeverAction;

	cocos2d::Animation* m_explodeAnimation;
	cocos2d::Animate* m_explodeAnimate;
	cocos2d::CallFuncN* m_changeStateToStayCallback;
	cocos2d::Sequence* m_destroySequence;

	float m_xVelocity;
	float m_yVelocity;
	float m_jumpVelocity;
	float m_gravity;
	float m_yMinVelocity;

	cocos2d::Vec2 m_fireStartPos;
	int m_fireDir;

	void ChangeStateToStayCallback(Node* obj);
	void ChangeState(FIRE_BALL_STATE state);

public:
	
	void UpdateFireBall(float deltaTime, float m_mapAnchorXEnd);
	bool init();
	bool Fire(cocos2d::Vec2 startPos, int dir);
	void SetPositionYBecauseCallWithBlock(float blockTopYPos, float yDelta);
	void SetDestroy();
	cocos2d::Rect GetCollider();
	void DestroyRetainObj();

	CREATE_FUNC(FireBall);

	FireBall();
	~FireBall();
};

