#ifndef _BD_MOVE_ACTION_
#define _BD_MOVE_ACTION_

#include "Actions/BDAction.h"

class BDMoveAction:public BDAction
{
public:

	BDMoveAction(BDObject* host);
	virtual ~BDMoveAction();
	virtual void Update(float delta);
};

#endif