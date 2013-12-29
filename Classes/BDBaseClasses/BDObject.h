#ifndef _BD_OBJECT_
#define _BD_OBJECT_

#include "cocos2d.h"
#include "jsapi.h"
#include "jsfriendapi.h"

class BDObject
{
public:
	BDObject();
	JSObject* GetJSObject();
	void SetJSObject(JSObject* jsObj);
protected:
	JSObject* lpJsObj;//the js side object
};
#endif