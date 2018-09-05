#pragma once
#include <cocos2d.h>

class Monster : public cocos2d::Node
{
protected:

	enum MONSTER_STATE
	{
		STAY,
		IDLE,
		DESTROY,
		DESTROYED,
	};

	MONSTER_STATE m_curState;

	cocos2d::LabelTTF* m_scoreLabel;
	cocos2d::Sprite* m_monsterSprite;
	cocos2d::Rect m_collider;
	cocos2d::Size m_monsterSize;
	cocos2d::Texture2D* m_curTexture;
	std::vector<cocos2d::Texture2D*> m_textures;

	int m_score;

	float m_idleSpriteChangeDeltaTime;
	float m_idleSpriteChangeTime;

	int m_curMoveDir;
	float m_spawnDist;
	float m_xVelocity;
	float m_minYVelocity;
	float m_yVelocity;
	float m_gravity;

	bool m_instantaneousDestroy;

	void SetCollider();
	void SetScoreLabel();
	void EndLabelActionCallback(Node* obj);
	void StartScoreLabelAction();

	virtual void SetTexture();
	virtual void ChangeState(MONSTER_STATE state, bool instantaneousDestroy = false);
	virtual void SetTextureAndMonsterSize() = 0;

	void ChangeStateCallback(cocos2d::Node* obj, MONSTER_STATE state);

	cocos2d::Rect CalCollRect(cocos2d::Rect rect1, cocos2d::Rect rect2);

	cocos2d::CallFuncN* m_changeStateToDestroyedCallback;

	cocos2d::MoveBy* m_labelMoveByAction;
	cocos2d::CallFuncN* m_endScoreLabelActionCallback;

public:

	virtual bool init();

	virtual void UpdateMonster(float deltaTime);

	virtual cocos2d::Rect GetCollider(bool UseCollWithMonster = false);

	virtual void SetYPositionBecauseColl(float deltaY);
	virtual void SetXPositionBecauseColl(float deltaX);

	void SetInstantaneousDestroy(bool changeDir);
	void Spawn();

	void ReleaseObj();
	void AttackedFromBullet(int attackedDir);
	virtual bool IsCanTeamKill();

	virtual bool CheckCollWithMario(cocos2d::Rect marioCollider, bool marioIsInvincible, bool marioIsDroping, bool* attackedUpside, Monster* neighborMonster);
	

	Monster();
	~Monster();
};