#ifndef __BABEJS_H__
#define __BABEJS_H__

#include "jsapi.h"
#include "jsfriendapi.h"

extern JSClass  *jsb_Babe_class;
extern JSObject *jsb_Babe_prototype;

JSBool js_cocos2dx_Babe_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_babe_finalize(JSContext *cx, JSObject *obj);
void js_register_babe(JSContext *cx, JSObject *global);
void register_all_babedash(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_Babe_sayHello(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_Babe_retain(JSContext* cx,uint32_t argc,jsval *vp);

#endif