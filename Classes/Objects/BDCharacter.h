#ifndef _BD_CHARACTER_
#define _BD_CHARACTER_

#include "BDBaseClasses/BDObject.h"
#include "cocos-ext.h"
#include "Defs/DBDef.h"
#include "Actions/BDAction.h"

#define ENABLE_PHYSICS_BOX2D_DETECT 1//BabeDash utilizes box2d for physics
#if ENABLE_PHYSICS_BOX2D_DETECT
#include "Box2DTestBed/GLES-Render.h"
#include "Box2D/Box2D.h"
#elif ENABLE_PHYSICS_CHIPMUNK_DETECT
#include "chipmunk.h"
#endif

extern JSClass  *jsb_BDCharacter_class;
extern JSObject *jsb_BDCharacter_prototype;
extern JSObject* jsb_CCArmature_prototype;

//native class
//----------------------------------------------------------------------------------------------
class BDCharacter:public cocos2d::CCNode,public BDObject
{
public:
	enum CHARACTER_STATE
	{
	   STATE_NULL = 0,
	   STATE_IDLE,
	   STATE_WALKING,
	   STATE_NUM
	};

	BDCharacter();
	~BDCharacter();
	static BDCharacter* Create();
	void PlayAnimation(char* anim);
	cocos2d::extension::CCArmature* GetArmature();
	void BuildArmature(const char* srcFile,const char* armatureName);
	void SetArmature(cocos2d::extension::CCArmature* pArmature);
	virtual void visit(void);
	virtual void update(float dt);
	virtual void GrowBody();
	void SetGameWorld(b2World* world);
	int GetCurState();
	void SetCurState(int state);
	int GetPrevState();
	void SetPrevState(int state);
	void StopCurBDAction();
	void SetGroup(int group);
	int GetGroup();

protected:
	cocos2d::extension::CCArmature* m_lpArmature;
	b2World* m_lpGameWorld;
	int m_iCurState;
	int m_iPrevState;
	int m_iGroup;
};

//JSBinding
//---------------------------------------------------------------------------------------------
JSBool js_bdcharacter_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_bdcharacter_finalize(JSContext *cx, JSObject *obj);
void js_register_bdcharacter(JSContext *cx, JSObject *global);
void register_all_bdcharacter(JSContext* cx, JSObject* obj);
//JSBool js_bdgamelayer_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdcharacter_sayHello(JSContext *cx, uint32_t argc, jsval *vp);

#endif
