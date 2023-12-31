#pragma once
#include "CComponent.h"

class CMotionBlur : public CComponent
{
private:
	Vec3 m_vStartPos;
	Vec3 m_vEndPos;

	float m_fSpawnTime;
	float m_fCurTime;

private:
	void create_mesh(float _fLocalScale);

public:
	void render();
	
	void Draw(float _fLocalScale, Vec3 _vPostion, Vec3 _vScale, Vec3 _vRotate);

public:
	CMotionBlur();
	~CMotionBlur();
	CLONE(CMotionBlur);

	virtual void finaltick()override;


	virtual void SaveToLevelFile(FILE* _File);
	virtual void LoadFromLevelFile(FILE* _FILE);


};

