#pragma once
#include "CEntity.h"

#include "CTexture.h"
#include "ptr.h"

//multi render target
class CMRT : public CEntity
{
private:
	Ptr<CTexture>  m_arrRT[8];
	UINT		   m_RTCount;
	Ptr<CTexture>  m_DSTex;

	Vec4		   m_ClearColor[8];

	D3D11_VIEWPORT m_ViewPort;

public:
	void Create(Ptr<CTexture>* _arrRtTex, UINT _RtCount, Ptr<CTexture> _DsTex);
	void SetClearColor(Vec4 _ClearColor, UINT _RTIndx) { m_ClearColor[_RTIndx] = _ClearColor; }
	void ClearTarget();
	void OMSet(bool _bStay = false);

public:
	CLONE_DISABLE(CMRT);

public:
	CMRT();
	~CMRT();
};

