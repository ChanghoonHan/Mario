#ifndef __MARIOTITTLE_SCENE_H__
#define __MARIOTITTLE_SCENE_H__

#include "cocos2d.h"

class MapAnchor;

class MarioTittleScene : public cocos2d::Scene
{
	cocos2d::Sprite* m_mainTittleSprite;
	MapAnchor* m_mapAnchor;

public:
    static cocos2d::Scene* createScene();
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MarioTittleScene);

	MarioTittleScene();
	~MarioTittleScene();
};

#endif // __MARIOTITTLE_SCENE_H__
