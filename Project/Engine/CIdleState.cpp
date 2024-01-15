#include "pch.h"
#include "CIdleState.h"
#include "CAnimator3D.h"

#include "CKeyMgr.h"
void CIdleState::final_tick()
{
	if(KEY_TAP(KEY::A) || KEY_TAP(KEY::S) ||
		KEY_TAP(KEY::W) || KEY_TAP(KEY::D))
	{
		ChanageState(GetFSM(), STATE_TYPE::WALK);
	}

}

void CIdleState::Enter()
{
	//Idle
	Chanage_Anim(GetName());
}

void CIdleState::Exit()
{

}

CIdleState::CIdleState()
{

}

CIdleState::~CIdleState()
{

}
