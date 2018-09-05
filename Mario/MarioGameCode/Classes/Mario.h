#pragma once
#include <cocos2d.h>

class MapAnchor;

class Mario : public cocos2d::Node
{	
#define INVIN_COUNT 4
	enum MARIO_STATE;

	enum MARIO_SIZE
	{
		SMALL_MARIO,
		BIG_MARIO,
		FIRE_MARIO,
		MARIO_SIZE_END,
	};

	enum MARIO_TEXTURE
	{
		IDLE_TEXTURE,
		WALK1_TEXTURE,
		WALK2_TEXTURE,
		WALK3_TEXTURE,
		BRAKE_TEXTURE,
		JUMP_TEXTURE,
		DIE_TEXTURE,
		FLAG1_TEXTURE,
		FLAG2_TEXTURE,
		GROW_TEXTURE,//only Big Mario Begin
		SIT_TEXTURE,
		FIRE_TEXTURE,
		MARIO_TEXTURE_END,
	};

	struct ChangeAnimationTextureInfo
	{
		MARIO_SIZE marioSize;
		MARIO_TEXTURE marioTexture;
	};

	MapAnchor* m_mapAnchor;

	std::vector<ChangeAnimationTextureInfo> m_changeAnimationTextureInfo[2];
	std::vector<cocos2d::Texture2D*> m_marioTextures[MARIO_SIZE_END][MARIO_TEXTURE_END];
	void LoadTexture();
	void LoadAnimationTextureInfo();

	cocos2d::LabelTTF* m_InfoLable;

	MARIO_SIZE m_marioSizeForTexture;
	MARIO_TEXTURE m_marioStateForTexture;
	int m_invincibleCount;

	MARIO_STATE m_curState;
	MARIO_STATE m_prevState;

	float m_growOrDiminishAnimationChangeTextureTime;
	float m_growOrDiminishAnimationChangeTextureDeltaTime;
	int m_growOrDiminishAnimationTextureInfoIdx;

	int m_changeToFireInvinCycleCount;
	float m_changeToFireInvinChangeDeltaTime;
	float m_changeToFireInvinchangeTime;

	float m_curSpeed;
	float m_timeToMaxSpeed;
	float m_timeToZeroSpeed;
	float m_timeToZeroSpeedForChangeDir;
	float m_maxSpeed;

	float m_maxSpeedOrigin;
	float m_maxSpeedDash;

	float m_changeWalkTextureDeltaTime;
	float m_changeWalkTextureTimeMax;
	float m_changeWalkTextureTimeMaxOrigin;
	float m_changeWalkTextureTimeMaxDash;
	float m_changeWalkTextureTimeMin;
	float m_changeWalkTextureTimeMinOrigin;
	float m_changeWalkTextureTimeMinDash;

	float m_changeFlagTextureDeltaTime;
	float m_changeFlagTextureTime;
	float m_changeGoToCastleDeltaTime;
	float m_changeGoToCastleTime;

	float m_invincibleTextureChangeDeltaTime;
	float m_invincibleTextureChangeTime;
	float m_invincibleTextureChangeTimeFast;
	float m_invincibleTextureChangeTimeSlow;
	float m_invincibleSustainmentDeltaTime;
	float m_invincibleSustainmentTime;

	float m_yVelocity;
	float m_yMinVelocity;
	float m_jumpVelocity;
	float m_gravity;

	float m_castleDoorXPos;

	bool m_releaseTurboKey;
	bool m_isSit;
	bool m_isHaveObstacle;
	bool m_canGoToCastle;
	bool m_isBlinking;
	bool m_isInvincible;
	bool m_isChangeWhenJumping;
	bool m_isJumping;
	float m_jumpKeyInputDeltaTime;
	float m_jumpKeyInputMaxTime;
	bool m_isPressedJumpKey;
	float m_flagDownSpeed;

	cocos2d::Vec2 m_prevPos;

	int m_curMoveDir;
	int m_prevMoveDir;
	int m_breakDir;

	MARIO_SIZE m_marioSize;

	float m_prevXPos;
	cocos2d::Sprite* m_marioSprite;

	float m_reloadTime;
	float m_reloadDeltaTime;

	cocos2d::CallFuncN* m_changePrevStateCallback;

	void ChangeMarioState(MARIO_STATE state);
	void ChangeMarioStateCallback(Node* obj, MARIO_STATE state);
	void CheckMoveState();
	void SetInfoString();
	void SetTexture();
	void SetJump();
	void ChangeWalkTexture(float deltaTime);
	void ChangeToPrevState();
	void InvincibleUpdate(float deltaTime, bool isChanging = false);
	void EndBlinkCallback(Node* obj);
	void FireFireBall();

public:
	enum MARIO_STATE
	{
		IDLE,
		WALK,
		BRAKE,
		BRAKE_FOR_CHANG_DIR,
		CHANGING_TO_BIG,
		CHANGING_TO_FIRE,
		CHANGING_TO_SMALL,
		DIE,
		DIED,
		FLAG_DOWNING,
		GO_END,
		CLEAR,
	};

	bool init();

	CREATE_FUNC(Mario);

	void UpdateMario(float deltaTime, float* marioMoveXDist);
	void LateUpdateMario();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
	
	void SetYPosBecauseColl(float y);
	void SetJumpBlocked();
	void SetJumpEnd();
	void SetJumpBecauseCollWithMonster();
	void AttackedFromMonster();
	void SetDie();
	bool IsJumping();
	bool IsDroping();
	bool IsInvincible();

	void SetFlagDowning();
	void SetGoToCastle(float castleDoorXPos);

	void EatItem(int itemType);
	
	bool IsChanging();
	bool IsBigMario();

	cocos2d::Rect GetCollider();

	void SetMapAnchore(MapAnchor* mapAnchor);
	void Destroy();

	MARIO_STATE GetMarioState();

	Mario();
	~Mario();
};

