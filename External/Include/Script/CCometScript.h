#pragma once
#include "Engine\CScript.h"

class CCometScript :
    public CScript
{
private:
    float m_fSpeed;
    float m_fRadius;

    Vec3  m_vDir;
    vector<Vec3>  m_vecNextDir;

    UINT m_iNextNum;

    float m_fCurTime;
    float m_fMaxTime;

private:
    void check_pos();
    void check_theta();

public:
    virtual void begin() override;
    virtual void tick() override;

    void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
    void SetRadius(float _fRadius) { m_fRadius = _fRadius; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CCometScript);

    CCometScript();
    ~CCometScript();
};
