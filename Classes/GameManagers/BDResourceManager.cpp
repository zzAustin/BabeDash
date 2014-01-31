#include "GameManagers/BDResourceManager.h"
#include "ScriptingCore.h"
#include "Util/CommonUtil.h"
USING_NS_CC;

BDResourceManager* BDResourceManager::m_lpInstance = NULL;

BDResourceManager::BDResourceManager()
{

}


BDResourceManager* BDResourceManager::GetInstance()
{
	if(m_lpInstance == NULL)
	{
		m_lpInstance =  new BDResourceManager();
		return m_lpInstance;
	}
	else
		return m_lpInstance;
}

void BDResourceManager::SetParticleDefs(cocos2d::CCDictionary* pDict)
{
	m_dictParticleDefs = pDict;
}

void BDResourceManager::Initialize()
{
	JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
	JSObject* obj = ScriptingCore::getInstance()->getGlobalObject();
	JS_DefineFunction(cx, obj, "constructParticleDefs", js_babedash_constructParticleDefs, 0, JSPROP_READONLY | JSPROP_PERMANENT);
}


JSBool js_babedash_constructParticleDefs(JSContext* cx,uint32_t argc,jsval *vp)
{
	CCLog("--js_babedash_constructParticleDefs-- constructing particle defs");

	jsval *argv = JS_ARGV(cx, vp);  
	jsval val = JSVAL_NULL;

	JSObject* defsObj = NULL;
	defsObj = JSVAL_TO_OBJECT(argv[0]); 

	if(defsObj == NULL)
	{
		CCLog("particle defs obj is NULL!");
		return JS_FALSE;
	}

	std::string plistFile;
	BDResourceManager::GetInstance()->SetParticleDefs(new CCDictionary());

	for(int i = 0;;i++)
	{
		JS_GetElement(cx,defsObj,i,&val);
		if(!JSVAL_IS_VOID(val))
		{ 
			JSString* jsstr;
			if(JSVAL_IS_STRING(val))
			{
				jsstr=JSVAL_TO_STRING(val);
			}
			else
			{
				jsstr=JS_ValueToString(cx,val);
			}
			size_t len = 0;
			const jschar* p = JS_GetStringCharsAndLength(cx,jsstr,&len);
			WCharArrToString((unsigned short*)p,plistFile);

			BDParticleSystemDef* particleDef = new BDParticleSystemDef(); 
			particleDef->initWithFile(plistFile.c_str());
			BDResourceManager::GetInstance()->GetParticleDefs()->setObject(particleDef,plistFile);  
		} 
		else
			break; 
	}

	return JS_TRUE;  
}

BDParticleSystemDef* BDResourceManager::ParticleDefForKey(std::string plistFile)
{
	return (BDParticleSystemDef*)m_dictParticleDefs->objectForKey(plistFile); 
}

CCDictionary* BDResourceManager::GetParticleDefs() 
{
	return m_dictParticleDefs;
}

 