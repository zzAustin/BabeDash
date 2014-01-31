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
		BDACTION_WALK,
		BDACTION_JUMP
	};

	enum BDAction_STATUS
	{
		BDACTION_READY,
		BDACTION_STARTED,
		BDACTION_FINISHED
	};

	BDAction();
	virtual ~BDAction();
	virtual void Update(float delta);
	virtual void OnActionDone();
	void SetHost(BDObject* pHost);
	BDObject* GetHost();
	void Stop();
	int GetType();
	bool IsRevocable();
	void SetRevocable(bool bRevocable);
protected:
	BDObject* m_lpHost;
	int m_iType;
	int m_iStatus;
	bool m_bRevocable;
};

#endif