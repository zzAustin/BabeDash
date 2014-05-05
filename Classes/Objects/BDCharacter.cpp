#include "Objects/BDCharacter.h"
#include "CocoStudio/Armature/CCBone.h";
#include "CocoStudio/Armature/physics/CCColliderDetector.h"
#include "Components/BDMovementComp.h"
#include "Layers/BDGameLayer.h"
#include "Particles/BDParticleSystem.h"
#include "Objects/firePngData.h"
#include "Util/CommonUtil.h"
#include "Actions/BDJumpAction.h"
USING_NS_CC;

JSClass  *jsb_BDCharacter_class = NULL;
JSObject *jsb_BDCharacter_prototype = NULL;

//native class
//-----------------------------------------------------------------------------------------------------------
BDArmature::BDArmature()
{

}

BDArmature::BDArmature(BDObject* pOwner)
{
	m_lpOwner = pOwner;
}

BDObject *BDArmature::GetOwner()
{
	return m_lpOwner;
}

BDArmature *BDArmature::create(const char *name,BDCharacter* pCharacter)
{
	BDArmature *armature = new BDArmature(pCharacter);
	if (armature && armature->init(name))
	{
		armature->autorelease();
		return armature;
	}
	CC_SAFE_DELETE(armature);
	return NULL;
}


BDCharacter::BDCharacter()
{
	m_iCurState = STATE_NULL;
	m_iPrevState = STATE_NULL;
	m_lpGameWorld = NULL;
	m_lpCurAction = NULL;
	//m_lpMovementComp = NULL;
	m_iGroup = -1;
	m_bIsMainCharacter = NULL;
	m_iType = OBJECT_CHARACTER;
}

BDCharacter::BDCharacter(BDGameLayer* pGameLayer):BDObject(pGameLayer)
{
	m_iCurState = STATE_NULL;
	m_iPrevState = STATE_NULL;
	m_lpGameWorld = NULL;
	m_lpCurAction = NULL;
	//m_lpMovementComp = NULL;
	m_lpGameLayer = pGameLayer;
	m_iGroup = -1;
	m_bIsMainCharacter = NULL;
	m_iType = OBJECT_CHARACTER;
}

BDCharacter::~BDCharacter()
{
   m_lpGameWorld = NULL;
   CC_SAFE_DELETE(m_lpCurAction);

   //destruction of lpMovementComp will be handled by ~CCNode 
}


BDCharacter* BDCharacter::Create()
{
	BDCharacter* pRet = NULL;
	extension::CCArmature* pArmature = NULL;

	pRet = new BDCharacter();
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

BDCharacter* BDCharacter::CreateWithGameLayer(BDGameLayer* pGameLayer)
{
	BDCharacter* pRet = NULL;
	extension::CCArmature* pArmature = NULL;

	pRet = new BDCharacter(pGameLayer);
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

void BDCharacter::PlaySkill(int iSkillType)
{
	jschar sSkillName[128];
	jsval jv=JSVAL_VOID;
	jsval retval = JSVAL_NULL;
	JSString* jstr = NULL;
	bool res = false;
	jsval v[1];
	
	
	switch (iSkillType)
	{
	case BDSKILL_JUMP:
		wcscpy(sSkillName,L"Jump");
		jstr=JS_NewUCStringCopyZ(ScriptingCore::getInstance()->getGlobalContext(),sSkillName); 
		jv = STRING_TO_JSVAL(jstr);
		v[0] = jv;

		ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(this->GetJSObject()),
			"playSkill", 1, v, &retval);
		res = JSVAL_TO_BOOLEAN(retval); 
		break;
	}
}

void BDCharacter::SetPosition(const CCPoint &position,bool bMoveArmature)
{
	CCNode::setPosition(position); 

	if(bMoveArmature)
		m_lpArmature->setPosition(position);
}

void BDCharacter::SetPosition(float x, float y,bool bMoveArmature)
{
	CCNode::setPosition(x,y);

	if(bMoveArmature)
		m_lpArmature->setPosition(x,y);
}

void BDCharacter::SetPositionX(float x,bool bMoveArmature)
{
	CCNode::setPositionX(x);

	if(bMoveArmature)
		m_lpArmature->setPositionX(x);
}

void BDCharacter::SetPositionY(float y,bool bMoveArmature)
{
	CCNode::setPositionY(y);

	if(bMoveArmature)
		m_lpArmature->setPositionY(y);
}


void BDCharacter::SetGameWorld(b2World* world)
{
	m_lpGameWorld = world;
}

void BDCharacter::GrowBody()
{
	// Define the dynamic body.
	//Set up a 1m squared box in the physics world
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	b2Body *body = m_lpGameWorld->CreateBody(&bodyDef); 

	////// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);//These are mid points for our 1m box

	////// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	////fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);


	////bullet->setB2Body(body);
	////bullet->setPTMRatio(PT_RATIO);
	////bullet->setPosition( ccp( -100, -100) );

	body = m_lpGameWorld->CreateBody(&bodyDef);
	m_lpArmature->setBody(body);
}

void BDCharacter::PlayAnimation(char* anim)
{
	m_lpArmature->getAnimation()->play(anim); 
}

void BDCharacter::BuildArmature(const char* srcFile,const char* armatureName)
{
	CCLog("about to add json file to armatureDataManager:%s ",srcFile);
	extension::CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(srcFile);//add the armatrues in the data to cache
	m_lpArmature = BDArmature::create(armatureName,this);//create armatrue from cache
}

BDArmature* BDCharacter::GetArmature()
{
	return m_lpArmature;
}

void BDCharacter::SetArmature(BDArmature* pArmature)
{
	m_lpArmature = pArmature;
}

void BDCharacter::SetGroup(int group)
{
	m_iGroup = group;

	//Set filter for each of the character's bone
	CCObject *boneElement = NULL;
	CCObject *collBody = NULL;
	CCArray *pBoneArray = NULL;  
	CCArray *pColliderBodyArray = NULL;
	pBoneArray = m_lpArmature->getChildren();
	CCARRAY_FOREACH(pBoneArray, boneElement)
	{
		if (extension::CCBone *bone = dynamic_cast<extension::CCBone *>(boneElement))
		{
			CCArray *displayList = bone->getDisplayManager()->getDecorativeDisplayList();

			CCObject *displayObject = NULL;
			CCARRAY_FOREACH(displayList, displayObject)
			{
				extension::CCColliderDetector *detector = ((extension::CCDecorativeDisplay *)displayObject)->getColliderDetector();
			
				if (detector != NULL)
				{
					pColliderBodyArray = detector->getColliderBodyList();
					CCARRAY_FOREACH(pColliderBodyArray, collBody)
					{
						extension::ColliderBody *colliderBody = (extension::ColliderBody *)collBody;

						b2Fixture* fixture = colliderBody->getB2Fixture();
						if (fixture == NULL)
						{
							return;
						}

						b2Filter filter;
					
						switch (m_iGroup)
						{
						case GROUP_BABE:
							filter.categoryBits = BDObject::GROUP_CATEGORY_BABE;
							filter.maskBits =  BDObject::GROUP_CATEGORY_ENEMY;
							fixture->SetFilterData(filter);
							break;
						case GROUP_ENEMY:
							filter.categoryBits = BDObject::GROUP_CATEGORY_ENEMY;
							filter.maskBits = BDObject::GROUP_CATEGORY_BABE;
							fixture->SetFilterData(filter);
							break;
						case GROUP_ITEM:
							filter.categoryBits = BDObject::GROUP_CATEGORY_ITEM;
							filter.maskBits = BDObject::GROUP_MASK_ITEM;
							fixture->SetFilterData(filter);
							break;
						}
					}
				}
			}
		}
	}
}

int BDCharacter::GetGroup()
{
	return m_iGroup;
}

void BDCharacter::StopCurBDAction()
{
	if(m_lpCurAction)
		m_lpCurAction->Stop();
	delete m_lpCurAction;
}

void BDCharacter::PlayParticle(const char *plistFile,int iPositionType)
{
	if(m_lpEmitter==NULL)
	{
		if(IsMainCharacter())
			m_lpEmitter = BDParticleSystem::create(plistFile,true);
		else
			m_lpEmitter = BDParticleSystem::create(plistFile,false);
		addChild(m_lpEmitter, 10);
		m_lpEmitter->setPosition(0.0f,0.0f);
		m_lpEmitter->setPositionType((tCCPositionType)iPositionType);
	}
}

void BDCharacter::visit()
{
	CCNode::visit();
	m_lpArmature->visit();
}

int BDCharacter::GetCurState()
{
	return m_iCurState;
}

void BDCharacter::SetBDJumpAction(BDCharacter* pHost,float fAcc)
{
	BDAction* pAct = new BDJumpAction(this,fAcc);
	this->SetCurBDAction(pAct);
}

void BDCharacter::SetCurState(int state)
{
	if(state == m_iCurState)
		return;

	m_iCurState = state;

	switch(m_iCurState)
	{
	case STATE_IDLE:
		PlayAnimation("FireWithoutBullet");
		break;
	case STATE_WALKING:
		PlayAnimation("Walk");
		break;
	}
	
}

int BDCharacter::GetPrevState()
{
	return m_iPrevState;
}

void BDCharacter::SetPrevState(int state)
{
	m_iPrevState = state;
}

bool BDCharacter::IsMainCharacter()
{
	return m_bIsMainCharacter;
}

void BDCharacter::SetIsMainCharacter(bool isMainCharacter)
{
	m_bIsMainCharacter = isMainCharacter;
}


void BDCharacter::update(float dt)
{
	CCNode::update(dt);

	m_lpArmature->update(dt);

	if(m_lpCurAction)
		m_lpCurAction->Update(dt);

	if(m_lpEmitter)
	{
		if(m_lpEmitter->IsFinished())
		{
			removeChild(m_lpEmitter,false);
			m_lpEmitter->unscheduleUpdate();
			m_lpEmitter = NULL;
		}
	}
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

JSBool js_bdcharacter_getGroup(JSContext *cx, uint32_t argc, jsval *vp)
{
	if(argc != 0)
	{
		JS_ReportError(cx, "js_bdcharacter_playSkill: wrong number of argument");
		return JS_FALSE;
	}

	jsval jsret;
	JSBool ok = JS_TRUE;
	JSObject *obj = NULL;
	BDCharacter* cobj = NULL;

	obj = JS_THIS_OBJECT(cx, vp); 
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	cobj = (BDCharacter*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");   

	int group = cobj->GetGroup();
	
	jsret = INT_TO_JSVAL(group);
	JS_SET_RVAL(cx, vp, jsret);

	return JS_TRUE;
}

JSBool js_bdcharacter_isMainCharacter(JSContext *cx, uint32_t argc, jsval *vp)
{
	if(argc != 0)
	{
		JS_ReportError(cx, "js_bdcharacter_playSkill: wrong number of argument");
		return JS_FALSE;
	}

	jsval jsret;
	JSBool ok = JS_TRUE;
	JSObject *obj = NULL;
	BDCharacter* cobj = NULL;

	obj = JS_THIS_OBJECT(cx, vp); 
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	cobj = (BDCharacter*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");   

	bool isMainChar = cobj->IsMainCharacter();

	jsret = BOOLEAN_TO_JSVAL(isMainChar);
	JS_SET_RVAL(cx, vp, jsret);

	return JS_TRUE;
}


JSBool js_bdcharacter_playParticle(JSContext *cx, uint32_t argc, jsval *vp)
{
	if(argc != 2)
	{
		JS_ReportError(cx, "js_bdcharacter_playSkill: wrong number of argument");
		return JS_FALSE;
	}

	jsval jsret;
	jsval *argv = JS_ARGV(cx, vp);   
	JSBool ok = JS_TRUE;

	JSObject *obj = NULL;
	BDCharacter* cobj = NULL;
	obj = JS_THIS_OBJECT(cx, vp); 
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	cobj = (BDCharacter*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");   

	if( !JSVAL_IS_STRING(argv[0]) || !JSVAL_IS_INT(argv[1]))
	{
		JS_ReportError(cx, "js_bdcharacter_playSkill:wrong type of argument");
		jsret = INT_TO_JSVAL(0);
		JS_SET_RVAL(cx, vp, jsret);
		return JS_FALSE;
	}

	std::string pListFile;
	int iPositionType;
	JSString* jsstr = NULL;
	size_t len = 0;
	jsstr=JSVAL_TO_STRING(argv[0]);
	iPositionType = JSVAL_TO_INT(argv[1]);
	const jschar* p = JS_GetStringCharsAndLength(cx,jsstr,&len);
	WCharArrToString((unsigned short*)p,pListFile);
	cobj->PlayParticle(pListFile.c_str(),iPositionType);
	
	jsret = INT_TO_JSVAL(1);
	JS_SET_RVAL(cx, vp, jsret);//return true to JS code if the particle is played

	return JS_TRUE;
}

JSBool js_bdcharacter_setJumpAction(JSContext *cx, uint32_t argc, jsval *vp)
{
	if(argc != 1)
	{
		JS_ReportError(cx, "js_bdcharacter_setJumpAction: wrong number of argument");
		return JS_FALSE;
	}

	jsval jsret;
	jsval *argv = JS_ARGV(cx, vp);   
	JSBool ok = JS_TRUE;

	JSObject *obj = NULL;
	BDCharacter* cobj = NULL;
	obj = JS_THIS_OBJECT(cx, vp); 
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	cobj = (BDCharacter*)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");   


	float fAcc = 0.0f;

	if(!JSVAL_IS_VOID(argv[0]))
	{
		if(JSVAL_IS_DOUBLE(argv[0]))
		{
			fAcc = (float)(JSVAL_TO_DOUBLE(argv[0]));
		}
		else
		{
			fAcc = (float)(JSVAL_TO_INT(argv[0]));
		}
	}

	cobj->SetBDJumpAction(cobj,fAcc);

	jsret = INT_TO_JSVAL(1);
	JS_SET_RVAL(cx, vp, jsret);//return true to JS if the action is set

	return JS_TRUE;
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
		JS_FN("getGroup", js_bdcharacter_getGroup, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("isMainCharacter", js_bdcharacter_isMainCharacter, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("playParticle", js_bdcharacter_playParticle, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("setJumpAction", js_bdcharacter_setJumpAction, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
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