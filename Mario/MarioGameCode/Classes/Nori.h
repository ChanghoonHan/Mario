#pragma once
#include "Monster.h"

class Nori : public Monster
{
	enum NORI_TEXTURE
	{
		IDLE1,
		IDLE2,
		DIE,
		NONE_DIE,
	};

	bool m_isDieMoving;

	float m_resurrectionTime;
	float m_semiResurrectionTime;
	float m_resurrectionDeltaTime;

	float m_dieTextureChangeTime;
	float m_dieTextureChangeDeltaTime;

	float m_gracePeriodTime;
	float m_gracePeriodDeltaTime;

	NORI_TEXTURE m_curNoriTextureIdx;

	virtual void ChangeState(MONSTER_STATE state, bool instantaneousDestroy = false);
	virtual void SetTexture();
public:
	bool init();
	void SetTextureAndMonsterSize();
	virtual bool IsCanTeamKill();

	virtual void SetYPositionBecauseColl(float deltaY);
	virtual void SetXPositionBecauseColl(float deltaX);

	virtual cocos2d::Rect GetCollider(bool UseCollWithMonster = false);
	virtual void UpdateMonster(float deltaTime);
	virtual bool CheckCollWithMario(cocos2d::Rect marioCollider, bool marioIsInvincible, bool marioIsDroping, bool* attackedUpside, Monster* neighborMonster);

	CREATE_FUNC(Nori);

	Nori();
	~Nori();
};