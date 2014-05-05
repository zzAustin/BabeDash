#include "Actions/BDMoveAction.h"
#include "Objects/BDCharacter.h"


BDMoveAction::BDMoveAction(BDObject* host)
{
	m_lpHost = host;
}

BDMoveAction::~BDMoveAction()
{

}


void BDMoveAction::Update(float delta)
{
	/*BDCharacter* pCharacter = NULL;
	float oldPosX;
	float newPosX;

	if(!m_lpHost)
	return;

	pCharacter = dynamic_cast<BDCharacter*>(m_lpHost);

	if(!pCharacter)
	return;

	oldPosX = pCharacter->GetArmature()->getPositionX();
	newPosX = oldPosX + pCharacter->GetSpeed().x;
	pCharacter->GetArmature()->setPositionX(newPosX);

	if(newPosX > 400.0f)
	{
	OnActionDone();
	}*/
}