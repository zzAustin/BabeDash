#ifndef __JSBinding_H__
#define __JSBinding_H__

#include "cocos2d.h"
#include "../../../../scripting/javascript/bindings/ScriptingCore.h"

namespace AustinJSB
{
	class AustinJSBinding: public cocos2d::CCObject
	{
	public:
		static cocos2d::CCScene* scene();

		virtual bool init();
		CREATE_FUNC(AustinJSBinding);
		void functionTest();
	
	};

}


#endif // __JSBinding_H__
