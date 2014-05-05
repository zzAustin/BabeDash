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

class BDGameLayer;

extern JSClass  *jsb_BDCharacter_class;
extern JSObject *jsb_BDCharacter_prototype;
extern JSObject* jsb_CCArmature_prototype;

class BDCharacter;
//native class
//----------------------------------------------------------------------------------------------
class BDArmature:public cocos2d::extension::CCArmature
{
public:
	BDArmature::BDArmature();
	BDArmature::BDArmature(BDObject* pOwner);
	static BDArmature *create(const char *name,BDCharacter* pCharacter);
	BDObject *GetOwner();
protected:
	BDObject* m_lpOwner;
};

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
	BDCharacter(BDGameLayer* pGameLayer);
	static BDCharacter* CreateWithGameLayer(BDGameLayer* pGameLayer);
	~BDCharacter();
	static BDCharacter* Create();

	//override the CCNode setPosition
	void SetPosition(const CCPoint &position,bool bMoveArmature);
	void SetPosition(float x, float y,bool bMoveArmature);
	void SetPositionX(float x,bool bMoveArmature);
	void SetPositionY(float y,bool bMoveArmature);

	void PlaySkill(int iSkillType);
	void PlayParticle(const char *plistFile,int iPositionType);
	void PlayAnimation(char* anim);
	BDArmature* GetArmature();
	void BuildArmature(const char* srcFile,const char* armatureName);
	void SetArmature(BDArmature* pArmature);

	virtual void visit(void);
	virtual void update(float dt);
	virtual void GrowBody();
	void SetGameWorld(b2World* world);
	int GetCurState();
	void SetCurState(int state);
	int GetPrevState();
	void SetPrevState(int state);

	virtual void SetBDJumpAction(BDCharacter* pHost,float fAcc);
	void StopCurBDAction();
	void SetGroup(int group);
	int GetGroup();
	bool IsMainCharacter();
	void SetIsMainCharacter(bool isMainCharacter);
	

protected:
	BDArmature* m_lpArmature;
	b2World* m_lpGameWorld;
	int m_iCurState;
	int m_iPrevState;
	int m_iGroup;
	bool m_bIsMainCharacter;
};

//JSBinding
//---------------------------------------------------------------------------------------------
JSBool js_bdcharacter_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_bdcharacter_finalize(JSContext *cx, JSObject *obj);
void js_register_bdcharacter(JSContext *cx, JSObject *global);
void register_all_bdcharacter(JSContext* cx, JSObject* obj);
//JSBool js_bdgamelayer_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdcharacter_sayHello(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdcharacter_getGroup(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdcharacter_isMainCharacter(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdcharacter_playParticle(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_bdcharacter_setJumpAction(JSContext *cx, uint32_t argc, jsval *vp);

#endif
