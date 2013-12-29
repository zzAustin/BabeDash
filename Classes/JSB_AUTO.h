#ifndef __JSBAUTO_H__
#define __JSBAUTO_H__

#include "jsapi.h"
#include "jsfriendapi.h"
#include "JSBinding.h"

void register_all(JSContext* cx, JSObject* obj);
void js_register(JSContext* cx,JSObject* global);


#endif // __JSBAUTO_H__
