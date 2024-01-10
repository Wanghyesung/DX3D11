
#include "pch.h"
#include "TestLevel.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CCameraMoveScript.h>
#include <Script\CCometScript.h>
#include <Script\CCometScript2.h>

#include "CLevelSaveLoad.h"


#include <Engine/CSetColorShader.h>
#include "TemLevel.h"


void CreateTemLevel()
{
	//return;

	// 컴퓨트 쉐이더 테스트
	Ptr<CTexture> pTestTexture =
		CResMgr::GetInst()->CreateTexture(L"ComputeTestTex"
			, 200, 200, DXGI_FORMAT_R8G8B8A8_UNORM
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
			, D3D11_USAGE_DEFAULT);

	Ptr<CSetColorShader> pCS = (CSetColorShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"SetColorCS").Get();
	pCS->SetTargetTexture(pTestTexture);
	pCS->SetColor(Vec3(1.f, 0.f, 1.f));
	pCS->Execute();


	//Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\BGM_Stage1.wav");
	//pSound->Play(1, 0.5f, false);

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Monster");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");

	CreateCircle();
	Createcomet();

	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraMoveScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetCameraIndex((int)CAMERA_TYPE::MAIN);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask((int)LAYER_TYPE::UI, false);// UI Layer 는 렌더링하지 않는다.

	SpawnGameObject(pMainCam, Vec3(0.f, 7000.f, 0.f), (int)LAYER_TYPE::Camera);

	// UI cameara
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICamera");
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);

	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICam->Camera()->SetCameraIndex((int)CAMERA_TYPE::UI);		// Sub 카메라로 지정
	pUICam->Camera()->SetLayerMask((int)LAYER_TYPE::UI, true);	// 31번 레이어만 체크

	SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), (int)LAYER_TYPE::Camera);

	//skybox 추가
	CGameObject* pSkyBox = new CGameObject();
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100));
	pSkyBox->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::SPHERE);
	pSkyBox->SkyBox()->SetSkyTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\Sky02.jpg"));

	SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), 0);





	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");
	
	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);
	
	//pLightObj->Transform()->SetRelativeRot(Vec3(XM_PI / 4.f, XM_PI / 4.f, 0.f));
	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetLightColor(Vec3(0.f, 0.f, 0.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.15f, 0.15f, 0.15f));
	
	SpawnGameObject(pLightObj, Vec3(0.f, 0.f, 0.f), 0);

	pLightObj = new CGameObject;
	pLightObj->SetName(L"Point Light 2");
	
	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);
	
	pLightObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f , 0.f));
	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::POINT);
	pLightObj->Light3D()->ShowRange(true);
	pLightObj->Light3D()->SetRadius(15000.f);
	pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.f, 0.f, 0.f));

	SpawnGameObject(pLightObj, Vec3(0.f, 0.f, 0.f), 0);
}

void CreateCircle()
{
	//CGameObject* pGameObj = new CGameObject;
	//pGameObj->SetName(L"Ring");
	//pGameObj->AddComponent(new CTransform);
	//pGameObj->AddComponent(new CMeshRender);
	//pGameObj->AddComponent(new CCometScript);
	//
	//pGameObj->Transform()->SetRelativeScale(Vec3(5000.f, 5000.f, 1.f));
	//pGameObj->Transform()->SetRelativeRot(Vec3(XM_PI / 2.f,0.f,0.f));
	//pGameObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
	//pGameObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"));
	//pGameObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01.tga"));
	//pGameObj->MeshRender()->GetMaterial()->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01_N.tga"));

	// 오브젝트 생성

	//태양 수금지화목토천혜
	CGameObject* pSun = new CGameObject;
	pSun->SetName(L"SUN");
	pSun->AddComponent(new CTransform);
	pSun->AddComponent(new CMeshRender);
	CCometScript* pSunsc = new CCometScript;
	pSunsc->SetSpeed(0.f);
	pSun->AddComponent(pSunsc);

	pSun->Transform()->SetRelativeScale(Vec3(3000.f, 3000.f, 3000.f));
	pSun->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	
	pSun->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pSun->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	pSun->MeshRender()->GetDynamicMaterial(0);
	int SpecCoeff = 1;
	pSun->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &SpecCoeff);
	int iActiveLight = 0;
	pSun->MeshRender()->GetMaterial(0)->SetScalarParam(INT_1, &iActiveLight);
	pSun->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\comet\\sun.jpg"));
	//pSun->MeshRender()->GetMaterial()->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01_N.tga"));
	SpawnGameObject(pSun, Vec3(0.f, 0.f, 0.f), (int)LAYER_TYPE::Default);


	CGameObject* pSU = new CGameObject;
	pSU->SetName(L"SU");
	pSU->AddComponent(new CTransform);
	pSU->AddComponent(new CMeshRender);
	CCometScript* pScript = new CCometScript;
	pScript->SetRadius(2500.f);
	pScript->SetSpeed(1000.f);
	pSU->AddComponent(pScript);


	pSU->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	pSU->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

	pSU->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pSU->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	pSU->MeshRender()->GetDynamicMaterial(0);
	pSU->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\comet\\su.jpg"));
	SpawnGameObject(pSU, Vec3(2500.f, 0.f, 0.f), (int)LAYER_TYPE::Default);

	CGameObject* pGM = new CGameObject;
	pGM->SetName(L"GM");
	pGM->AddComponent(new CTransform);
	pGM->AddComponent(new CMeshRender);
	pScript = new CCometScript;
	pScript->SetRadius(3300.f);
	pScript->SetSpeed(400.f);
	pGM->AddComponent(pScript);
	
	
	pGM->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 400.f));
	pGM->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	
	pGM->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pGM->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	pGM->MeshRender()->GetDynamicMaterial(0);
	pGM->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\comet\\gm.jpg"));
	SpawnGameObject(pGM, Vec3(3300.f, 0.f, 0.f), (int)LAYER_TYPE::Default);
	
	CGameObject* pGI = new CGameObject;
	pGI->SetName(L"GI");
	pGI->AddComponent(new CTransform);
	pGI->AddComponent(new CMeshRender);
	pScript = new CCometScript;
	pScript->SetRadius(4200.f);
	pScript->SetSpeed(800.f);
	pGI->AddComponent(pScript);
	//
	//
	pGI->Transform()->SetRelativeScale(Vec3(600.f, 600.f, 600.f));
	pGI->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	
	pGI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pGI->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	pGI->MeshRender()->GetDynamicMaterial(0);
	pGI->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\comet\\gi.jpg"));
	SpawnGameObject(pGI, Vec3(4200.f, 0.f, 0.f), (int)LAYER_TYPE::Default);
	//
	//
	CGameObject* pHW = new CGameObject;
	pHW->SetName(L"HW");
	pHW->AddComponent(new CTransform);
	pHW->AddComponent(new CMeshRender);
	pScript = new CCometScript;
	pScript->SetRadius(5200.f);
	pScript->SetSpeed(700.f);
	pHW->AddComponent(pScript);
	
	
	pHW->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	pHW->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	
	pHW->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pHW->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	pHW->MeshRender()->GetDynamicMaterial(0);
	pHW->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\comet\\hw.jpg"));
	SpawnGameObject(pHW, Vec3(5200.f, 0.f, 0.f), (int)LAYER_TYPE::Default);
	//
	//
	CGameObject* pMOK = new CGameObject;
	pMOK->SetName(L"MOK");
	pMOK->AddComponent(new CTransform);
	pMOK->AddComponent(new CMeshRender);
	pScript = new CCometScript;
	pScript->SetRadius(7500.f);
	pScript->SetSpeed(1000.f);
	pMOK->AddComponent(pScript);
	
	
	pMOK->Transform()->SetRelativeScale(Vec3(1800.f, 1800.f, 1800.f));
	pMOK->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	
	pMOK->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pMOK->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	//pSU->MeshRender()->GetDynamicMaterial();
	pMOK->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\comet\\mk.jpg"));
	SpawnGameObject(pMOK, Vec3(7500.f, 0.f, 0.f), (int)LAYER_TYPE::Default);
	
	
	CGameObject* pTO = new CGameObject;
	pTO->SetName(L"TO");
	pTO->AddComponent(new CTransform);
	pTO->AddComponent(new CMeshRender);
	pScript = new CCometScript;
	pScript->SetRadius(9400.f);
	pScript->SetSpeed(1200.f);
	pTO->AddComponent(pScript);
	
	
	pTO->Transform()->SetRelativeScale(Vec3(1600.f, 1600.f, 1600.f));
	pTO->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	
	pTO->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pTO->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	pTO->MeshRender()->GetDynamicMaterial(0);
	pTO->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\comet\\to.jpg"));
	SpawnGameObject(pTO, Vec3(9400.f, 0.f, 0.f), (int)LAYER_TYPE::Default);
	
	
	CGameObject* pCHUN = new CGameObject;
	pCHUN->SetName(L"CHUM");
	pCHUN->AddComponent(new CTransform);
	pCHUN->AddComponent(new CMeshRender);
	pScript = new CCometScript;
	pScript->SetRadius(10500.f);
	pScript->SetSpeed(2000.f);
	pCHUN->AddComponent(pScript);
	
	
	pCHUN->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1000.f));
	pCHUN->Transform()->SetRelativeRot(Vec3(1000.f, 1000.f, 1000.f));
	
	pCHUN->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pCHUN->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	pCHUN->MeshRender()->GetDynamicMaterial(0);
	pCHUN->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\comet\\chun.jpg"));
	SpawnGameObject(pCHUN, Vec3(10500.f, 0.f, 0.f), (int)LAYER_TYPE::Default);
	
	
	CGameObject* pHE = new CGameObject;
	pHE->SetName(L"HE");
	pHE->AddComponent(new CTransform);
	pHE->AddComponent(new CMeshRender);
	pScript = new CCometScript;
	pScript->SetRadius(12500.f);
	pScript->SetSpeed(750.f);
	pHE->AddComponent(pScript);
	
	
	pHE->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1000.f));
	pHE->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	
	pHE->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pHE->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	pHE->MeshRender()->GetDynamicMaterial(0);
	pHE->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\comet\\he.jpg"));
	SpawnGameObject(pHE, Vec3(12500.f, 0.f, 0.f), (int)LAYER_TYPE::Default);
}

void Createcomet()
{
	CGameObject* pComet = new CGameObject;
	pComet->SetName(L"Commet1");

	pComet->AddComponent(new CTransform);
	pComet->AddComponent(new CParticleSystem);

	SpawnGameObject(pComet, Vec3(0.f, 4000.f, 0.f), (int)LAYER_TYPE::Default);

	CGameObject* GameObj = new CGameObject;
	GameObj->SetName(L"Commet2");

	GameObj->AddComponent(new CTransform);
	GameObj->AddComponent(new CMeshRender);
	CCometScript2* pScript = new CCometScript2;
	pScript->SetTarget(pComet);
	GameObj->AddComponent(pScript);


	GameObj->Transform()->SetRelativeScale(Vec3(80.f, 80.f, 80.f));
	GameObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	
	GameObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	GameObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	GameObj->MeshRender()->GetDynamicMaterial(0);
	GameObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\comet\\sun.jpg"));
	SpawnGameObject(GameObj, Vec3(0, 4000.f, 0.f), (int)LAYER_TYPE::Default);

}
