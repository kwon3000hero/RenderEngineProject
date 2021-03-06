#include "KRenderTarget.h"
#include "KDepthStencilTarget.h"
#include "KRenderer.h"
#include "KTexture.h"
#include "KRenderPipeline.h"

KPTR<KRenderer> KRenderTarget::m_MergeRenderPlayer;
KPTR<KRenderer> KRenderTarget::m_AlwaysMergeRenderPlayer;

void KRenderTarget::Create(KVector _Size, KVector _Color, DXGI_FORMAT _Fmt, UINT _BindFlag)
{
	if (_Size.x < 0)
	{
		assert(false);
	}

	if (_Size.y < 0)
	{
		assert(false);
	}

	m_ClearColor = _Color;

	KPTR<KTexture> pTexture = make_KPTR<KTexture>();
	pTexture->Create(_Size, _Fmt, _BindFlag, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	m_RenderTargetTextureContainer.push_back(pTexture);
	m_RenderTargetViewContainer.push_back(pTexture->m_pRTV);
	CreateViewPort(pTexture);
}

void KRenderTarget::Create(ID3D11Texture2D* _pTEX, KVector _Color, UINT _BindFlag)
{
	if (nullptr == _pTEX)
	{
		assert(false);
	}

	m_ClearColor = _Color;
	KPTR<KTexture> pTexture = make_KPTR<KTexture>();
	pTexture->Create(_pTEX, _BindFlag);
	m_RenderTargetTextureContainer.push_back(pTexture);
	m_RenderTargetViewContainer.push_back(pTexture->m_pRTV);
	CreateViewPort(pTexture);
}

void KRenderTarget::Clear(bool _isEnableDepth/* = true*/)
{
	for (auto& renderTarget : m_RenderTargetViewContainer)
	{
		KGameDeviceManager::MainContext()->ClearRenderTargetView(renderTarget, m_ClearColor.Arr);
	}

	if (nullptr != m_Depth.get() && true == _isEnableDepth)
	{
		m_Depth->Clear();
	}

}

void KRenderTarget::CreateViewPort(KPTR<KTexture> _texture)
{
	D3D11_VIEWPORT viewPort = {};

	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	viewPort.Width = (float)_texture->Size().UIX();
	viewPort.Height = (float)_texture->Size().UIY();

	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1.0f;

	m_ViewPortContainer.push_back(viewPort);
}

KPTR<KDepthStencilTarget> KRenderTarget::DepthStencil()
{
	return m_Depth;
}

void KRenderTarget::Setting()
{
	ID3D11DepthStencilView* DSV = nullptr;

	if (nullptr != m_Depth.get())
	{
		DSV = m_Depth->DSV();
	}

	KGameDeviceManager::MainContext()->RSSetViewports((unsigned int)m_ViewPortContainer.size(), &m_ViewPortContainer[0]);

	KGameDeviceManager::MainContext()->OMSetRenderTargets((unsigned int)m_RenderTargetTextureContainer.size(), &m_RenderTargetViewContainer[0], DSV);
}

void KRenderTarget::Setting(unsigned int _index)
{
	ID3D11DepthStencilView* DSV = nullptr;

	if (nullptr != m_Depth.get())
	{
		DSV = m_Depth->DSV();
	}

	KGameDeviceManager::MainContext()->RSSetViewports(1, &m_ViewPortContainer[_index]);
	KGameDeviceManager::MainContext()->OMSetRenderTargets(1, &m_RenderTargetViewContainer[_index], DSV);
}

void KRenderTarget::SetDepthStencil(KPTR<KDepthStencilTarget> _depthStencil)
{
	m_Depth = _depthStencil;
}

void KRenderTarget::Init()
{
	m_MergeRenderPlayer = make_KPTR<KRenderer>(L"FULLRECT", L"MERGETARGET");
	m_MergeRenderPlayer->SetSampler(L"Smp", L"LWSMP");

	m_AlwaysMergeRenderPlayer = make_KPTR<KRenderer>(L"FULLRECT", L"ALWAYSMERGETARGET");
	m_AlwaysMergeRenderPlayer->SetSampler(L"Smp", L"LWSMP");
}

void KRenderTarget::End()
{
}

void KRenderTarget::CreateDepth()
{
	m_Depth = make_KPTR<KDepthStencilTarget>();
	m_Depth->Create(m_RenderTargetTextureContainer[0]->Size(), 1.0f, 0);
}

void KRenderTarget::Merge(unsigned int _srcIndex, unsigned int  _destIndex, KPTR<KRenderTarget> _dest)
{
	Setting(_srcIndex);
	m_MergeRenderPlayer->SetTexture(L"MergeTex", _dest->m_RenderTargetTextureContainer[_destIndex]);
	m_MergeRenderPlayer->Render();
}

void KRenderTarget::Merge(KPTR<KRenderTarget> _dest)
{
	Merge(0, 0, _dest);
}

void KRenderTarget::AlwaysMerge(unsigned int _srcIndex, unsigned int _destIndex, KPTR<KRenderTarget> _dest)
{
	Setting(_srcIndex);
	m_AlwaysMergeRenderPlayer->SetTexture(L"MergeTex", _dest->m_RenderTargetTextureContainer[_destIndex]);
	m_AlwaysMergeRenderPlayer->Render();
}

void KRenderTarget::AlwaysMerge(KPTR<KRenderTarget> _dest)
{
	AlwaysMerge(0, 0, _dest);
}

void KRenderTarget::AlwaysCopy(unsigned int _srcIndex, unsigned int _destIndex, KPTR<KRenderTarget> _dest)
{
	Clear();
	Setting(_srcIndex);
	m_AlwaysMergeRenderPlayer->SetTexture(L"MergeTex", _dest->m_RenderTargetTextureContainer[_destIndex]);
	m_AlwaysMergeRenderPlayer->Render();

}

void KRenderTarget::AlwaysCopy(KPTR<KTexture> _dest)
{
	Clear();
	Setting(0);
	m_AlwaysMergeRenderPlayer->SetTexture(L"MergeTex", _dest);
	m_AlwaysMergeRenderPlayer->Render();

}

void KRenderTarget::AlwaysCopy(KPTR<KRenderTarget> _dest)
{
	AlwaysMerge(0, 0, _dest);
}

void KRenderTarget::Effect(int _index, KPTR<KRenderer> _renderPlayer, bool _IsClear, bool _IsDepthClear)
{
	if (true == _IsClear)
	{
		Clear(_IsDepthClear);
	}

	Setting(_index);
	_renderPlayer->Render();
}

void KRenderTarget::Effect(KPTR<KRenderer> _renderPlayer, bool _IsClear, bool _IsDepthClear)
{
	if (true == _IsClear)
	{
		Clear(_IsDepthClear);
	}

	Setting();
	_renderPlayer->Render();
}

KPTR<KTexture> KRenderTarget::Texture(unsigned int _Index)
{
	if (_Index >= m_RenderTargetTextureContainer.size())
	{
		AssertMsg(L"?????? ?????? ??????????????.");
	}
	return m_RenderTargetTextureContainer[_Index];
}
