#ifndef _BD_MOVEMENT_COMP_
#define _BD_MOVEMENT_COMP_

#include "cocos2d.h"

class BDMovementComp:public cocos2d::CCComponent
{
public:
	BDMovementComp();
	BDMovementComp(cocos2d::CCPoint& ptSpeed);
	static  BDMovementComp *Create();
	static  BDMovementComp *Create(cocos2d::CCPoint& ptSpeed);
	void onEnter();
	void onExit();
    void update(float delta);

private:
	cocos2d::CCPoint m_ptSpeed;
};

#endif