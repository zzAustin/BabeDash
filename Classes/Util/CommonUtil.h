#ifndef __UTIL_H__
#define __UTIL_H__

#include "cocos2d.h"
#include "jsapi.h"

std::string& GetStrJSVal(const char* field, JSObject* defObj, JSContext* cx);

float* GetDoubleArrayJSVal(const char* field, JSObject* defObj, JSContext* cx, const int size);

void WCharArrToString(unsigned short* src, std::string& str);

int GetWCharArrLen(unsigned short* src);

float SpeedLogicToScreen(float logicX,float refX);

#endif