#include "CBabe.h"

USING_NS_CC;



void Babe::SayHello()
{
	cocos2d::CCLog("BabeTest...");

	//perform a call back of script function testJSB.callback
	js_proxy_t* p = jsb_get_native_proxy(this);
	jsval retval;
	jsval v[] = {
		v[0] = UINT_TO_JSVAL(32),
		v[1] = UINT_TO_JSVAL(88)
	};

	int res;
	ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(p->obj),
		"callback", 2, v, &retval);
	res = JSVAL_TO_INT(retval);
	CCLog("the callback returns:%d",res);

	int ok = ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(ScriptingCore::getInstance()->getGlobalObject()),
		"callBabe", 0, v, &retval);
	res = JSVAL_TO_INT(retval);
	CCLog("the callbabe returns:%d",res);
}