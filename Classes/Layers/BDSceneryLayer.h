#ifndef _BD_SCENERY_LAYER_
#define _BD_SCENERY_LAYER_

#include "cocos2d.h"
#include "BDBaseClasses/BDObject.h"
#include "Defs/DBDef.h"

//def class
//----------------------------------------------------------------------------------------------
class BDSceneryLayerDef:public BDDef
{
public:
	BDSceneryLayerDef();
	cocos2d::CCPoint m_ptSpeed;
	std::string m_strSpriteRes1;
	std::string m_strSpriteRes2;

	void ApplyFromDefObj(JSContext* cx,JSObject* defObj);
};

//native class
//------------------------------------------------------------------------------------------------
class BDSceneryLayer:public CCLayer,public BDObject
{
public:
	BDSceneryLayer();
	static BDSceneryLayer* create();
	virtual void update(float delta);

	cocos2d::CCPoint GetSpeed();
	cocos2d::CCSprite* GetSprite1();
	cocos2d::CCSprite* GetSprite2();
	
protected:
	cocos2d::CCPoint m_ptSpeed;
	cocos2d::CCSprite* m_sprSprite1;
	cocos2d::CCSprite* m_sprSprite2;
};
#endif