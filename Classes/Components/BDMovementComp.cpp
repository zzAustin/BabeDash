#include "Components/BDMovementComp.h"
#include "BDBaseClasses/BDObject.h"
USING_NS_CC;

BDMovementComp::BDMovementComp()
{
	m_ptSpeed.x = 0.0f;
	m_ptSpeed.y = 0.0f;
}

BDMovementComp::BDMovementComp(cocos2d::CCPoint& ptSpeed)
{
	m_ptSpeed = ptSpeed;
}


BDMovementComp *BDMovementComp::Create()
{
	BDMovementComp* pRet = new BDMovementComp();

	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}

	return pRet;
}

BDMovementComp *BDMovementComp::Create(cocos2d::CCPoint& ptSpeed)
{
	BDMovementComp* pRet = new BDMovementComp(ptSpeed);

	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}

	return pRet;
}

void BDMovementComp::onEnter()
{

}

void BDMovementComp::onExit()
{

}

void BDMovementComp::update(float delta)
{
	BDObject* bdObj = dynamic_cast<BDObject*>(m_pOwner);
	CCPoint speed;
	if(bdObj == NULL)
		return;

	speed = bdObj->GetSpeed();

	if(speed.x != 0 && speed.y != 0)
	{
		
	}
}