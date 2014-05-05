#ifndef _BD_OBJECT_
#define _BD_OBJECT_

#include "cocos2d.h"
#include "jsapi.h"
#include "jsfriendapi.h"
#include "Components/BDMovementComp.h"

class BDAction;
class BDGameLayer;
class BDParticleSystem;

enum 
{
	SCENERY_LAYER,
	GAME_LAYER
};

enum OBJECT_TYPE
{
	OBJECT_BASEOBJ = -1,
	OBJECT_CHARACTER,
	OBJECT_ITEM
};

enum SKILL_TYPE
{
	BDSKILL_BASE,
	BDSKILL_JUMP
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
	BDObject(BDGameLayer* pGameLayer);

	JSObject* GetJSObject();
	void SetJSObject(JSObject* jsObj);

	virtual void SetCurBDAction(BDAction* pAction);
	virtual BDAction* GetCurBDAction();
	BDGameLayer* GetGameLayer();
	virtual int GetType();

	cocos2d::CCPoint GetSpeed();
	void SetSpeed(const cocos2d::CCPoint& ptSpeed);

	void SetLogicPosition(const cocos2d::CCPoint &position);
	void SetLogicPosition(float x, float y);
	void SetLogicPositionX(float x);
	void SetLogicPositionY(float y);

	cocos2d::CCPoint& GetLogicPosition();
	float GetLogicPositionX();
	float GetLogicPositionY();

	BDMovementComp* GetMovementComp();
	void SetMovementComp(BDMovementComp* pMovementComp);

	BDParticleSystem* GetEmitter();
	void SetEmitter(BDParticleSystem* pEmitter);

	virtual void PlayParticle(const char *plistFile,int iPositionType);

	static int GROUP_CATEGORY_BABE;
	static int GROUP_CATEGORY_ENEMY;
	static int GROUP_CATEGORY_ITEM;

	static int GROUP_MASK_BABE;
	static int GROUP_MASK_ENEMY;
	static int GROUP_MASK_ITEM;

protected:
	JSObject* m_lpJsObj;//the js side object
	BDAction* m_lpCurAction;
	cocos2d::CCPoint m_ptLogicPos;
	BDMovementComp* m_lpMovementComp;
	BDGameLayer* m_lpGameLayer;//pointer to the game layer this object belongs to 
	BDParticleSystem* m_lpEmitter;
	int m_iType;
};
#endif