#pragma once
#include "CGameObject.h"

class CComet : public CGameObject
{
public:
	CComet();
	CComet(const CComet& _Other);
	virtual ~CComet();


	CLONE(CGameObject);

public:
	virtual void finaltick()override;

};

