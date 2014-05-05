#include "Components/BDMovementComp.h"
#include "Util/CommonUtil.h"
#include "BDBaseClasses/BDObject.h"
#include "Layers/BDGameLayer.h"
USING_NS_CC;

BDMovementComp::BDMovementComp()
{
	m_ptOrigSpeed.x = 0.0f;
	m_ptOrigSpeed.y = 0.0f;
	m_ptCurSpeed.x = 0.0f;
	m_ptCurSpeed.y = 0.0f;
	m_ptOrigAcc.x = 0.0f;
	m_ptOrigAcc.y = 0.0f;
	m_ptCurAcc.x = 0.0f;
	m_ptCurAcc.y = 0.0f;
	m_strName = "BDMovementComp";
}

BDMovementComp::BDMovementComp(cocos2d::CCPoint& ptSpeed)
{
	m_ptOrigSpeed = ptSpeed; 
	m_ptCurSpeed = ptSpeed;
	m_ptCurSpeed.y = 0;//to be altered
	m_strName = "BDMovementComp";
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

CCPoint& BDMovementComp::GetCurSpeed()
{
	return m_ptCurSpeed;
}

CCPoint& BDMovementComp::GetOrigSpeed()
{
	return m_ptOrigSpeed;;
}

CCPoint& BDMovementComp::GetCurAcc()
{
	return m_ptCurAcc;
}

float BDMovementComp::GetCurSpeedX()
{
	return m_ptCurSpeed.x;
}

float BDMovementComp::GetCurSpeedY()
{
	return m_ptCurSpeed.y;
}

float BDMovementComp::GetOrigSpeedX()
{
	return m_ptOrigSpeed.x;
}

float BDMovementComp::GetOrigSpeedY()
{
	return m_ptOrigSpeed.y;
}

float BDMovementComp::GetCurAccX()
{
	return m_ptCurAcc.x;
}

float BDMovementComp::GetCurAccY() 
{
	return m_ptCurAcc.y;
}

void BDMovementComp::SetCurSpeed(const CCPoint& ptSpeed)
{
	m_ptCurSpeed = ptSpeed;
}

void BDMovementComp::SetCurAcc(const CCPoint& ptAcc)
{
	m_ptCurAcc = ptAcc;
}

void BDMovementComp::SetCurSpeedX(float fX)
{
	m_ptCurSpeed.x = fX;
}

void BDMovementComp::SetCurSpeedY(float fY)
{
	m_ptCurSpeed.y = fY;
}

void BDMovementComp::SetCurAccX(float fX)
{
	m_ptCurAcc.x = fX;
}

void BDMovementComp::SetCurAccY(float fY)
{
	m_ptCurAcc.y = fY; 
}



void BDMovementComp::update(float delta)
{
	//update the position of the owner
   CCPoint pos = m_pOwner->getPosition();
   BDObject* pOwner = dynamic_cast<BDObject*>(m_pOwner);

   if(pOwner == NULL) 
	   return;

   m_ptCurSpeed.x += m_ptCurAcc.x;
   m_ptCurSpeed.y += m_ptCurAcc.y;

   float speedX = SpeedLogicToScreen(m_ptCurSpeed.x,pOwner->GetGameLayer()->GetMainCharacter()->GetSpeed().x);

   BDCharacter* pCharacter = dynamic_cast<BDCharacter*>(m_pOwner);

   //for main character we move only the container object horizontally but not the armature
   if(pCharacter!=NULL && pCharacter->GetGroup() == BDObject::GROUP_BABE)
   {
	   pos.x += speedX;
	   pos.y += m_ptCurSpeed.y;
	   pCharacter->SetPosition(pos.x + speedX,pos.y,true);

	   CCPoint lgcPos = pCharacter->GetLogicPosition();
	   lgcPos.x += m_ptCurSpeed.x; 
	   lgcPos.y += m_ptCurSpeed.y; 
	   
	   pCharacter->SetLogicPosition(lgcPos);
   }
   else if(pCharacter!=NULL)
   {
		pos.x += speedX;
		pos.y += m_ptCurSpeed.y;
		pCharacter->SetPosition(pos.x,pos.y,true);

		CCPoint lgcPos = pCharacter->GetLogicPosition();
		lgcPos.x += m_ptCurSpeed.x; 
		lgcPos.y += m_ptCurSpeed.y; 
		pCharacter->SetLogicPosition(lgcPos);
   }
   //else
   //{
	  // pos.x += speedX;
	  // pos.y += m_ptCurSpeed.y;
	  // pOwner->setPosition(pos);
   //}
}

void BDMovementComp::CmdRight()
{
	m_ptCurSpeed.x = abs(m_ptCurSpeed.x);
}

void BDMovementComp::CmdLeft()
{
	m_ptCurSpeed.x = 0 - abs(m_ptCurSpeed.x);
}

void BDMovementComp::CmdUp()
{
	m_ptCurSpeed.y = abs(m_ptCurSpeed.y);
}

void BDMovementComp::CmdDown()
{
	m_ptCurSpeed.y = 0 - abs(m_ptCurSpeed.y);
}

void BDMovementComp::StopHori()
{
	m_ptCurSpeed.x = 0.0f;
	m_ptCurAcc.x = 0.0f;
}

void BDMovementComp::StopVert()
{
	m_ptCurSpeed.y = 0.0f;
	m_ptCurAcc.y = 0.0f;
}