#include "cocos2d.h"
#include "cocos2d_specifics.hpp"
#include "Scenes/BDGameScene.h"
#include "Layers/BDSceneryLayer.h"
#include "Layers/BDGameLayer.h"
#include "Util/CommonUtil.h"
USING_NS_CC;

JSClass  *jsb_BDGameScene_class = NULL;
JSObject *jsb_BDGameScene_prototype = NULL;

//def class
//-----------------------------------------------------------------------------------
BDSceneUnitDef::BDSceneUnitDef()
{
	this->obj_type = "null";
	this->pos.x = 0.0f;
	this->pos.y = 0.0f;
	this->speed.x = 0.0f;
	this->speed.y = 0.0f;
}

void BDSceneUnitDef::ApplyFromDefObj(JSContext* cx,JSObject* defObj)
{
	jsval vp = JSVAL_VOID;
	JSObject* posObj = NULL;
	JSObject* speedObj = NULL; 
	float fRes[2];
	size_t len;

	//get obj_type
	JS_GetProperty(cx,defObj,"obj_type",&vp);
	if(!JSVAL_IS_VOID(vp))
	{
		JSString* jsstr;
		if(JSVAL_IS_STRING(vp))
		{
			jsstr=JSVAL_TO_STRING(vp);
		}
		else
		{
			jsstr=JS_ValueToString(cx,vp);
		}
		const jschar* p = JS_GetStringCharsAndLength(cx,jsstr,&len);
		WCharArrToString((unsigned short*)p,this->obj_type);
	}



	//get pos info
	JS_GetProperty(cx,defObj,"pos",&vp);
	if(!JSVAL_IS_VOID(vp))
	{
		posObj = JSVAL_TO_OBJECT(vp); 
		for(int i = 0;;i++)
		{
			JS_GetElement(cx,posObj,i,&vp);
			if(!JSVAL_IS_VOID(vp))
			{ 
				if(JSVAL_IS_DOUBLE(vp))
				{
					fRes[i] = (float)(JSVAL_TO_DOUBLE(vp));
				}
				else
				{
					fRes[i] = (float)(JSVAL_TO_INT(vp));
				}
			}
			else
				break;
		}
		this->pos.x = fRes[0];
		this->pos.y = fRes[1];
	}


	//get speed info
	JS_GetProperty(cx,defObj,"speed",&vp);
	if(!JSVAL_IS_VOID(vp))
	{
		speedObj = JSVAL_TO_OBJECT(vp);
		for(int i = 0;;i++)
		{
			JS_GetElement(cx,speedObj,i,&vp);
			if(!JSVAL_IS_VOID(vp))
			{
				if(JSVAL_IS_DOUBLE(vp))
				{
					fRes[i] = (float)(JSVAL_TO_DOUBLE(vp));
				}
				else
				{
					fRes[i] = (float)(JSVAL_TO_INT(vp));
				}
			}
			else
				break;
		}
		this->speed.x = fRes[0];
		this->speed.y = fRes[1];
	}
}


//native class
BDGameScene::BDGameScene()
{
	lpGameLayer = NULL;
}

BDGameScene::~BDGameScene()
{
	m_pChildren->removeAllObjects();
}

BDGameScene *BDGameScene::Create()
{
	BDGameScene* pRet = new BDGameScene();

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		pRet->scheduleUpdate();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

BDGameLayer* BDGameScene::GetGameLayer()
{
	return lpGameLayer;
}

void BDGameScene::SetGameLayer(BDGameLayer* pGameLayer)
{
	lpGameLayer = pGameLayer;
}

void BDGameScene::update(float delta)
{
	CCScene::update(delta);
	
	if ( m_pChildren && m_pChildren->count() > 0 )
	{
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CCNode* pNode = (CCNode*) child;
			if (pNode)
			{
				pNode->update(delta);
			}
		}
	}
}


BDObject* BDGameScene::AddSceneUnit(JSContext* cx,BDSceneUnitDef& def)//this function will be deleted later
{
	if(def.obj_type.length() == 0)
	{
		CCLog("obj_type length is 0!");  
		return NULL;
	}

	if(def.obj_type.find("GameLayer")>=0)
	{
		BDGameLayer* pLayer = BDGameLayer::CreateWithScene(this);
		//JSObject* jsObj = JS_NewObject(cx,jsb_BDGameLayer_class,jsb_BDGameLayer_prototype,jsb_CCLayer_prototype);
		//CCAssert(jsObj!=NULL,"---BDGameScene::AddSceneUnit jsObj is NULL");
		//pLayer->SetJSObject(jsObj);
		pLayer->setTouchEnabled(true);
		pLayer->InitWorld();
		return pLayer;
	}

	return NULL;
}

BDObject* BDGameScene::AddSceneUnit(std::string& unit_type)
{
	if(unit_type.find("GameLayer")!= std::string::npos)
	{
		BDGameLayer* pLayer = BDGameLayer::CreateWithScene(this);
		pLayer->InitWorld();
		pLayer->setTouchEnabled(true);
		SetGameLayer(pLayer);
		return pLayer;
	}

	if(unit_type.find("SceneryLayer")!= std::string::npos)
	{
		BDSceneryLayer* pLayer = BDSceneryLayer::CreateWithScene(this);
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
		BDGameScene* ret = BDGameScene::Create();
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

JSBool js_bdgamescene_addSceneUnit(JSContext *cx, uint32_t argc, jsval *vp)
{
	const jschar* p;
	std::string unit_type;
	size_t len;
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
		JSString* jsstr;
		BDObject* newUnit = NULL;
		JSObject* jsObj = NULL;

		if(JSVAL_IS_STRING(argv[0]))
		{
			jsstr=JSVAL_TO_STRING(argv[0]);
		}
		else
		{
			jsstr=JS_ValueToString(cx,argv[0]);
		}
		p = JS_GetStringCharsAndLength(cx,jsstr,&len);
		WCharArrToString((unsigned short*)p,unit_type);
		newUnit = cobj->AddSceneUnit(unit_type);

		/*BDSceneUnitDef def;
		JSObject* defObj = JSVAL_TO_OBJECT(argv[0]);
		BDObject* newObj = NULL;
		JSObject* jsObj = NULL;

		def.ApplyFromDefObj(cx,defObj);
		newUnit = cobj->AddSceneUnit(cx,def);*/
		CCAssert(newUnit!=NULL,"---js_bdgamescene_addSceneUnit newUnit is NULL!");

		jsval jsret = JSVAL_NULL;
		do {
			if (newUnit->GetType() == GAME_LAYER) {
				js_proxy_t *proxy = js_get_or_create_proxy<BDGameLayer>(cx, (BDGameLayer*)newUnit);
				jsret = OBJECT_TO_JSVAL(proxy->obj);
				newUnit->SetJSObject(proxy->obj);
			}
			if (newUnit->GetType() == SCENERY_LAYER) {
				js_proxy_t *proxy = js_get_or_create_proxy<BDSceneryLayer>(cx, (BDSceneryLayer*)newUnit);
				jsret = OBJECT_TO_JSVAL(proxy->obj);
				newUnit->SetJSObject(proxy->obj);
			}
		} while (0);

		JS_SET_RVAL(cx, vp, jsret);
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
		JS_FN("addSceneUnit", js_bdgamescene_addSceneUnit, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
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