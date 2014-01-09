#ifndef _BD_OBJECT_
#define _BD_OBJECT_

#include "cocos2d.h"
#include "jsapi.h"
#include "jsfriendapi.h"

class BDAction;

enum 
{
	SCENERY_LAYER,
	GAME_LAYER
};


class BDObject
{
public:
	enum OBJECT_GROUP
	{
		GROUP_NULL = 0,
		GROUP_BABE,
		GROUP_ENEMY,
		GROUP_ITEM,
		GROUP_NUM
	};

	BDObject();
	JSObject* GetJSObject();
	void SetJSObject(JSObject* jsObj);
	virtual void SetCurBDAction(BDAction* pAction);
	virtual BDAction* GetCurBDAction();
	virtual int GetType();
	virtual cocos2d::CCPoint& GetSpeed();
	virtual void SetSpeed(cocos2d::CCPoint& ptSpeed);

	static int GROUP_CATEGORY_BABE;
	static int GROUP_CATEGORY_ENEMY;
	static int GROUP_CATEGORY_ITEM;

	static int GROUP_MASK_BABE;
	static int GROUP_MASK_ENEMY;
	static int GROUP_MASK_ITEM;

protected:
	JSObject* m_lpJsObj;//the js side object
	BDAction* m_lpCurAction;
    cocos2d::CCPoint m_ptSpeed;
};
#endif