#include "Objects/BDCharacter.h"
USING_NS_CC;

JSClass  *jsb_BDCharacter_class = NULL;
JSObject *jsb_BDCharacter_prototype = NULL;


//native class
//-----------------------------------------------------------------------------------------------------------
BDCharacter::BDCharacter()
{
	lpGameWorld = NULL;
}

BDCharacter* BDCharacter::CreateCharacter(const char* srcFile,const char* armatureName)
{
	BDCharacter* pRet = NULL;
	extension::CCArmature* pArmature = NULL;

	pRet = new BDCharacter();
	CCLog("about to add json file to armatureDataManager:%s ",srcFile);
	extension::CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(srcFile);//add the armatrues in the data to cache
	pArmature = extension::CCArmature::create(armatureName);//create armatrue from cache
	pRet->lpArmature = pArmature;
	pRet->scheduleUpdate();
	return pRet;
}


void BDCharacter::SetGameWorld(b2World* world)
{
	lpGameWorld = world;
}

void BDCharacter::GrowBody()
{
	// Define the dynamic body.
	//Set up a 1m squared box in the physics world
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	b2Body *body = lpGameWorld->CreateBody(&bodyDef);

	////// Define another box shape for our dynamic body.
	////b2PolygonShape dynamicBox;
	////dynamicBox.SetAsBox(.5f, .5f);//These are mid points for our 1m box

	////// Define the dynamic body fixture.
	////b2FixtureDef fixtureDef;
	////fixtureDef.shape = &dynamicBox;
	////fixtureDef.isSensor = true;
	////body->CreateFixture(&fixtureDef);


	////bullet->setB2Body(body);
	////bullet->setPTMRatio(PT_RATIO);
	////bullet->setPosition( ccp( -100, -100) );

	body = lpGameWorld->CreateBody(&bodyDef);  
	lpArmature->setBody(body);
}

void BDCharacter::PlayAnimation(char* anim)
{
	lpArmature->getAnimation()->play(anim);
}

extension::CCArmature* BDCharacter::GetArmature()
{
	return lpArmature;
}

void BDCharacter::SetArmature(extension::CCArmature* pArmature)
{
	lpArmature = pArmature;
}

void BDCharacter::visit()
{
	lpArmature->visit();
}

void BDCharacter::update(float dt)
{
	lpArmature->update(dt);
}

//JSBinding
//-------------------------------------------------------------------------------------------------
JSBool js_bdcharacter_sayHello(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);   
	JSBool ok = JS_TRUE;

	JSObject *obj = NULL;
	BDCharacter* cobj = NULL;
	obj = JS_THIS_OBJECT(cx, vp); 
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	cobj = (BDCharacter*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");   

	JS_ReportError(cx, "wrong number of arguments");
	return JS_FALSE;
}

JSBool js_bdcharacter_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	if (argc == 0) {
		BDCharacter* cobj = new BDCharacter();
		cocos2d::CCObject *_ccobj = dynamic_cast<cocos2d::CCObject *>(cobj);
		if (_ccobj) {
			_ccobj->autorelease();
		}
		TypeTest<BDCharacter> t;
		js_type_class_t *typeClass;
		uint32_t typeId = t.s_id();
		HASH_FIND_INT(_js_global_type_ht, &typeId, typeClass);
		assert(typeClass);
		JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
		// link the native object with the javascript object
		js_proxy_t* p = jsb_new_proxy(cobj, obj);
		JS_AddNamedObjectRoot(cx, &p->obj, "BDCharacter");
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	return JS_FALSE;
}

void js_bdcharacter_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOGINFO("jsbindings: finalizing JS object %p (BDCharacter)", obj);
}


void js_register_bdcharacter(JSContext *cx, JSObject *global) {
	jsb_BDCharacter_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_BDCharacter_class->name = "BDCharacter";
	jsb_BDCharacter_class->addProperty = JS_PropertyStub;
	jsb_BDCharacter_class->delProperty = JS_PropertyStub;
	jsb_BDCharacter_class->getProperty = JS_PropertyStub;
	jsb_BDCharacter_class->setProperty = JS_StrictPropertyStub;
	jsb_BDCharacter_class->enumerate = JS_EnumerateStub;
	jsb_BDCharacter_class->resolve = JS_ResolveStub;
	jsb_BDCharacter_class->convert = JS_ConvertStub;
	jsb_BDCharacter_class->finalize = js_bdcharacter_finalize;
	jsb_BDCharacter_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		{0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
	};

	static JSFunctionSpec funcs[] = {
		JS_FN("sayHello", js_bdcharacter_sayHello, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	/*static JSFunctionSpec st_funcs[] = {
	JS_FN("create", js_bdgamelayer_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_FS_END
	};*/

	jsb_BDCharacter_prototype = JS_InitClass(
		cx, global,
		jsb_CCArmature_prototype, // parent proto
		jsb_BDCharacter_class,
		js_bdcharacter_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		NULL/*st_funcs*/);

	// make the class enumerable in the registered namespace
	JSBool found;
	JS_SetPropertyAttributes(cx, global, "BDCharacter", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

	// add the proto and JSClass to the type->js info hash table
	TypeTest<BDCharacter> t;
	js_type_class_t *p;
	uint32_t typeId = t.s_id();
	HASH_FIND_INT(_js_global_type_ht, &typeId, p);
	if (!p) {
		p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
		p->type = typeId;
		p->jsclass = jsb_BDCharacter_class;
		p->proto = jsb_BDCharacter_prototype;
		p->parentProto = jsb_CCArmature_prototype;
		HASH_ADD_INT(_js_global_type_ht, type, p);
	}
}

void register_all_bdcharacter(JSContext* cx, JSObject* obj) {
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

	js_register_bdcharacter(cx, obj);
	//JS_DefineFunction(cx, jsb_BDGameLayer_prototype, "retain", js_bdgamelayer_retain, 0, JSPROP_READONLY | JSPROP_PERMANENT);
}