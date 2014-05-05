#include "Actions/BDJumpAction.h"
#include "Objects/BDCharacter.h"
#include "BDBaseClasses/BDObject.h"
#include "Particles/BDParticleSystem.h"
USING_NS_CC;
//action
BDJumpAction::BDJumpAction(BDObject* pHost)
{
	m_lpHost = pHost;
	m_fAccVert = 0.0f;
	m_bRevocable = false;
}

BDJumpAction::BDJumpAction(BDObject* pHost,float fAcc)
{
	m_lpHost = pHost;
	m_fAccVert = fAcc;
	m_bRevocable = false;
}

BDJumpAction::~BDJumpAction()
{

}

int BDJumpAction::GetType()
{
	return BDACTION_JUMP;
}


void BDJumpAction::Update(float delta)
{
	BDCharacter* pCharacter = NULL;

	if(!m_lpHost)
		return;

	pCharacter = dynamic_cast<BDCharacter*>(m_lpHost); 

	if(!pCharacter)
		return;

	if(m_iStatus == BDACTION_READY)
	{
		pCharacter->GetMovementComp()->SetCurAccY(m_fAccVert);
		pCharacter->GetMovementComp()->SetCurSpeedY(pCharacter->GetMovementComp()->GetOrigSpeedY());
		pCharacter->GetMovementComp()->CmdUp();

		if(pCharacter->GetEmitter()==NULL)
		{
			pCharacter->SetEmitter(BDParticleSystem::create("lavaflow.plist",true));
			pCharacter->addChild(pCharacter->GetEmitter(), 10);
			pCharacter->GetEmitter()->setPosition(-50.0f,0.0f);
		}
		

		m_iStatus = BDACTION_STARTED;
	}

	if(m_iStatus == BDACTION_STARTED)
	{
		float speedY = pCharacter->GetMovementComp()->GetCurSpeed().y;
		CCPoint pos = pCharacter->GetArmature()->getPosition();
		if(abs(speedY) < 0.0f)
		{
			CCPoint acc = pCharacter->GetMovementComp()->GetCurAcc(); 
			acc.y = -0.7f;
			pCharacter->GetMovementComp()->SetCurAcc(acc);
			pCharacter->GetMovementComp()->CmdDown();
		}

		if(pos.y < 230.0f) 
		{
			pos.y = 230.0f;
			pCharacter->SetPosition(pos,true);
			pCharacter->GetMovementComp()->StopVert();
			m_iStatus = BDACTION_FINISHED;
			if(pCharacter->GetEmitter()!=NULL)
				pCharacter->GetEmitter()->stopSystem();
			//pCharacter->SetEmitter(NULL);
			OnActionDone();
		}

	}
}