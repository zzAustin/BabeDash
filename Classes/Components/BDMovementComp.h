#ifndef _BD_MOVEMENT_COMP_
#define _BD_MOVEMENT_COMP_

#include "cocos2d.h"

class BDMovementComp:public cocos2d::CCComponent
{
public:
	BDMovementComp();
	BDMovementComp(cocos2d::CCPoint& ptSpeed);
	static  BDMovementComp *Create();
	static  BDMovementComp *Create(cocos2d::CCPoint& ptSpeed);
	void onEnter();
	void onExit();
    void update(float delta);
	cocos2d::CCPoint& GetCurSpeed();
	cocos2d::CCPoint& GetOrigSpeed();
	cocos2d::CCPoint& GetCurAcc();

	float GetCurSpeedX();
	float GetCurSpeedY();
	float GetOrigSpeedX();
	float GetOrigSpeedY();
	float GetCurAccX();
	float GetCurAccY();

	void SetCurSpeed(const cocos2d::CCPoint& ptSpeed);
	void SetCurAcc(const cocos2d::CCPoint& ptAcc);

	void SetCurSpeedX(float fX);
	void SetCurSpeedY(float fY);
	void SetCurAccX(float fX);
	void SetCurAccY(float fY);

    
    void CmdRight();
    void CmdLeft();
    void CmdUp();
    void CmdDown();
    void StopHori();
    void StopVert();

private:
	cocos2d::CCPoint m_ptCurSpeed;
	cocos2d::CCPoint m_ptOrigSpeed;
    cocos2d::CCPoint m_ptOrigAcc;
    cocos2d::CCPoint m_ptCurAcc;
};

#endif