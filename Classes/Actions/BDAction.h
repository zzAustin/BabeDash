#ifndef _BD_ACTION_
#define _BD_ACTION_

#include "BDBaseClasses/BDObject.h"

class BDAction
{
public:
	enum BDACTION_TYPE
	{
		BDACTION_BASE,
		BDACTION_STAY_IDLE,
		BDACTION_WALK
	};

	BDAction();
	virtual ~BDAction();
	virtual void Update(float delta);
	virtual void OnActionDone();
	void SetHost(BDObject* pHost);
	BDObject* GetHost();
	void Stop();
	int GetType();
protected:
	BDObject* m_lpHost;
	int m_iType;
};

#endif