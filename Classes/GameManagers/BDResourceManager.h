#ifndef _BD_RESOURCE_MANAGER_
#define _BD_RESOURCE_MANAGER_

#include "cocos2d.h"
#include "jsapi.h"
#include "jsfriendapi.h"
#include "Particles/BDParticleSystem.h"


JSBool js_babedash_constructParticleDefs(JSContext* cx,uint32_t argc,jsval *vp);

class BDResourceManager:public cocos2d::CCObject
{
public:
	static BDResourceManager* GetInstance();
	cocos2d::CCDictionary* GetParticleDefs();
	void SetParticleDefs(cocos2d::CCDictionary* pDict);
	BDParticleSystemDef* ParticleDefForKey(std::string plistFile);
	void Initialize();
private:
	BDResourceManager();
	static BDResourceManager* m_lpInstance;
	cocos2d::CCDictionary* m_dictParticleDefs;
};
#endif