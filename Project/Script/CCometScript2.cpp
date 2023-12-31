#include "pch.h"
#include "CCometScript2.h"

#include "CCometScript.h"

CCometScript2::CCometScript2():
	CScript((UINT)SCRIPT_TYPE::COMETSCRIPT2),
	fRadian(0.f)
{

}

CCometScript2::~CCometScript2()
{

}

void CCometScript2::begin()
{
	if (!m_pParticleObj)
		assert(nullptr);
}

void CCometScript2::tick()
{
	fRadian += (DT/5.f);

	//우치랑 방향 갱신
	//나
	Vec3 vDir = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vPos =Transform()->GetRelativePos();

	
	Transform()->SetRelativeRot(Vec3(fRadian, 0.f, 0.f));

	Vec3 vAddForc = vDir * 1000.f * DT;
	vPos += vAddForc;
	Transform()->SetRelativePos(vPos);


	//파티클
	Vec3 vTargetPos = m_pParticleObj->Transform()->GetRelativePos();
	vTargetPos += vAddForc;


	m_pParticleObj->Transform()->SetRelativePos(vTargetPos);
	m_pParticleObj->ParticleSystem()->SetDir(-vDir * 220.f);
}

void CCometScript2::SaveToLevelFile(FILE* _File)
{
}

void CCometScript2::LoadFromLevelFile(FILE* _FILE)
{
}


