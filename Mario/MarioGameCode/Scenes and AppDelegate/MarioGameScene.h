#ifndef __MARIOGAME_SCENE_H__
#define __MARIOGAME_SCENE_H__

#include <cocos2d.h>

class MapAnchor;
class ScoreBoard;

class MarioGameScene : public cocos2d::Scene
{
	MapAnchor* m_mapAnchor;
	ScoreBoard* m_scoreBoard;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void SetScoreBoard();
	void LoadSavePoint();

	CREATE_FUNC(MarioGameScene);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	virtual void update(float deltaTime);

	MarioGameScene();
	~MarioGameScene();
};

#endif // __MARIOGAME_SCENE_H__