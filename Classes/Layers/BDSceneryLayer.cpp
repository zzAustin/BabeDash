#include "Layers/BDSceneryLayer.h"
#include "Layers/BDGameLayer.h"
#include "Scenes/BDGameScene.h"
#include "Util/CommonUtil.h"

USING_NS_CC;

JSClass  *jsb_BDSceneryLayer_class = NULL;
JSObject *jsb_BDSceneryLayer_prototype = NULL;

//def class
//------------------------------------------------------------
BDSceneryLayerDef::BDSceneryLayerDef()
{
	m_ptPos.x = 0.0f;
	m_ptPos.y = 0.0f;
	m_ptSpeed.x = 0.0f;
	m_ptSpeed.y = 0.0f;
	m_strSpriteRes1 = "";
	m_strSpriteRes2 = "";
};

void BDSceneryLayerDef::ApplyFromDefObj(JSContext* cx,JSObject* defObj)
{
	jsval vp = JSVAL_VOID;
	JSObject* posObj = NULL;
	JSObject* speedObj = NULL; 
	float fRes[2];
	size_t len;

	//get obj_type
	JS_GetProperty(cx,defObj,"map1",&vp);
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
		WCharArrToString((unsigned short*)p,this->m_strSpriteRes1);
	}

	//get res
	JS_GetProperty(cx,defObj,"map2",&vp);
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
		WCharArrToString((unsigned short*)p,this->m_strSpriteRes2);
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
		this->m_ptPos.x = fRes[0];
		this->m_ptPos.y = fRes[1];
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
		this->m_ptSpeed.x = fRes[0];
		this->m_ptSpeed.y = fRes[1];
	}
}


//native class
//----------------------------------------------------------------------------------------
BDSceneryLayer::BDSceneryLayer()
{
	m_lpSprite1 = NULL;
	m_lpSprite2 = NULL;
	m_lpGameScene = NULL;
}

BDSceneryLayer::BDSceneryLayer(BDGameScene* pScene)
{
	m_lpSprite1 = NULL;
	m_lpSprite2 = NULL;
	m_lpGameScene = pScene;
}

BDSceneryLayer::~BDSceneryLayer()
{

}


BDSceneryLayer *BDSceneryLayer::CreateWithScene(BDGameScene* pScene)
{
	BDSceneryLayer* pRet = new BDSceneryLayer(pScene);

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		//pRet->scheduleUpdate();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}

	return pRet;
}

BDSceneryLayer *BDSceneryLayer::Create()
{
	BDSceneryLayer* pRet = new BDSceneryLayer();

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		//pRet->scheduleUpdate();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}

	return pRet;
}

int BDSceneryLayer::GetType()
{
	return SCENERY_LAYER;
}

bool BDSceneryLayer::InitWithAttributes(BDSceneryLayerDef& def) 
{
	CCSize size;  
	m_lpSprite1 = CCSprite::create(def.m_strSpriteRes1.c_str());
	m_lpSprite2 = CCSprite::create(def.m_strSpriteRes2.c_str());  
	m_ptSpeed = def.m_ptSpeed; 
	m_lpSprite1->setPosition(def.m_ptPos);   
	size = m_lpSprite1->getContentSize();  
	m_lpSprite2->setPositionX(size.width);   
	m_lpSprite2->setPositionY(def.m_ptPos.y);   

	this->addChild(m_lpSprite1);
	this->addChild(m_lpSprite2);
	m_lpSprite1->setAnchorPoint(ccp(0,0));
	m_lpSprite2->setAnchorPoint(ccp(0,0));

	return true;
}

void BDSceneryLayer::update(float delta)
{
	if(m_lpSprite1 == NULL || m_lpSprite2 == NULL)
		return;

	CCSprite* front = NULL;
	CCSprite* behind = NULL;
	BDGameLayer* gameLayer = NULL;
	float screenSpeedX = 0.0f;
	float refSpeedX = 0.0f;

	

	float x1 = m_lpSprite1->getPositionX();
	float x2 = m_lpSprite2->getPositionX();

	//the left one is defined 'front'
	if(x1<x2)
	{
		front = m_lpSprite1;
		behind = m_lpSprite2;
	}
	else{
		front = m_lpSprite2;
		behind = m_lpSprite1;
	}

	//Get the viewport after the resolution adaptation
	CCRect rect = CCEGLView::sharedOpenGLView()->getViewPortRect();
	
	gameLayer = m_lpGameScene->GetGameLayer();
	if(gameLayer->GetMainCharacter() != NULL && gameLayer->GetMainCharacter()->GetSpeed().x > 0)//main character is moving forward logically
	{
		if(behind->getPositionX()<= (0-(behind->getContentSize().width - rect.size.width)))
		{
			front->setPositionX(behind->getPositionX()+behind->getContentSize().width);
		}
	}
	
	if(gameLayer->GetMainCharacter() != NULL && gameLayer->GetMainCharacter()->GetSpeed().x < 0)//main character is moving backwards logically
	{
		if(front->getPositionX() >= 0)
		{
			behind->setPositionX(0-behind->getContentSize().width);
		}
	}

	
	refSpeedX = gameLayer->GetMainCharacter()->GetSpeed().x;
	
	screenSpeedX = SpeedLogicToScreen(m_ptSpeed.x,refSpeedX);

	m_lpSprite1->setPositionX(m_lpSprite1->getPositionX()+screenSpeedX);
	m_lpSprite2->setPositionX(m_lpSprite2->getPositionX()+screenSpeedX);

}

CCSprite* BDSceneryLayer::GetSprite1()
{
	return m_lpSprite1;
}

CCSprite* BDSceneryLayer::GetSprite2()
{
	return m_lpSprite2;
}


//--------------------------------------------------------------------------------------------
//JS Binding
JSBool js_bdscenerylayer_create(JSContext *cx, uint32_t argc, jsval *vp)
{
	if (argc == 0) {
		BDSceneryLayer* ret = BDSceneryLayer::Create();  
		jsval jsret;
		do {
			if (ret) {
				js_proxy_t *proxy = js_get_or_create_proxy<BDSceneryLayer>(cx, ret);
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

JSBool js_bdscenerylayer_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	if (argc == 0) {
		BDSceneryLayer* cobj = BDSceneryLayer::Create();
		cocos2d::CCObject *_ccobj = dynamic_cast<cocos2d::CCObject *>(cobj);
		if (_ccobj) {
			_ccobj->autorelease();
		}
		TypeTest<BDSceneryLayer> t;
		js_type_class_t *typeClass;
		uint32_t typeId = t.s_id();
		HASH_FIND_INT(_js_global_type_ht, &typeId, typeClass);
		assert(typeClass);
		JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
		// link the native object with the javascript object
		js_proxy_t* p = jsb_new_proxy(cobj, obj);
		JS_AddNamedObjectRoot(cx, &p->obj, "BDSceneryLayer");
		return JS_TRUE;
	}

	JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 0);
	return JS_FALSE;
}

void js_bdscenerylayer_finalize(JSFreeOp *fop, JSObject *obj) {  
	CCLOGINFO("jsbindings: finalizing JS object %p (BDSceneryLayer)", obj);
}

JSBool js_bdscenerylayer_initwithattributes(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);  
	JSBool ok = JS_TRUE;

	JSObject *obj = NULL;
	BDSceneryLayer* cobj = NULL;
	obj = JS_THIS_OBJECT(cx, vp); 
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	cobj = (BDSceneryLayer*)(proxy ? proxy->ptr : NULL);
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
		
		BDSceneryLayerDef def;
		JSObject* defObj = JSVAL_TO_OBJECT(argv[0]); 
		JSObject* jsObj = NULL;
		
		def.ApplyFromDefObj(cx,defObj); 
		cobj->InitWithAttributes(def);

		return JS_TRUE;    
	}
	
	JS_ReportError(cx, "wrong number of arguments");  
	return JS_FALSE;
}

void js_register_bdscenerylayer(JSContext *cx, JSObject *global) {
	jsb_BDSceneryLayer_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_BDSceneryLayer_class->name = "BDSceneryLayer";
	jsb_BDSceneryLayer_class->addProperty = JS_PropertyStub;
	jsb_BDSceneryLayer_class->delProperty = JS_PropertyStub;
	jsb_BDSceneryLayer_class->getProperty = JS_PropertyStub;
	jsb_BDSceneryLayer_class->setProperty = JS_StrictPropertyStub;
	jsb_BDSceneryLayer_class->enumerate = JS_EnumerateStub;
	jsb_BDSceneryLayer_class->resolve = JS_ResolveStub;
	jsb_BDSceneryLayer_class->convert = JS_ConvertStub;
	jsb_BDSceneryLayer_class->finalize = js_bdscenerylayer_finalize;
	jsb_BDSceneryLayer_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		{0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
	};

	static JSFunctionSpec funcs[] = {
		JS_FN("initWithAttributes", js_bdscenerylayer_initwithattributes, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
		JS_FN("Create", js_bdscenerylayer_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	jsb_BDSceneryLayer_prototype = JS_InitClass(
		cx, global,
		jsb_CCLayer_prototype, // parent proto
		jsb_BDSceneryLayer_class,
		js_bdscenerylayer_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs/*st_funcs*/);

	// make the class enumerable in the registered namespace
	JSBool found;
	JS_SetPropertyAttributes(cx, global, "BDSceneryLayer", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

	// add the proto and JSClass to the type->js info hash table
	TypeTest<BDSceneryLayer> t;
	js_type_class_t *p;
	uint32_t typeId = t.s_id();
	HASH_FIND_INT(_js_global_type_ht, &typeId, p);
	if (!p) {
		p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
		p->type = typeId;
		p->jsclass = jsb_BDSceneryLayer_class;
		p->proto = jsb_BDSceneryLayer_prototype;
		p->parentProto = jsb_CCLayer_prototype;
		HASH_ADD_INT(_js_global_type_ht, type, p);
	}
}

void register_all_bdscenerylayer(JSContext* cx, JSObject* obj) {
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

	js_register_bdscenerylayer(cx, obj);

	//JS_DefineFunction(cx, jsb_BDGameLayer_prototype, "retain", js_bdgamelayer_retain, 0, JSPROP_READONLY | JSPROP_PERMANENT);
}