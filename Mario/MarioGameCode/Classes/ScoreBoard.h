#pragma once
#include <cocos2d.h>

class ScoreBoard : public cocos2d::Node
{
	enum LABEL_TYPE
	{
		MARIO,
		SCORE,
		COIN,
		WORLD,
		WORLDNUM,
		TIME,
		TIMENUM,
		ENDOFENUM,
	};

	static ScoreBoard* S;

	cocos2d::Sprite* m_coinUISprite;
	cocos2d::Sprite* m_xUISprite;
	int m_life;
	int m_score;
	int m_coin;
	int m_worldLevel;
	int m_dungeonLevel;
	float m_time;

	float m_changeTimeToScoreTime;
	float m_changeTimeToScoreDeltaTime;

	int m_scorePerSec;

	int m_fontSize;

	bool m_isTouchSavePoint;

	std::string m_fontPath;

	cocos2d::Size m_visibleSize;
	cocos2d::LabelTTF* m_scoreBoardLabels[LABEL_TYPE::ENDOFENUM];

	CREATE_FUNC(ScoreBoard);

	ScoreBoard();
	~ScoreBoard();

public:

	virtual bool init();
	void IncreaseScore(int score);
	void IncreaseCoinCount();

	void UpdateScoreBoard(float deltaTime);

	float GetTime();
	void SetTimeBlank();
	void InitTime();
	void TouchSavePoint();
	bool IsTouchSavePoint();

	int GetLife();
	void DecreaseLife();

	void ChangeTimeToScore(float deltaTime);

	static ScoreBoard* GetInstance();
	static void Destroy();
};

