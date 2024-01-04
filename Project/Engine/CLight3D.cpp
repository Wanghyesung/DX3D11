#include "pch.h"
#include "CLight3D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

#include "CResMgr.h"

#include "CCamera.h"
CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D),
	m_bShowRange(false),
	m_LightIdx(-1),
	m_pCamObj(nullptr)
{
	
	m_pCamObj = new CGameObject;
	m_pCamObj->AddComponent(new CTransform);
	m_pCamObj->AddComponent(new CCamera);

	m_pCamObj->Camera()->SetLayerMaskAll(true);
	m_pCamObj->Camera()->SetLayerMask(31, false);

	SetLightType(LIGHT_TYPE::DIRECTIONAL);

}

CLight3D::CLight3D(const CLight3D& _Origin)
	: CComponent(_Origin)
	, m_bShowRange(_Origin.m_bShowRange)
	, m_LightIdx(-1)
	, m_pCamObj(nullptr)
{
	m_pCamObj = new CGameObject(*_Origin.m_pCamObj);
}

CLight3D::~CLight3D()
{
	if (nullptr != m_pCamObj)
		delete m_pCamObj;
}

void CLight3D::finaltick()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	
	m_LightIdx = (UINT)CRenderMgr::GetInst()->RegisterLight3D(this, m_LightInfo);

	//DrawDebugSphere(Transform()->GetWorldMat(), Vec4(0.2f, 1.f, 0.2f, 1.f), 0.f, true);

	if (m_bShowRange)
	{
		if ((UINT)LIGHT_TYPE::POINT == m_LightInfo.LightType)
		{
			DrawDebugSphere(Transform()->GetWorldMat(), Vec4(0.2f, 1.f, 0.2f, 1.f), 0.f, true);
		}
		else if ((UINT)LIGHT_TYPE::SPOT == m_LightInfo.LightType)
		{
			//DrawDebugSphere(Transform()->GetWorldMat(), Vec4(0.2f, 1.f, 0.2f, 1.f), 0.f, true);
		}
	}

	//카메라 위치 == 광원 위치
	*m_pCamObj->Transform() = *Transform();
	//광원이 가지고있는 모듈(카메라 업데이트)
	m_pCamObj->finaltick_module();
}

void CLight3D::render()
{
	Transform()->UpdateData();

	//light 재질 업데이트
	m_Mtrl->SetScalarParam(INT_0, &m_LightIdx);

	if (m_LightInfo.LightType == (UINT)LIGHT_TYPE::DIRECTIONAL)
	{
		Matrix matVP = m_pCamObj->Camera()->GetViewMat() * m_pCamObj->Camera()->GetProjMat();
		m_Mtrl->SetScalarParam(MAT_0, &matVP);
		m_Mtrl->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"DynamicShadowMapTex"));
	}

	m_Mtrl->UpdateData();

	m_Mesh->render();
}

void CLight3D::render_shadowmap()
{
	m_pCamObj->Camera()->SortObject_Shadow();

	m_pCamObj->Camera()->render_shadowmap();
}

void CLight3D::SetRadius(float _fRadius)
{
	m_LightInfo.Radius = _fRadius;

	// SphereMesh 의 로컬 반지름이 0.5f 이기 때문에 2배로 적용
	Transform()->SetRelativeScale(Vec3(_fRadius * 2.f, _fRadius * 2.f, _fRadius * 2.f));
}

void CLight3D::SetLightType(LIGHT_TYPE _type)
{
	m_LightInfo.LightType = (int)_type;

	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		//광원을 렌더링할 때, 광원의 영향범위를 형상화 할 수 있는 메쉬(볼륨메쉬)를 선택
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"); //내 렌더타겟 전체를 범위로
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl");

		m_pCamObj->Camera()->SetFar(100000.f);
		m_pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_pCamObj->Camera()->SetOrthoWidth(4000.f);
		m_pCamObj->Camera()->SetOrthoHeight(4000.f);
	}

	else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		//내 구 범위를 범위로
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");

	}

	else
	{
		//conemesh생성
		m_Mesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
		m_Mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"SpotLightMtrl");
	}

	if (nullptr != m_Mtrl)
	{
		//t0 ,t1 레지스터에 노말, 위치 텍스쳐 바인딩
		m_Mtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex"));
		m_Mtrl->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));

		//속성 1번 빛을 받을지 결정하는 불값
		m_Mtrl->SetTexParam(TEX_6, CResMgr::GetInst()->FindRes<CTexture>(L"DataTargetTex"));
	}

}

void CLight3D::SaveToLevelFile(FILE* _File)
{

}

void CLight3D::LoadFromLevelFile(FILE* _File)
{

}


