#include "JSBinding.h"
//#include "AppMacros.h"
USING_NS_CC;


CCScene* AustinJSB::AustinJSBinding::scene()
{
	return NULL;
}

bool AustinJSB::AustinJSBinding::init()
{
	bool bRef = false;
	do{
		cocos2d::CCLog("JSB init...");
		bRef = true;
	}while(0);

	return bRef;
}


void AustinJSB::AustinJSBinding::functionTest()
{
	cocos2d::CCLog("Function test...");

	//perform a call back of script function testJSB.callback
	js_proxy_t* p = jsb_get_native_proxy(this);
	jsval retval;
	jsval v[] = {
		v[0] = UINT_TO_JSVAL(32),
		v[1] = UINT_TO_JSVAL(88)
	};

	ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(p->obj),
		"callback", 2, v, &retval);
}