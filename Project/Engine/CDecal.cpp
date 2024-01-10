#include "pch.h"
#include "CDecal.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "CTransform.h"
CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
	, m_bDeferred(false)
	, m_bEmissive(false)
{
	//강제로 큐브메쉬와 데칼 재질을 가지게한다 또한 위치값 확인을 위해 포지션 텍스쳐를 가져온다
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	SetDeferredDecal(m_bDeferred);
}

CDecal::~CDecal()
{
}

void CDecal::SetDeferredDecal(bool _bDeferred)
{

	m_bDeferred = _bDeferred;

	if (m_bDeferred)
	{
		SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DeferredDecalMtrl"),0);
	}
	else
	{
		SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DecalMtrl"),0);
	}

	GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
}

void CDecal::finaltick()
{
	DrawDebugCube(Transform()->GetWorldMat(), Vec4(0.f, 1.f, 0.f, 1.f), 0.f, true);
}

void CDecal::render()
{

	if (nullptr == GetMaterial(0) || nullptr == GetMesh())
		return;

	Transform()->UpdateData();

	//자페발광할것인지
	int IsEmissive = m_bEmissive;
	GetMaterial(0)->SetScalarParam(INT_0, &IsEmissive);
	GetMaterial(0)->UpdateData();

	GetMesh()->render(0);
}


