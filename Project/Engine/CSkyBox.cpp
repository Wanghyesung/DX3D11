#include "pch.h"
#include "CSkyBox.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CTransform.h"


CSkyBox::CSkyBox():
	CRenderComponent(COMPONENT_TYPE::SKYBOX),
	m_Type(SKYBOX_TYPE::SPHERE)
{
	//생성자에서 바로 메쉬와 재질을 선택
	SetSkyBoxType(m_Type);
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyBoxMtrl"));

	SetFrustumCheck(false);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::SetSkyBoxType(SKYBOX_TYPE _Type)
{
	m_Type = _Type;

	if (m_Type == SKYBOX_TYPE::SPHERE)
	{
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	}
	else
	{
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	}
}

void CSkyBox::SetSkyTexture(Ptr<CTexture> _Tex)
{
	m_SkyBoxTex = _Tex;
}

void CSkyBox::finaltick()
{

}

void CSkyBox::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	Transform()->UpdateData();

	GetMaterial()->SetScalarParam(INT_0, &m_Type);

	if (nullptr != m_SkyBoxTex)
	{
		if (m_SkyBoxTex->IsCube())
		{
			//레지스터 8번에 바인딩 스카이박스 샘플링을 해야하기떄문
			GetMaterial()->SetTexParam(TEX_CUBE_0, m_SkyBoxTex);
		}
		else
		{
			GetMaterial()->SetTexParam(TEX_0, m_SkyBoxTex);
		}
		
	}

	GetMaterial()->UpdateData();

	//렌더링
	GetMesh()->render();
}

