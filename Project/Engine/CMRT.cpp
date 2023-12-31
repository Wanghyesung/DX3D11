#include "pch.h"
#include "CMRT.h"

#include "CDevice.h"

CMRT::CMRT():
	m_arrRT{},
	m_RTCount(0),
	m_ClearColor{},
	m_ViewPort{}
{
}

CMRT::~CMRT()
{
}

void CMRT::Create(Ptr<CTexture>* _arrRtTex, UINT _RtCount, Ptr<CTexture> _DsTex)
{
	for (UINT i = 0; i < _RtCount; ++i)
	{
		m_arrRT[i] = _arrRtTex[i];
 	}

	m_RTCount = _RtCount;

	m_DSTex = _DsTex;

	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;

	m_ViewPort.Width = _arrRtTex[0]->Width();
	m_ViewPort.Height = _arrRtTex[0]->Height();

	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;

}

void CMRT::ClearTarget()
{
	for (UINT i = 0; i < m_RTCount; ++i)
	{
		CONTEXT->ClearRenderTargetView(m_arrRT[i]->GetRTV().Get(), m_ClearColor[i]);
	}

	//ds가 업을 수 있음
	if (nullptr != m_DSTex)
	{
		CONTEXT->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.f, 0.f);
	}	
}

void CMRT::OMSet(bool _bStay)
{
	ID3D11RenderTargetView* arrRTV[8] = {};
	for (UINT i = 0; i < m_RTCount; ++i)
	{
		arrRTV[i] = m_arrRT[i]->GetRTV().Get();
	}
	//8개의 렌더타켓 배열, 1개의 깊이버퍼를 렌더타켓으로
	if (nullptr != m_DSTex)
	{
		CONTEXT->OMSetRenderTargets(m_RTCount, arrRTV, m_DSTex->GetDSV().Get());
	}
	else
	{
		ComPtr<ID3D11DepthStencilView> pDSV = nullptr;

		//스왑체인 깊이버러를 공유함 가장 최근의 깊이버러플 가져옴
		if (_bStay)
		{
			CONTEXT->OMGetRenderTargets(0, nullptr, pDSV.GetAddressOf());
		}

		CONTEXT->OMSetRenderTargets(m_RTCount, arrRTV, pDSV.Get());
	}
	
	CONTEXT->RSSetViewports(1, &m_ViewPort);
}


