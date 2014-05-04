#ifndef _BD_GAME_LAYER_
#define _BD_GAME_LAYER_

#include "BDBaseClasses/BDObject.h"
#include "Defs/DBDef.h"
#include "physics_nodes/CCPhysicsSprite.h"
#include "CocoStudio/Armature/CCBone.h"
#include "Objects/BDCharacter.h"
#include "CocoStudio/Armature/animation/CCArmatureAnimation.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f


#define ENABLE_PHYSICS_BOX2D_DETECT 1//BabeDash utilizes box2d for physics
#if ENABLE_PHYSICS_BOX2D_DETECT
#include "Box2DTestBed/GLES-Render.h"
#include "Box2D/Box2D.h"
#elif ENABLE_PHYSICS_CHIPMUNK_DETECT
#include "chipmunk.h"
#endif

class BDGameScene;

extern JSObject *jsb_CCLayer_prototype;//parent class
extern JSClass  *jsb_BDGameLayer_class;
extern JSObject *jsb_BDGameLayer_prototype;

//def class
//----------------------------------------------------------------------------------------------
class BDMovementAttr
{
public:
	CCPoint ptSpeed;
};

class BDGameObjDef:public BDDef
{
public:
	BDGameObjDef();
	~BDGameObjDef();
	std::string obj_type;
	std::string res;
	std::string armature;
	CCPoint pos;
	CCPoint scale;
	CCPoint speed;
	int state;
	int group;
	BDMovementAttr* lpMovementAttr;

	void ApplyFromDefObj(JSContext* cx,JSObject* defObj);

};

//box2d
//------------------------------------------------------------------------------------------------
class Contact
{
public:
	b2Fixture *fixtureA;
	b2Fixture *fixtureB;
};

class ContactListener : public b2ContactListener
{
	//! Callbacks for derived classes.
	virtual void BeginContact(b2Contact *contact);
	virtual void EndContact(b2Contact *contact);
	virtual void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
	virtual void PostSolve(const b2Contact *contact, const b2ContactImpulse *impulse);
public:
	std::list<Contact> contact_list;
};


//native class
//-------------------------------------------------------------------------------------------
class BDGameLayer:public cocos2d::CCLayer,public BDObject
{
public:
	BDGameLayer();
	BDGameLayer(BDGameScene* pScene);
	static BDGameLayer* CreateWithScene(BDGameScene* pScene);//notice this create function is native only,it's the following non-param func that is JS binded
	static BDGameLayer* Create();
	~BDGameLayer();
	BDObject *AddGameObject(JSContext* cx,BDGameObjDef& def);
	virtual void InitWorld();
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	int GetType();
	virtual void update(float delta);
	virtual void draw();
	void SetMainCharacter(BDCharacter* pMainCharacter);
	BDCharacter* GetMainCharacter();
	void CreateBodiesTest() ;
	void CreateBodiesTest2() ;

	void HandleCollision(BDObject* objA,BDObject* objB);
	void onFrameEvent(cocos2d::extension::CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex);
	cocos2d::extension::CCPhysicsSprite* bullet;
	BDCharacter* mainCharacter;
	BDCharacter* enemy;


protected:
	b2World *m_lpGameWorld;
	ContactListener *m_lpContactListener;
	GLESDebugDraw *m_lpDebugDraw;
	BDGameScene* m_lpGameScene; //pointer to the current game scene
	BDCharacter* m_lpMainCharacter;
};


//---------------------------------------------------------------------------------------------
//JS Binding
JSBool js_bdgamelayer_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_bdgamelayer_finalize(JSContext *cx, JSObject *obj);
void js_register_bdgamelayer(JSContext *cx, JSObject *global);
void register_all_bdgamelayer(JSContext* cx, JSObject* obj);
JSBool js_bdgamelayer_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdgamelayer_addGameObject(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdgamelayer_sayHello(JSContext *cx, uint32_t argc, jsval *vp);
#endif