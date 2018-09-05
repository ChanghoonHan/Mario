#ifndef __GAMEMMIDDLE_SCENE_H__
#define __GAMEMMIDDLE_SCENE_H__

#include <cocos2d.h>

class ScoreBoard;

class GameMiddleScene : public cocos2d::Scene
{
	float m_changeSceneTime;
	float m_changeSceneDeltaTime;

	bool m_isGameClear;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameMiddleScene);

	virtual void update(float deltaTime);

	void SetScoreBoard();
	void SetGameClear();

	GameMiddleScene();
	~GameMiddleScene();
};

#endif // __GAMEMMIDDLE_SCENE_H__