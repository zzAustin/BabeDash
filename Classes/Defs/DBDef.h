#ifndef _BD_DEF_
#define _BD_DEF_

#include "cocos2d_specifics.hpp"

class BDDef
{
public:
	virtual void ApplyFromDefObj(JSContext* cx,JSObject* defObj) = 0;
};

#endif