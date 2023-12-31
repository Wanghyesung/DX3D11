#include "pch.h"
#include "CCometScript.h"

CCometScript::CCometScript():
	CScript((UINT)SCRIPT_TYPE::COMETSCRIPT),
	m_vDir(Vec3::Zero),
	m_vecNextDir{},
	m_fSpeed(100.f),
	m_fRadius(0.f),
	m_iNextNum(0)
{
}

CCometScript::~CCometScript()
{
}


void CCometScript::begin()
{
	UINT Slice = 40;
	float fTheta = XM_2PI / (float)Slice;
	m_vecNextDir.resize(40);

	// 정점 위치 지정
	for (UINT i = 0; i < Slice; ++i)
	{
		m_vecNextDir[i] = Vec3(m_fRadius * cosf(fTheta * (float)i), 0.f, m_fRadius * sinf(fTheta * (float)i));
	}
}

void CCometScript::check_pos()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vNextPos = m_vecNextDir[m_iNextNum];

	Vec3 vDir = vNextPos - vPos;
	vDir.Normalize();

	vPos += (vDir * DT * m_fSpeed);
	Transform()->SetRelativePos(vPos);

	//Transform()->SetRelativePos(m_vecNextDir[m_iNextNum]);
}

void CCometScript::check_theta()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vNextPos = m_vecNextDir[m_iNextNum];

	float fLen = (vNextPos - vPos).Length();
	if (fLen <= 2.f)
	{
		++m_iNextNum;
		if (m_iNextNum >= 40)
			m_iNextNum = 0;
	}
}

void CCometScript::tick()
{
	check_theta();
	check_pos();

	Vec3 vRot = Transform()->GetRelativeRot();
	vRot.y += DT;
	Transform()->SetRelativeRot(vRot);
}

void CCometScript::SaveToLevelFile(FILE* _File)
{

}

void CCometScript::LoadFromLevelFile(FILE* _FILE)
{

}



