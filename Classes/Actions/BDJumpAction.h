#ifndef _BD_JUMP_ACTION_
#define _BD_JUMP_ACTION_

#include "Actions/BDAction.h"

//action
class BDJumpAction:public BDAction
{
public:

	BDJumpAction(BDObject* pHost);
	BDJumpAction(BDObject* pHost,float fAcc);
	virtual ~BDJumpAction();
	virtual void Update(float delta);
	int GetType();

    float m_fAccVert;
};

#endif