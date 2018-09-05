#ifndef __MARIOMAPEDITORMAIN_SCENE_H__
#define __MARIOMAPEDITORMAIN_SCENE_H__

#include "cocos2d.h"

class MapAnchor;
class Block;

enum BLOCK_TYPE;
enum ITEM_TYPE;
enum MONSTER_TYPE;

USING_NS_CC;

class MarioMapEditorMainScene : public Scene
{
	MapAnchor* m_mapAnchor;
	Size m_visibleSize;
	Block* m_followMouseBlock;

	float m_curMapAnchorMoveSpeedPerSecond;
	float m_mapAnchorMoveSpeedPerSecond;

	bool m_leftMouseButtonDown;
	bool m_rightMouseButtonDown;

	virtual void update(float deltaTime);
	void ChangeFollowMouseBlock(cocos2d::Ref* pSender, BLOCK_TYPE blockType);
	void ChangeFollowMouseBlockToItem(cocos2d::Ref* pSender, ITEM_TYPE itemType);
	void ChangeFollowMouseBlockToMonster(cocos2d::Ref* pSender, MONSTER_TYPE itemType);
	void SaveMap(cocos2d::Ref* pSender);

	void onMouseDown(Event* event);
	void onMouseUp(Event* event);
	void onMouseMove(Event* event);
	void onKeyPressed(EventKeyboard::KeyCode keycode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	
	void SetMenuButton(MenuItem* menuItemImage, Vec2 pos);

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MarioMapEditorMainScene);
};

#endif // __MARIOMAPEDITORMAIN_SCENE_H__
