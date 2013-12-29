#ifndef _BD_GAME_SCENE_
#define _BD_GAME_SCENE_

#include "BDBaseClasses/BDObject.h"
#include "Layers/BDGameLayer.h"

extern JSObject *jsb_CCScene_prototype;//parent class
extern JSClass  *jsb_BDGameScene_class;
extern JSObject *jsb_BDGameScene_prototype;


//def class
class BDSceneUnitDef:public BDDef
{
public:
	BDSceneUnitDef();
	std::string obj_type;
	CCPoint pos;
	CCPoint speed;
	void ApplyFromDefObj(JSContext* cx,JSObject* defObj);

	
};


//native class
class BDGameScene:public cocos2d::CCScene,public BDObject
{
public:
	static BDGameScene* create();
	BDObject* AddSceneUnit(JSContext* cx,BDSceneUnitDef& def);
	void SayHello();
};


//-----------------------------------------------------------------------------------
//JS Binding
JSBool js_bdgamescene_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_bdgamescene_finalize(JSContext *cx, JSObject *obj);
void js_register_bdgamescene(JSContext *cx, JSObject *global);
void register_all_bdgamescene(JSContext* cx, JSObject* obj);
JSBool js_bdgamescene_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdgamescene_sayHello(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdgamescene_retain(JSContext* cx,uint32_t argc,jsval *vp);

#endif