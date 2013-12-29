#ifndef _BD_GAME_LAYER_
#define _BD_GAME_LAYER_

#include "BDBaseClasses/BDObject.h"
#include "Defs/DBDef.h"

extern JSObject *jsb_CCLayer_prototype;//parent class
extern JSClass  *jsb_BDGameLayer_class;
extern JSObject *jsb_BDGameLayer_prototype;

//def class
class BDGameLayerDef:public BDDef
{
public:
	std::string obj_type;
	CCPoint pos;
	CCPoint speed;
	void ApplyFromDefObj(JSContext* cx,JSObject* defObj);

	BDGameLayerDef::BDGameLayerDef();
};

//native class
class BDGameLayer:public cocos2d::CCLayer,public BDObject
{
public:
	static BDGameLayer *BDGameLayer::create();
};


//---------------------------------------------------------------------------------------------
//JS Binding
JSBool js_bdgamelayer_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_bdgamelayer_finalize(JSContext *cx, JSObject *obj);
void js_register_bdgamelayer(JSContext *cx, JSObject *global);
void register_all_bdgamelayer(JSContext* cx, JSObject* obj);
JSBool js_bdgamelayer_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdgamelayer_sayHello(JSContext *cx, uint32_t argc, jsval *vp);
#endif