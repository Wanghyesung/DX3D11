#pragma once
#include "CEntity.h"
#include "CState.h"

class CFSM : public CEntity
{
private:
	CGameObject* m_pOwner;

	map<STATE_TYPE, CState*> m_mapState;

	CState* m_pCurState;
public:
	void final_tick();

public:
	void SetOwner(CGameObject* _pObj) { m_pOwner = _pObj; }
	CGameObject* GetOwner() { return m_pOwner; }

	CState* FindState(STATE_TYPE _eType);

	void ChanageState(STATE_TYPE _eType);
	void AddState(STATE_TYPE _eType, CState* _pState);
	void SetState(STATE_TYPE _eType);

	CLONE_DISABLE(CFSM);
public:
	CFSM();
	virtual ~CFSM();

};

