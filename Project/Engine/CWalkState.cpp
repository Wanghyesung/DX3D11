#include "pch.h"
#include "CWalkState.h"
#include "CKeyMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRigidbody.h"

void CWalkState::final_tick()
{
	CGameObject* pObj = GetOwner();
	CRigidbody* pRigidbody = pObj->Rigidbody();
	Vec3 vCurPos = pObj->Transform()->GetRelativePos();

	Vec3 vFoward = pObj->Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = pObj->Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = pObj->Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = 1000.f;
	Vec3 vForce = Vec3::Zero;

	wstring strDir = {};
	if (KEY_PRESSED(KEY::W))
	{
		vFoward *= fSpeed;

		vForce += vFoward;

		strDir = L"Front";
	}

	if (KEY_PRESSED(KEY::D))
	{
		vRight *= fSpeed;

		vForce += vRight;

		strDir = L"Right";
	}
	else if (KEY_PRESSED(KEY::A))
	{
		vRight *= fSpeed;

		vForce -= vRight;

		strDir = L"Left";
	}

	if (KEY_PRESSED(KEY::S))
	{
		vFoward *= fSpeed;

		vForce -= vFoward;

		strDir = L"Back";
	}

	if (vForce == Vec3::Zero)
	{
		ChanageState(GetFSM(), STATE_TYPE::IDLE);
		return;
	}
	
	SetName(L"Walk_" + strDir);
	pRigidbody->AddForce(vForce);
}

void CWalkState::Enter()
{
	Chanage_Anim(GetName());
}

void CWalkState::Exit()
{

}

CWalkState::CWalkState()
{

}

CWalkState::~CWalkState()
{
}
