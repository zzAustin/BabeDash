#include "BDBaseClasses/BDObject.h"
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
}

JSObject* BDObject::GetJSObject()
{
	return m_lpJsObj;
}


void BDObject::SetJSObject(JSObject* jsObj)
{
	m_lpJsObj = jsObj;
}

void BDObject::SetCurBDAction(BDAction* pAction)
{
	m_lpCurAction = pAction;
}

BDAction* BDObject::GetCurBDAction()
{
	return m_lpCurAction;
}

int BDObject::GetType()
{
	return -1;
}

CCPoint& BDObject::GetSpeed()
{
	return m_ptSpeed;
}

void BDObject::SetSpeed(cocos2d::CCPoint& ptSpeed)
{
	m_ptSpeed = ptSpeed;
}