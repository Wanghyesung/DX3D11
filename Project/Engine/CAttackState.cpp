#include "pch.h"
#include "CAttackState.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
#include "CAnimation3D.h"
#include "CFSM.h"
void CAttackState::final_tick()
{
	CGameObject* pChildObj = GetOwner()->GetChild().at(0);
	bool bComplete = pChildObj->Animator3D()->GetCurAnim()->IsComplete();

	if (bComplete)
	{
		ChanageState(GetFSM(), STATE_TYPE::IDLE);
		return;
	}
}

void CAttackState::Exit()
{

}

void CAttackState::Enter()
{
	std::random_device rDiv;
	std::mt19937 en(rDiv());
	std::uniform_int_distribution<int> num(0, 7);
	int iNum = (int)num(en);

	m_strAttackNum = std::to_wstring(0);

	wstring strFinalAnim = GetName() + m_strAttackNum;
	Chanage_Anim(strFinalAnim);
}

void CAttackState::AddAttack(tAttackInfo _tAttackInfo)
{
	if (m_vecAttack.size() <= _tAttackInfo.iAttackNum)
	{
		m_vecAttack.resize(_tAttackInfo.iAttackNum + 1);
	}

	m_vecAttack[_tAttackInfo.iAttackNum] = _tAttackInfo;
}

CAttackState::CAttackState():
	m_strAttackNum(L"0"),
	m_vecAttack{}
{

}

CAttackState::~CAttackState()
{

}
