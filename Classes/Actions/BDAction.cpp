#include "Actions/BDAction.h"

USING_NS_CC;


BDAction::BDAction()
{
	m_lpHost = NULL;
	m_iType = BDACTION_BASE;
}

BDAction::~BDAction()
{
	if(!m_lpHost)
		m_lpHost = NULL;
}

void BDAction::Update(float delta)
{

}

void BDAction::OnActionDone() 
{
	CCLog("OnActionDone:action id:%d",GetType());
	m_lpHost->SetCurBDAction(NULL);
}

void BDAction::SetHost(BDObject* pHost)
{
	m_lpHost = pHost;
}

BDObject* BDAction::GetHost()
{
	return m_lpHost;
}

void BDAction::Stop()
{
	m_lpHost = NULL;
}

int BDAction::GetType()
{
	return m_iType;
}