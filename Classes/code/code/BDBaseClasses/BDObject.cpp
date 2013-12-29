#include "BDBaseClasses/BDObject.h"

BDObject::BDObject()
{
	lpJsObj = NULL;
}

JSObject* BDObject::GetJSObject()
{
	return lpJsObj;
}


void BDObject::SetJSObject(JSObject* jsObj)
{
	lpJsObj = jsObj;
}