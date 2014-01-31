#include "BDBaseClasses/BDObject.h"
#include "Actions/BDAction.h"
#include "cocos2d.h"
USING_NS_CC;

int BDObject::GROUP_CATEGORY_BABE = 0x0001;
int BDObject::GROUP_CATEGORY_ENEMY = 0x0002;
int BDObject::GROUP_CATEGORY_ITEM = 0x0008;

int BDObject::GROUP_MASK_BABE = 0x000A;
int BDObject::GROUP_MASK_ENEMY = 0x0001;
int BDObject::GROUP_MASK_ITEM = 0x0001;

BDObject::BDObject()
{
	m_lpJsObj = NULL;
	m_lpMovementComp = NULL;
	m_ptLogicPos.x = 0.0f;
	m_ptLogicPos.y = 0.0f;
	m_lpEmitter = NULL;
	m_iType = OBJECT_BASEOBJ;
}

BDObject::BDObject(BDGameLayer* pGameLayer)
{
	m_lpJsObj = NULL;
	m_lpMovementComp = NULL;
	m_lpGameLayer = pGameLayer;
	m_ptLogicPos.x = 0.0f;
	m_ptLogicPos.y = 0.0f;
	m_lpEmitter = NULL;
}

JSObject* BDObject::GetJSObject()
{
	return m_lpJsObj;
}

void BDObject::SetJSObject(JSObject* jsObj)
{
	m_lpJsObj = jsObj;
}

BDGameLayer* BDObject::GetGameLayer()
{
	return m_lpGameLayer;
}

void BDObject::SetCurBDAction(BDAction* pAction)
{
	if(pAction == NULL)
	{
		m_lpCurAction = pAction;
		return;
	}

	if(m_lpCurAction==NULL)
		m_lpCurAction = pAction;
	else if(m_lpCurAction->IsRevocable())
	{
		m_lpCurAction->OnActionDone();
		CC_SAFE_DELETE(m_lpCurAction);
		m_lpCurAction = pAction;
	}
}

BDAction* BDObject::GetCurBDAction()
{
	return m_lpCurAction;
}

BDMovementComp* BDObject::GetMovementComp()
{
   return m_lpMovementComp;
}

void BDObject::SetMovementComp(BDMovementComp* pMovementComp)
{
	m_lpMovementComp = pMovementComp;
}

int BDObject::GetType()
{
	return m_iType;
}

CCPoint BDObject::GetSpeed()
{
	if(m_lpMovementComp != NULL)
		return m_lpMovementComp->GetCurSpeed();
	else
		return ccp(0.0f,0.0f);
}


void BDObject::SetSpeed(const CCPoint& ptSpeed)
{
	if(m_lpMovementComp != NULL)
		m_lpMovementComp->SetCurSpeed(ptSpeed);
}

void BDObject::SetLogicPosition(const CCPoint &position)
{
	m_ptLogicPos = position;
}

void BDObject::SetLogicPosition(float x, float y)
{
	m_ptLogicPos.x = x;
	m_ptLogicPos.y = y;
}

void BDObject::SetLogicPositionX(float x)
{
	m_ptLogicPos.x = x;
}

void BDObject::SetLogicPositionY(float y)
{
	m_ptLogicPos.y = y;
}

CCPoint& BDObject::GetLogicPosition()
{
	return m_ptLogicPos;
}

float BDObject::GetLogicPositionX()
{
	return m_ptLogicPos.x;
}

float BDObject::GetLogicPositionY()
{
	return m_ptLogicPos.y;
}

BDParticleSystem* BDObject::GetEmitter()
{
	return m_lpEmitter;
}

void BDObject::SetEmitter(BDParticleSystem* pEmitter)
{
	m_lpEmitter = pEmitter;
}

void BDObject::PlayParticle(const char *plistFile,int iPositionType)
{

}