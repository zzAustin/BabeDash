#include "CBabe.h"
#include "BabeJS.h"
#include "cocos2d.h"
#include "cocos2d_specifics.hpp"
USING_NS_CC;


JSBool js_cocos2dx_Babe_sayHello(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;

	JSObject *obj = NULL;
	Babe* cobj = NULL;
	obj = JS_THIS_OBJECT(cx, vp);
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	cobj = (Babe*)(proxy ? proxy->ptr : NULL);
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


//JS_DefineFunction will register this
JSBool js_cocos2dx_Babe_retain(JSContext* cx,uint32_t argc,jsval *vp)
{
	CCLog("in babe retain...");
	JSObject* thisObj = JS_THIS_OBJECT(cx,vp);

	if(thisObj){
		js_proxy_t* proxy = jsb_get_js_proxy(thisObj);

		if(proxy){
			((Babe*)proxy->ptr)->SayHello();
			CCLog("Babe Retain succeed!");
			return JS_TRUE;
		}
	}

	JS_ReportError(cx,"Invalid native object");
	return JS_FALSE;
}

JSBool js_cocos2dx_Babe_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	if (argc == 0) {
		Babe* cobj = new Babe();
		//cocos2d::CCObject *_ccobj = dynamic_cast<cocos2d::CCObject *>(cobj);
		//if (_ccobj) {
		//_ccobj->autorelease();
		//}
		TypeTest<Babe> t;
		js_type_class_t *typeClass;
		uint32_t typeId = t.s_id();
		HASH_FIND_INT(_js_global_type_ht, &typeId, typeClass);
		assert(typeClass);
		JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
		// link the native object with the javascript object
		js_proxy_t* p = jsb_new_proxy(cobj, obj);
		JS_AddNamedObjectRoot(cx, &p->obj, "Babe");
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	return JS_FALSE;
}

void js_babe_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOGINFO("jsbindings: finalizing JS object %p (Babe)", obj);
}

JSClass  *jsb_Babe_class;
JSObject *jsb_Babe_prototype;

void js_register_babe(JSContext *cx, JSObject *global) {
	jsb_Babe_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_Babe_class->name = "Babe";
	jsb_Babe_class->addProperty = JS_PropertyStub;
	jsb_Babe_class->delProperty = JS_PropertyStub;
	jsb_Babe_class->getProperty = JS_PropertyStub;
	jsb_Babe_class->setProperty = JS_StrictPropertyStub;
	jsb_Babe_class->enumerate = JS_EnumerateStub;
	jsb_Babe_class->resolve = JS_ResolveStub;
	jsb_Babe_class->convert = JS_ConvertStub;
	jsb_Babe_class->finalize = js_babe_finalize;
	jsb_Babe_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		{0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
	};

	static JSFunctionSpec funcs[] = {
		JS_FN("sayHello", js_cocos2dx_Babe_sayHello, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	/*static JSFunctionSpec st_funcs[] = {
	JS_FN("create", js_cocos2dx_CCNode_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_FS_END
	};*/

	jsb_Babe_prototype = JS_InitClass(
		cx, global,
		NULL, // parent proto
		jsb_Babe_class,
		js_cocos2dx_Babe_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		NULL/*st_funcs*/);
	// make the class enumerable in the registered namespace
	JSBool found;
	JS_SetPropertyAttributes(cx, global, "Babe", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

	// add the proto and JSClass to the type->js info hash table
	TypeTest<Babe> t;
	js_type_class_t *p;
	uint32_t typeId = t.s_id();
	HASH_FIND_INT(_js_global_type_ht, &typeId, p);
	if (!p) {
		p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
		p->type = typeId;
		p->jsclass = jsb_Babe_class;
		p->proto = jsb_Babe_prototype;
		p->parentProto = NULL;
		HASH_ADD_INT(_js_global_type_ht, type, p);
	}
}

void register_all_babedash(JSContext* cx, JSObject* obj) {
	// first, try to get the ns
	jsval nsval;
	JSObject *ns;
	JS_GetProperty(cx, obj, "BabeDomain", &nsval);
	if (nsval == JSVAL_VOID) {
		ns = JS_NewObject(cx, NULL, NULL, NULL);
		nsval = OBJECT_TO_JSVAL(ns);
		JS_SetProperty(cx, obj, "BabeDomain", &nsval);
	} else {
		JS_ValueToObject(cx, nsval, &ns);
	}
	obj = ns;

	js_register_babe(cx, obj);

	JS_DefineFunction(cx, jsb_Babe_prototype, "retain", js_cocos2dx_Babe_retain, 0, JSPROP_READONLY | JSPROP_PERMANENT);
}
