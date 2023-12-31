#pragma once
#include "CSingleton.h"

class CLevel;
class CGameObject;

class CLevelMgr :
    public CSingleton<CLevelMgr>
{   
    SINGLE(CLevelMgr);
private:
    CLevel*     m_pCurLevel;

    vector<CLevel*> m_vecLevel;

public:
    CLevel* GetCurLevel() { return m_pCurLevel; }
    CGameObject* FindObjectByName(const wstring& _strName);
    void FindObjectByName(const wstring& _strName, vector<CGameObject*>& _Out);

    void ChangeLevel(CLevel* _NextLevel);
    CLevel* FindLevel(const wstring& _strName);
    CLevel* FindLevel(const LEVEL_TYPE& _eType);
    void AddLevel(CLevel* _pLevel) { m_vecLevel.push_back(_pLevel); }

public:
    void init();
    void tick();
};

