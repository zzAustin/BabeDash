#include "Actions/BDAction.h"

USING_NS_CC;


BDAction::BDAction()
{
	m_lpHost = NULL;
	m_iType = BDACTION_BASE;
	m_iStatus = BDACTION_READY;
	m_bRevocable = false;
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

bool BDAction::IsRevocable()
{
	return m_bRevocable;
}

void BDAction::SetRevocable(bool bRevocable)
{
	m_bRevocable = bRevocable;
}

void BDAction::Stop()
{
	m_lpHost = NULL;
}

int BDAction::GetType()
{
	return m_iType;
}