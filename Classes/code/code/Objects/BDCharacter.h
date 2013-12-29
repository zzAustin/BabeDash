#ifndef _BD_CHARACTER_
#define _BD_CHARACTER_

#include "BDBaseClasses/BDObject.h"
#include "../../../extensions/cocos-ext.h"


#define ENABLE_PHYSICS_BOX2D_DETECT 1//BabeDash utilizes box2d for physics
#if ENABLE_PHYSICS_BOX2D_DETECT
#include "Box2DTestBed/GLES-Render.h"
#include "Box2D/Box2D.h"
#elif ENABLE_PHYSICS_CHIPMUNK_DETECT
#include "chipmunk.h"
#endif

class BDCharacter:public BDObject,public cocos2d::extension::CCArmature
{
public:
	static cocos2d::extension::CCArmature* CreateArmature(char* srcFile,char* armatureName);
	void PlayAnimation(char* anim);
};


#endif
