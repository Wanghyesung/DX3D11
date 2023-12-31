#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>

#include <Engine\CRenderMgr.h>
#include <Engine\CRigidbody.h>

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(100.f)
	, m_iActive(1)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Player Speed");
	AddScriptParam(SCRIPT_PARAM::INT, &m_iActive, "Player Active");
}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::begin()
{
	MeshRender()->GetDynamicMaterial();
}

void CPlayerScript::tick()
{
	if (m_iActive == 0)
		return;

	//Vec3 vCurPos = Transform()->GetRelativePos();
	
	Vec3 vFoward = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	Vec3 vForce = Vec3::Zero;
	if (KEY_PRESSED(KEY::LSHIFT))
	{
		m_fSpeed = 1600.f;
	}
	else
	{
		m_fSpeed = 800.f;
	}

	if (KEY_PRESSED(KEY::W))
	{
		vFoward *= m_fSpeed;
		
		vForce += vFoward;
	}

	else if (KEY_PRESSED(KEY::S))
	{
		
		vFoward *= m_fSpeed;
		
		vForce -= vFoward;

	}

	if (KEY_PRESSED(KEY::A))
	{
		vRight *= m_fSpeed;
		
		vForce -= vRight;

	}

	else if (KEY_PRESSED(KEY::D))
	{
		vRight *= m_fSpeed;
		
		vForce += vRight;
	}

	Rigidbody()->AddForce(vForce);
	//Transform()->SetRelativePos(vCurPos);
}

void CPlayerScript::Shoot()
{
	// 미사일 프리팹 참조
	Ptr<CPrefab> pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
	Vec3 vMissilePos = Transform()->GetRelativePos() + Vec3(0.f, 0.5f, 0.f) * Transform()->GetRelativeScale();
	CGameObject* pCloneMissile = pMissilePrefab->Instantiate();

	// 레벨에 추가
	SpawnGameObject(pCloneMissile, vMissilePos, L"PlayerProjectile");
}



void CPlayerScript::BeginOverlap(CCollider2D* _Other)
{
	
}





void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
}