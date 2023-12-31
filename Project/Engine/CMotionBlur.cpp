#include "pch.h"
#include "CMotionBlur.h"


void CMotionBlur::finaltick()
{
}

void CMotionBlur::SaveToLevelFile(FILE* _File)
{
}

void CMotionBlur::LoadFromLevelFile(FILE* _FILE)
{
}

void CMotionBlur::create_mesh(float _fLocalScale)
{
	
}

void CMotionBlur::render()
{

}

void CMotionBlur::Draw(float _fLocalScale, Vec3 _vPostion, Vec3 _vScale, Vec3 _vRotate)
{

}


CMotionBlur::CMotionBlur():
	CComponent(COMPONENT_TYPE::MOTIONBLUR),
	m_fCurTime(0.f),
	m_fSpawnTime(0.f),
	m_vEndPos(Vec3::Zero),
	m_vStartPos(Vec3::Zero)
{

}

CMotionBlur::~CMotionBlur()
{

}

