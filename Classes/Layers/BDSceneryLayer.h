#ifndef _BD_SCENERY_LAYER_
#define _BD_SCENERY_LAYER_

#include "cocos2d.h"
#include "BDBaseClasses/BDObject.h"
#include "Defs/DBDef.h"


class BDGameScene;

extern JSObject *jsb_CCLayer_prototype;//parent class
extern JSClass  *jsb_BDSceneryLayer_class;
extern JSObject *jsb_BDSceneryLayer_prototype;

//def class
//----------------------------------------------------------------------------------------------
class BDSceneryLayerDef:public BDDef
{
public:
	BDSceneryLayerDef();
	cocos2d::CCPoint m_ptPos;
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
	BDSceneryLayer(BDGameScene* pScene);
	~BDSceneryLayer();
	static BDSceneryLayer* CreateWithScene(BDGameScene* pScene);//notice this create function is native only,it's the following non-param func that is JS binded
	static BDSceneryLayer* Create();
	void update(float delta);

	cocos2d::CCSprite* GetSprite1();
	cocos2d::CCSprite* GetSprite2();
	int GetType();
	bool InitWithAttributes(BDSceneryLayerDef& def);
	
protected:
	cocos2d::CCSprite* m_lpSprite1;
	cocos2d::CCSprite* m_lpSprite2;
	cocos2d::CCPoint m_ptSpeed;
	BDGameScene* m_lpGameScene;//pointer to the game scene of current level           
};

//---------------------------------------------------------------------------------------------
//JS Binding
JSBool js_bdscenerylayer_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_bdscenerylayer_finalize(JSContext *cx, JSObject *obj);
void js_register_bdscenerylayer(JSContext *cx, JSObject *global);
void register_all_bdscenerylayer(JSContext* cx, JSObject* obj);
JSBool js_bdscenerylayer_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdscenerylayer_initwithattributes(JSContext *cx, uint32_t argc, jsval *vp);

#endif