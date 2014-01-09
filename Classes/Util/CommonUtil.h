#ifndef __UTIL_H__
#define __UTIL_H__

#include "cocos2d.h"

void WCharArrToString(unsigned short* src, std::string& str);

int GetWCharArrLen(unsigned short* src);

float SpeedLogicToScreen(float logicX,float refX);

#endif