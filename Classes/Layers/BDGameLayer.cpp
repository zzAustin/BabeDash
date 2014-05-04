#include "Layers/BDGameLayer.h"
#include "Util/CommonUtil.h"
#include "Defs/DBDef.h"
#include "Objects/BDCharacter.h"

#include "Actions/BDJumpAction.h"//testing...could be deleted
#include "Layers/BDGameLayer.h"
#include "Particles/BDParticleSystem.h"


USING_NS_CC;
using namespace cocos2d::extension;

JSClass  *jsb_BDGameLayer_class = NULL;
JSObject *jsb_BDGameLayer_prototype = NULL;


//def class
//------------------------------------------------------------
BDGameObjDef::BDGameObjDef()
{
	obj_type = "null";
	pos.x = 0.0f;
	pos.y = 0.0f;
	speed.x = 0.0f;
	speed.y = 0.0f;
	group = -1;
	state = -1;
	lpMovementAttr = NULL;
};

BDGameObjDef::~BDGameObjDef()
{
	CC_SAFE_DELETE(lpMovementAttr);
}

void BDGameObjDef::ApplyFromDefObj(JSContext* cx,JSObject* defObj)
{
	jsval vp = JSVAL_VOID;
	JSObject* posObj = NULL;
	JSObject* speedObj = NULL;
	JSObject* scaleObj = NULL;
	float* fRes = NULL;
	size_t len;

	//get obj_type
	this->obj_type = GetStrJSVal("obj_type",defObj,cx);

	//get res	
	this->res =  GetStrJSVal("res",defObj,cx);

	//get armature
	this->armature =  GetStrJSVal("armature",defObj,cx);

	//get pos info
	fRes = GetDoubleArrayJSVal("pos", defObj, cx, 2);

	if(fRes != NULL)
	{
		this->pos.x = fRes[0];
		this->pos.y = fRes[1];
	}


	//get speed info
	fRes = GetDoubleArrayJSVal("speed", defObj, cx, 2);

	if(fRes != NULL)
	{
		this->speed.x = fRes[0];
		this->speed.y = fRes[1];
	}

	//get scale info
	fRes = GetDoubleArrayJSVal("scale", defObj, cx, 2);

	if(fRes != NULL)
	{
		this->scale.x = fRes[0];
		this->scale.y = fRes[1];
	}

	//get group
	this->group = GetIntJSVal("group", defObj, cx);
	

	//get state	
	this->state = GetIntJSVal("state", defObj, cx);


	JS_GetProperty(cx,defObj,"movement_component",&vp);
	
	jsval v = JSVAL_NULL;
	JSObject* moveCompObj = NULL;

	if(!JSVAL_IS_VOID(vp))
	{
		moveCompObj = JSVAL_TO_OBJECT(vp);
		lpMovementAttr = new BDMovementAttr();
			
		fRes = GetDoubleArrayJSVal("speed", moveCompObj, cx, 2);

		if(fRes != NULL)
		{
			lpMovementAttr->ptSpeed.x = fRes[0];
			lpMovementAttr->ptSpeed.y = fRes[1];
		}

	}
}

//box2d
//----------------------------------------------------------------------------------------------
void ContactListener::BeginContact(b2Contact *contact) 
{
	if (contact)  
	{
		Contact c;
		c.fixtureA = contact->GetFixtureA(); 
		c.fixtureB = contact->GetFixtureB();

		contact_list.push_back(c);
	}
	B2_NOT_USED(contact);
}

void ContactListener::EndContact(b2Contact *contact)
{
	contact_list.clear();
	B2_NOT_USED(contact);
}

void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
	B2_NOT_USED(contact);
	B2_NOT_USED(oldManifold);
}

void ContactListener::PostSolve(const b2Contact *contact, const b2ContactImpulse *impulse)
{
	B2_NOT_USED(contact);
	B2_NOT_USED(impulse);
}

//native class
BDGameLayer::~BDGameLayer()
{
    m_pChildren->removeAllObjects();
	CC_SAFE_DELETE(m_lpGameWorld);
	CC_SAFE_DELETE(m_lpDebugDraw);
}


BDGameLayer::BDGameLayer()
{
	m_lpGameWorld = NULL;
	m_lpDebugDraw = NULL;
	m_lpGameScene = NULL;
	m_lpMainCharacter = NULL;

	bullet = NULL;
}

BDGameLayer::BDGameLayer(BDGameScene* pScene)
{
	m_lpGameWorld = NULL;
	m_lpDebugDraw = NULL;
	m_lpMainCharacter = NULL;
	m_lpGameScene = pScene;

	bullet = NULL;
}

void BDGameLayer::onFrameEvent(extension::CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex)
{
    CCLOG("(%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt, currentFrameIndex);

    /*
    * originFrameIndex is the frame index editted in Action Editor
    * currentFrameIndex is the current index animation played to
    * frame event may be delay emit, so originFrameIndex may be different from currentFrameIndex.
    */

    CCPoint p = mainCharacter->GetArmature()->getBone("Layer126")->getDisplayRenderNode()->convertToWorldSpaceAR(ccp(0, 0));
    bullet->setPosition(ccp(p.x + 60, p.y));

    bullet->stopAllActions();
    bullet->runAction(CCMoveBy::create(1.5f, ccp(350, 0)));
}


//--------------------------------------------------------------------------------------------
BDGameLayer *BDGameLayer::CreateWithScene(BDGameScene* pScene)
{
	BDGameLayer* pRet = new BDGameLayer(pScene);

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		//pRet->scheduleUpdate();
		
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

BDGameLayer *BDGameLayer::Create()
{
	BDGameLayer* pRet = new BDGameLayer();

	if (pRet && pRet->init())
	{
		pRet->autorelease();
		//pRet->scheduleUpdate();

		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

void BDGameLayer::HandleCollision(BDObject* objA,BDObject* objB)
{
	jschar sSkillName[128];
	jsval jv=JSVAL_VOID;
	jsval retval = JSVAL_NULL;
	jsval v[2];

	jv = OBJECT_TO_JSVAL(objA->GetJSObject());
	v[0] = jv;
	jv = OBJECT_TO_JSVAL(objB->GetJSObject());
	v[1] = jv;

	ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(ScriptingCore::getInstance()->getGlobalObject()),
		"handleCollision", 2, v, &retval);
	//res = JSVAL_TO_BOOLEAN(retval); 
}

void BDGameLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	BDCharacter* pMainCharacter = GetMainCharacter();
	if(pMainCharacter == NULL)
		return; 

	/*BDAction* pJumpAction = new BDJumpAction(pMainCharacter,-0.1f);
	CCPoint sp = pMainCharacter->GetMovementComp()->GetCurSpeed();
	if(pMainCharacter->GetCurBDAction() == NULL)
		pMainCharacter->GetMovementComp()->SetCurSpeed(pMainCharacter->GetMovementComp()->GetOrigSpeed()); 
	pMainCharacter->SetCurBDAction(pJumpAction);*/
	pMainCharacter->PlaySkill(BDSKILL_JUMP);
}

int BDGameLayer::GetType() 
{
	return GAME_LAYER;
}

void BDGameLayer::SetMainCharacter(BDCharacter* pMainCharacter)
{
	m_lpMainCharacter = pMainCharacter;
}

BDCharacter* BDGameLayer::GetMainCharacter()
{
	return m_lpMainCharacter;
}

BDObject* BDGameLayer::AddGameObject(JSContext* cx,BDGameObjDef& def)
{
	if(def.obj_type.length() == 0)
	{
		CCLog("---BDGameLayer::AddGameObject--- obj_type length is 0!");
		return NULL;
	}

	if(def.obj_type.find("Character") != std::string::npos)
	{
		
		BDCharacter* pCharacter = NULL;
		pCharacter = BDCharacter::CreateWithGameLayer(this);
		CCAssert(pCharacter != NULL,"---BDGameLayer::AddGameObject--- newly created pCharacter is NULL!");
	/*	JSObject* jsObj = JS_NewObject(cx,jsb_BDCharacter_class,jsb_BDCharacter_prototype,jsb_CCArmature_prototype);
		CCAssert(jsObj!=NULL,"---BDGameLayer::AddGameObject jsObj is NULL");
		pCharacter->SetJSObject(jsObj);*/
		pCharacter->BuildArmature(def.res.c_str(),def.armature.c_str());

		//We have to set pCharacter and its armature the same position here
		pCharacter->SetPosition(def.pos.x,def.pos.y,true);
		//also record the logic position of the character
		pCharacter->SetLogicPosition(def.pos.x,def.pos.y);

		pCharacter->GetArmature()->setScaleX(def.scale.x);
		pCharacter->GetArmature()->setScaleY(def.scale.y);
		pCharacter->SetCurState(def.state);

		//BDAction test...to be deleted----------------------------
		if(def.state == BDCharacter::STATE_IDLE)
		{
			//BDAction* pMoveAction = new BDMoveAction(pCharacter);
			//pCharacter->SetCurBDAction(pMoveAction);
		}
		//----------------------------------------------------------

		//pCharacter->PlayAnimation("FireWithoutBullet");
		pCharacter->SetGameWorld(m_lpGameWorld);
		pCharacter->GrowBody();
		pCharacter->SetGroup(def.group);

		if(def.group == GROUP_BABE)//set the main character pointer to GameLayer
		{
			SetMainCharacter(pCharacter);
	        pCharacter->SetIsMainCharacter(true);

			/*BDParticleSystem* m_emitter = BDParticleSystem::create("lavaflow.plist",true);
			m_emitter->retain();
			pCharacter->addChild(m_emitter, 10);
			m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("fire.png") );
			m_emitter->setPosition(-50.0f,0.0f);*/

		}
		else
		{
			/*BDParticleSystem* m_emitter = BDParticleSystem::createWithTotalParticles(600,false);
			m_emitter->retain();
			pCharacter->addChild(m_emitter, 10);
			m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("fire.png") );
			m_emitter->setPosition(50.0f,0.0f);*/
		}
		if(def.lpMovementAttr != NULL) 
		{
			pCharacter->SetMovementComp(new BDMovementComp(def.lpMovementAttr->ptSpeed));
			pCharacter->addComponent(pCharacter->GetMovementComp());
			pCharacter->GetMovementComp()->setEnabled(true);
		} 

		//if(bullet == NULL)
		//{
		//	bullet = extension::CCPhysicsSprite::createWithSpriteFrameName("25.png");
		//	addChild(bullet);

		//	// Define the dynamic body.
		//	//Set up a 1m squared box in the physics world
		//	b2BodyDef bodyDef;
		//	bodyDef.type = b2_dynamicBody;

		//	b2Body *body = m_lpGameWorld->CreateBody(&bodyDef);

		//	// Define another box shape for our dynamic body.
		//	b2PolygonShape dynamicBox;
		//	dynamicBox.SetAsBox(.5f, .5f);//These are mid points for our 1m box

		//	// Define the dynamic body fixture.
		//	b2FixtureDef fixtureDef;
		//	fixtureDef.shape = &dynamicBox;
		//	fixtureDef.isSensor = true;
		//	body->CreateFixture(&fixtureDef);


		//	bullet->setB2Body(body);
		//	bullet->setPTMRatio(PT_RATIO);
		//	bullet->setPosition( ccp( -100, -100) );

		//	mainCharacter = pCharacter;
		//	mainCharacter->GetArmature()->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(BDGameLayer::onFrameEvent));
		//}
		//else
		//{
		//	pCharacter->PlayAnimation("Walk");
		//	enemy = pCharacter;
		//
		return pCharacter;
	}
	
	return NULL;
}

void BDGameLayer::InitWorld()
{
	b2Vec2 noGravity(0, 0);

	m_lpGameWorld = new b2World(noGravity);
	m_lpGameWorld->SetAllowSleeping(true);

	m_lpContactListener = new ContactListener();
	m_lpGameWorld->SetContactListener(m_lpContactListener);

	m_lpDebugDraw = new GLESDebugDraw( PT_RATIO );
	m_lpGameWorld->SetDebugDraw(m_lpDebugDraw);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//        flags += b2Draw::e_jointBit;
	//        flags += b2Draw::e_aabbBit;
	//        flags += b2Draw::e_pairBit;
	//        flags += b2Draw::e_centerOfMassBit;
	m_lpDebugDraw->SetFlags(flags);
}

void BDGameLayer::draw() 
{
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
	kmGLPushMatrix();
	m_lpGameWorld->DrawDebugData();  
	kmGLPopMatrix();
}

void BDGameLayer::update(float delta)
{
	CCLayer::update(delta);

	//Update the children
	CCObject* child = NULL;
	CCARRAY_FOREACH(m_pChildren, child)                                
	{                                                             
		CCNode* pNode = (CCNode*)child;                 
		if(pNode)                                                 
		{                                                         
			 pNode->update(delta);                                        
		}                                                         
	}   

	//step the physics world
	int32 velocityIterations = 8;   //how strongly to correct velocity
	int32 positionIterations = 3;   //how strongly to correct position
	m_lpGameWorld->Step(delta, velocityIterations, positionIterations);

	//enemy->GetArmature()->setVisible(true);
	for (std::list<Contact>::iterator it = m_lpContactListener->contact_list.begin(); it != m_lpContactListener->contact_list.end(); ++it)
	{
		Contact &contact = *it;

		CCBone *ba = (CCBone *)contact.fixtureA->GetUserData();
		CCBone *bb = (CCBone *)contact.fixtureB->GetUserData();
		 
		extension::CCArmature* aa = NULL;
		extension::CCArmature* ab = NULL;
		BDArmature* ba_a = NULL;
		BDArmature* ba_b = NULL;
		BDObject* owner_a = NULL;
		BDObject* owner_b = NULL;

		aa = ba->getArmature();
		ba_a = dynamic_cast<BDArmature*>(aa);
		owner_a = ba_a->GetOwner();

		ab = bb->getArmature();
		ba_b = dynamic_cast<BDArmature*>(ab);
		owner_b = ba_b->GetOwner();

		HandleCollision(owner_a,owner_b);
	
	/*	if( bb != NULL)
			bb->getArmature()->setVisible(false);*/
	}
}

//--------------------------------------------------------------------------------------------
//JS Binding
JSBool js_bdgamelayer_create(JSContext *cx, uint32_t argc, jsval *vp)
{
	if (argc == 0) {
		BDGameLayer* ret = BDGameLayer::Create();  
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

JSBool js_bdgamelayer_addGameObject(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);  
	JSBool ok = JS_TRUE;

	JSObject *obj = NULL;
	BDGameLayer* cobj = NULL;
	obj = JS_THIS_OBJECT(cx, vp); 
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	cobj = (BDGameLayer*)(proxy ? proxy->ptr : NULL);
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
		BDGameObjDef def;
		JSObject* defObj = JSVAL_TO_OBJECT(argv[0]); 
		BDObject* newObj = NULL;
		JSObject* jsObj = NULL;
		
		def.ApplyFromDefObj(cx,defObj); 
		newObj = cobj->AddGameObject(cx,def);

		jsval jsret;
		do {
			if (newObj) {
				js_proxy_t *proxy = js_get_or_create_proxy<BDCharacter>(cx, (BDCharacter*)newObj);
				jsret = OBJECT_TO_JSVAL(proxy->obj);
				newObj->SetJSObject(proxy->obj);
			} else {
				jsret = JSVAL_NULL;
			}
		} while (0);
		JS_SET_RVAL(cx, vp, jsret);

		CCAssert(newObj!=NULL,"---js_bdgamelayer_addGameObject newObj is NULL!");  
		//jsObj = newObj->GetJSObject();
		//*vp = (jsObj != NULL)?OBJECT_TO_JSVAL(jsObj):JSVAL_NULL;

		return JS_TRUE;    
	}
	
	JS_ReportError(cx, "wrong number of arguments");  
	return JS_FALSE;
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
		JS_FN("addGameObject", js_bdgamelayer_addGameObject, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
		JS_FN("Create", js_bdgamelayer_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
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


