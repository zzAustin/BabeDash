#include "cocos2d.h"
#include "cocos2d_specifics.hpp"
#include "Scenes/BDGameScene.h"
#include "Layers/BDGameLayer.h"
USING_NS_CC;

JSClass  *jsb_BDGameScene_class;
JSObject *jsb_BDGameScene_prototype;


//native class
BDGameScene *BDGameScene::create()
{
	BDGameScene* pRet = new BDGameScene();

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

BDObject* BDGameScene::AddObject(JSContext* cx,BDGameLayerDef& def)
{
	if(def.obj_type.length() == 0)
	{
		CCLog("obj_type length is 0!");
		return NULL;
	}

	if(def.obj_type.find("GameLayer")>=0)
	{
		BDGameLayer* pLayer = BDGameLayer::create();
		JSObject* jsObj = JS_NewObject(cx,jsb_BDGameLayer_class,jsb_BDGameLayer_prototype,jsb_CCLayer_prototype);
		CCAssert(jsObj!=NULL,"---BDGameScene::AddObject jsObj is NULL");
		pLayer->SetJSObject(jsObj);
		return pLayer;
	}

	return NULL;
}


void BDGameScene::SayHello()
{
	CCLog("-----BDGameScene:Time to say hello...");
}


//---------------------------------------------------------------------------------------------------
//JS binding
JSBool js_bdgamescene_create(JSContext *cx, uint32_t argc, jsval *vp)
{
	if (argc == 0) {
		BDGameScene* ret = BDGameScene::create();
		jsval jsret;
		do {
			if (ret) {
				js_proxy_t *proxy = js_get_or_create_proxy<BDGameScene>(cx, ret);
				jsret = OBJECT_TO_JSVAL(proxy->obj);
			} else {
				jsret = JSVAL_NULL;
			}
		} while (0);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments");
	return JS_FALSE;
}

JSBool js_bdgamescene_sayHello(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;

	JSObject *obj = NULL;
	BDGameScene* cobj = NULL;
	obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	cobj = (BDGameScene*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	

	/*do {
	if (argc == 1) { 
	cocos2d::CCNode* arg0;
	do {
	if (!argv[0].isObject()) { ok = JS_FALSE; break; }
	js_proxy_t *proxy;
	JSObject *tmpObj = JSVAL_TO_OBJECT(argv[0]);
	proxy = jsb_get_js_proxy(tmpObj);
	arg0 = (cocos2d::CCNode*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( arg0, cx, JS_FALSE, "Invalid Native Object");
	} while (0);
	if (!ok) { ok = JS_TRUE; break; }
	cobj->addChild(arg0);
	JS_SET_RVAL(cx, vp, JSVAL_VOID);
	return JS_TRUE;
	}
	} while(0);*/

	if(argc == 0)
	{
		cobj->SayHello();
		return JS_TRUE;
	}
	

	JS_ReportError(cx, "wrong number of arguments");
	return JS_FALSE;
}

JSBool js_bdgamescene_addObject(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;

	JSObject *obj = NULL;
	BDGameScene* cobj = NULL;
	obj = JS_THIS_OBJECT(cx, vp); 
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	cobj = (BDGameScene*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
	

	/*do {
	if (argc == 1) {
	cocos2d::CCNode* arg0;
	do {
	if (!argv[0].isObject()) { ok = JS_FALSE; break; }
	js_proxy_t *proxy;
	JSObject *tmpObj = JSVAL_TO_OBJECT(argv[0]);
	proxy = jsb_get_js_proxy(tmpObj);
	arg0 = (cocos2d::CCNode*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( arg0, cx, JS_FALSE, "Invalid Native Object");
	} while (0);
	if (!ok) { ok = JS_TRUE; break; }
	cobj->addChild(arg0);
	JS_SET_RVAL(cx, vp, JSVAL_VOID);
	return JS_TRUE;
	}
	} while(0);*/


	if(argc == 1) 
	{
		
		BDGameLayerDef def;
		JSObject* defObj = JSVAL_TO_OBJECT(argv[0]);
		BDObject* newObj = NULL;
		JSObject* jsObj = NULL;
		
		def.ApplyFromDefObj(cx,defObj); 
		newObj = cobj->AddObject(cx,def);
		CCAssert(newObj!=NULL,"---js_bdgamescene_addObject newObj is NULL!");
		jsObj = newObj->GetJSObject();
		*vp = (jsObj != NULL)?OBJECT_TO_JSVAL(jsObj):JSVAL_NULL;

		return JS_TRUE;
	}
	

	JS_ReportError(cx, "wrong number of arguments");
	return JS_FALSE;
}


//JS_DefineFunction will register this
JSBool js_bdgamescene_retain(JSContext* cx,uint32_t argc,jsval *vp)
{
	CCLog("in js_bdgamescene_retain...");
	JSObject* thisObj = JS_THIS_OBJECT(cx,vp);

	if(thisObj){
		js_proxy_t* proxy = jsb_get_js_proxy(thisObj);

		if(proxy){
			((BDGameScene*)proxy->ptr)->SayHello();
			CCLog("BDGameScene Retain succeed!");
			return JS_TRUE;
		}
	}

	JS_ReportError(cx,"Invalid native object");
	return JS_FALSE;
}

JSBool js_bdgamescene_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	if (argc == 0) {
		BDGameScene* cobj = new BDGameScene();
		cocos2d::CCObject *_ccobj = dynamic_cast<cocos2d::CCObject *>(cobj);
		if (_ccobj) {
			_ccobj->autorelease();
		}
		TypeTest<BDGameScene> t;
		js_type_class_t *typeClass;
		uint32_t typeId = t.s_id();
		HASH_FIND_INT(_js_global_type_ht, &typeId, typeClass);
		assert(typeClass);
		JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
		// link the native object with the javascript object
		js_proxy_t* p = jsb_new_proxy(cobj, obj);
		JS_AddNamedObjectRoot(cx, &p->obj, "BDGameScene");
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	return JS_FALSE;
}

void js_bdgamescene_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOGINFO("jsbindings: finalizing JS object %p (BDGameScene)", obj);
}



void js_register_bdgamescene(JSContext *cx, JSObject *global) {
	jsb_BDGameScene_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_BDGameScene_class->name = "BDGameScene";
	jsb_BDGameScene_class->addProperty = JS_PropertyStub;
	jsb_BDGameScene_class->delProperty = JS_PropertyStub;
	jsb_BDGameScene_class->getProperty = JS_PropertyStub;
	jsb_BDGameScene_class->setProperty = JS_StrictPropertyStub;
	jsb_BDGameScene_class->enumerate = JS_EnumerateStub;
	jsb_BDGameScene_class->resolve = JS_ResolveStub;
	jsb_BDGameScene_class->convert = JS_ConvertStub;
	jsb_BDGameScene_class->finalize = js_bdgamescene_finalize;
	jsb_BDGameScene_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		{0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
	};

	static JSFunctionSpec funcs[] = {
		JS_FN("sayHello", js_bdgamescene_sayHello, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("addObject", js_bdgamescene_addObject, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
	JS_FN("create", js_bdgamescene_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_FS_END
	};

	jsb_BDGameScene_prototype = JS_InitClass(
		cx, global,
		jsb_CCScene_prototype, // parent proto
		jsb_BDGameScene_class,
		js_bdgamescene_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs/*st_funcs*/);

	// make the class enumerable in the registered namespace
	JSBool found;
	JS_SetPropertyAttributes(cx, global, "BDGameScene", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

	// add the proto and JSClass to the type->js info hash table
	TypeTest<BDGameScene> t;
	js_type_class_t *p;
	uint32_t typeId = t.s_id();
	HASH_FIND_INT(_js_global_type_ht, &typeId, p);
	if (!p) {
		p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
		p->type = typeId;
		p->jsclass = jsb_BDGameScene_class;
		p->proto = jsb_BDGameScene_prototype;
		p->parentProto = jsb_CCScene_prototype;
		HASH_ADD_INT(_js_global_type_ht, type, p);
	}
}

void register_all_bdgamescene(JSContext* cx, JSObject* obj) {
	// first, try to get the ns
	jsval nsval;
	JSObject *ns;
	JS_GetProperty(cx, obj, "BD", &nsval);
	if (nsval == JSVAL_VOID) {
		ns = JS_NewObject(cx, NULL, NULL, NULL);
		nsval = OBJECT_TO_JSVAL(ns);
		JS_SetProperty(cx, obj, "BD", &nsval);
	} else {
		JS_ValueToObject(cx, nsval, &ns);
	}
	obj = ns;

	js_register_bdgamescene(cx, obj);

	JS_DefineFunction(cx, jsb_BDGameScene_prototype, "retain", js_bdgamescene_retain, 0, JSPROP_READONLY | JSPROP_PERMANENT);
}