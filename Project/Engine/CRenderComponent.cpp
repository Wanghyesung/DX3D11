#include "pch.h"
#include "CRenderComponent.h"

#include "CTransform.h"
#include "CResMgr.h"
#include "CMaterial.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _type)
	: CComponent(_type)
	, m_bFrustumCheck(true)
{
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::render_shadowmap()
{
	Ptr<CMaterial> pShadowMapMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");

	//카메라기준 행렬 재계산
	Transform()->UpdateData();

	pShadowMapMtrl->UpdateData();

	GetMesh()->render();
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
	m_pSharedMtrl = _Mtrl;
	m_pCurrentMtrl = m_pSharedMtrl;
}

Ptr<CMaterial>CRenderComponent::GetSharedMaterial()
{
	//기존에 가리키던 것은 대입연산자 오버라이딩에서 카운트 값을 
	//내림 어차피 복사로 만들어도 mater은 크기가 작음
	if (m_pCurrentMtrl != m_pSharedMtrl)
	{
		m_pCurrentMtrl = m_pSharedMtrl;
		m_pDynamicMtrl = nullptr;
	}

	return m_pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// 원본 재질이 없다 -> Nullptr 반환
	if (nullptr == m_pSharedMtrl)
	{
		m_pCurrentMtrl = nullptr;
		return m_pCurrentMtrl;
	}

	// 동적 재질 최초 요청시 제작 해서 준다.
	if (nullptr == m_pDynamicMtrl)
	{		
		m_pDynamicMtrl = m_pSharedMtrl->Clone();		
	}

	// 동적 재질을 현재 사용재질로 지정한다.
	m_pCurrentMtrl = m_pDynamicMtrl;


	return m_pCurrentMtrl;
}

void CRenderComponent::SaveToLevelFile(FILE* _File)
{
	SaveResRef(m_pMesh.Get(), _File);
	SaveResRef(m_pSharedMtrl.Get(), _File);	
}

void CRenderComponent::LoadFromLevelFile(FILE* _File)
{
	LoadResRef(m_pMesh, _File);
	LoadResRef(m_pSharedMtrl, _File);

	SetMaterial(m_pSharedMtrl);
}
