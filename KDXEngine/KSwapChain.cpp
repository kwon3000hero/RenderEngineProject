#include "KSwapChain.h"
#include <d3d11_4.h>
#include <assert.h>
#include <KGAMEWIN.h>
#include "KGameMacros.h"



KSwapChain::KSwapChain() :m_pSwapChain(nullptr)
{

}


bool KSwapChain::CreateSwapChain(ID3D11Device* _pDevice, KPTR<KGameWindow> _window)
{
	DXGI_SWAP_CHAIN_DESC SCDECS;
	memset(&SCDECS, 0, sizeof(DXGI_SWAP_CHAIN_DESC));


	SCDECS.BufferDesc.Width = _window->Size().UIX();
	SCDECS.BufferDesc.Height = _window->Size().UIY();

	SCDECS.BufferDesc.RefreshRate.Numerator = 60;
	SCDECS.BufferDesc.RefreshRate.Denominator = 1;

	SCDECS.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SCDECS.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SCDECS.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SCDECS.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SCDECS.SampleDesc.Quality = 0;
	SCDECS.SampleDesc.Count = 1;
	SCDECS.OutputWindow = _window->WINHWND();

	// 이걸 2개로 만들어서 처리
	SCDECS.BufferCount = 2;
	SCDECS.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SCDECS.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SCDECS.Windowed = true;
	IDXGIDevice* pDXGIDevice = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	IDXGIFactory* pFactory = nullptr;

	_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
	if (nullptr == pDXGIDevice)
	{
		assert(false);
		return false;
	}

	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);
	if (nullptr == pAdapter)
	{
		assert(false);
		return false;
	}

	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (nullptr == pFactory)
	{
		assert(false);
		return false;
	}

	if (S_OK != pFactory->CreateSwapChain(_pDevice, &SCDECS, &m_pSwapChain))
	{
		assert(false);
		return false;
	}

	SafeRelease(pDXGIDevice);
	SafeRelease(pAdapter);
	SafeRelease(pFactory);

	return true;
}


KSwapChain::~KSwapChain()
{
	SafeRelease(m_pSwapChain);
}
