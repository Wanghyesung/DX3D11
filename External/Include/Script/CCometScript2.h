#pragma once

#include "Engine\CScript.h"

class CCometScript2 : public CScript
{
private:
    CGameObject* m_pParticleObj;

    float fRadian;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetTarget(CGameObject* _pGameObj) { m_pParticleObj = _pGameObj; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CCometScript2);

    CCometScript2();
    ~CCometScript2();

};

