#include "Layers/BDGameLayer.h"
#include "Util/CommonUtil.h"
USING_NS_CC;

JSClass  *jsb_BDGameLayer_class;
JSObject *jsb_BDGameLayer_prototype;

BDGameLayerDef::BDGameLayerDef()
{
	this->obj_type = "null";
	this->pos.x = 0.0f;
	this->pos.y = 0.0f;
	this->speed.x = 0.0f;
	this->speed.y = 0.0f;
}

void BDGameLayerDef::ApplyFromDefObj(JSContext* cx,JSObject* defObj)
{
	jsval vp = JSVAL_VOID;
	JSObject* posObj = NULL;
	JSObject* speedObj = NULL; 
	float fRes[2];
	size_t len;

	//get obj_type
	JS_GetProperty(cx,defObj,"res",&vp);
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
		WCharArrToString((wchar_t*)p,this->obj_type);
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

BDGameLayer *BDGameLayer::create()
{
	BDGameLayer* pRet = new BDGameLayer();

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


//--------------------------------------------------------------------------------------------
//JS Binding
JSBool js_bdgamelayer_create(JSContext *cx, uint32_t argc, jsval *vp)
{
	if (argc == 0) {
		BDGameLayer* ret = BDGameLayer::create();
		jsval jsret;
		do {
			if (ret) {
				js_proxy_t *proxy = js_get_or_create_proxy<BDGameLayer>(cx, ret);
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


JSBool js_bdgamelayer_sayHello(JSContext *cx, uint32_t argc, jsval *vp)
{
	int kkk = 0;
	

	JS_ReportError(cx, "wrong number of arguments");
	return JS_FALSE;
}

JSBool js_bdgamelayer_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	if (argc == 0) {
		BDGameLayer* cobj = new BDGameLayer();
		cocos2d::CCObject *_ccobj = dynamic_cast<cocos2d::CCObject *>(cobj);
		if (_ccobj) {
			_ccobj->autorelease();
		}
		TypeTest<BDGameLayer> t;
		js_type_class_t *typeClass;
		uint32_t typeId = t.s_id();
		HASH_FIND_INT(_js_global_type_ht, &typeId, typeClass);
		assert(typeClass);
		JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
		// link the native object with the javascript object
		js_proxy_t* p = jsb_new_proxy(cobj, obj);
		JS_AddNamedObjectRoot(cx, &p->obj, "BDGameLayer");
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	return JS_FALSE;
}

void js_bdgamelayer_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOGINFO("jsbindings: finalizing JS object %p (BDGameLayer)", obj);
}

void js_register_bdgamelayer(JSContext *cx, JSObject *global) {
	jsb_BDGameLayer_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_BDGameLayer_class->name = "BDGameLayer";
	jsb_BDGameLayer_class->addProperty = JS_PropertyStub;
	jsb_BDGameLayer_class->delProperty = JS_PropertyStub;
	jsb_BDGameLayer_class->getProperty = JS_PropertyStub;
	jsb_BDGameLayer_class->setProperty = JS_StrictPropertyStub;
	jsb_BDGameLayer_class->enumerate = JS_EnumerateStub;
	jsb_BDGameLayer_class->resolve = JS_ResolveStub;
	jsb_BDGameLayer_class->convert = JS_ConvertStub;
	jsb_BDGameLayer_class->finalize = js_bdgamelayer_finalize;
	jsb_BDGameLayer_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		{0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
	};

	static JSFunctionSpec funcs[] = {
		JS_FN("sayHello", js_bdgamelayer_sayHello, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		//JS_FN("addObject", js_bdgamescene_addObject, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
		JS_FN("create", js_bdgamelayer_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	jsb_BDGameLayer_prototype = JS_InitClass(
		cx, global,
		jsb_CCLayer_prototype, // parent proto
		jsb_BDGameLayer_class,
		js_bdgamelayer_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs/*st_funcs*/);

	// make the class enumerable in the registered namespace
	JSBool found;
	JS_SetPropertyAttributes(cx, global, "BDGameLayer", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

	// add the proto and JSClass to the type->js info hash table
	TypeTest<BDGameLayer> t;
	js_type_class_t *p;
	uint32_t typeId = t.s_id();
	HASH_FIND_INT(_js_global_type_ht, &typeId, p);
	if (!p) {
		p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
		p->type = typeId;
		p->jsclass = jsb_BDGameLayer_class;
		p->proto = jsb_BDGameLayer_prototype;
		p->parentProto = jsb_CCLayer_prototype;
		HASH_ADD_INT(_js_global_type_ht, type, p);
	}
}

void register_all_bdgamelayer(JSContext* cx, JSObject* obj) {
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

	js_register_bdgamelayer(cx, obj);

	//JS_DefineFunction(cx, jsb_BDGameLayer_prototype, "retain", js_bdgamelayer_retain, 0, JSPROP_READONLY | JSPROP_PERMANENT);
}