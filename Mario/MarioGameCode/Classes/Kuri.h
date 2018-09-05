#pragma once
#include "Monster.h"

class Kuri : public Monster
{
	enum KURI_TEXTURE
	{
		IDLE1,
		IDLE2,
		DIE,
	};

	float m_destroyTime;
	float m_destroyDeltaTime;

	KURI_TEXTURE m_curKuriTextureIdx;
	virtual void ChangeState(MONSTER_STATE state, bool instantaneousDestroy = false);
	virtual void SetTexture();

public:

	bool init();
	void SetTextureAndMonsterSize();
	virtual void UpdateMonster(float deltaTime);

	CREATE_FUNC(Kuri);

	Kuri();
	~Kuri();
};

