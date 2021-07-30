#pragma once
#include "KSwapChain.h"

#if WDK_NTDDI_VERSION >= NTDDI_WIN10_19H1
template<>
bool KSwapChain<KSwapChainWrapper6>::CreateSwapChain(Microsoft::WRL::ComPtr<ID3D11Device> _pDevice, KPTR<KGameWindow> _window)
{
	DXGI_SWAP_CHAIN_DESC1 SCDECS;
	memset(&SCDECS, 0, sizeof(DXGI_SWAP_CHAIN_DESC1));


	SCDECS.Width = _window->Size().UIX();
	SCDECS.Height = _window->Size().UIY();

	SCDECS.Stereo = true;
	SCDECS.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	SCDECS.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SCDECS.Scaling = DXGI_SCALING::DXGI_SCALING_ASPECT_RATIO_STRETCH;

	SCDECS.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SCDECS.SampleDesc.Quality = 0;
	SCDECS.SampleDesc.Count = 1;

	SCDECS.BufferCount = 2;
	SCDECS.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	SCDECS.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	Microsoft::WRL::ComPtr<IDXGIFactory2> pFactory(nullptr);
	m_pAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pFactory);
	if (nullptr == pFactory)
	{
		assert(false);
		return false;
	}

	if (S_OK != pFactory->CreateSwapChainForHwnd(_pDevice.Get(), _window->WINHWND(), &SCDECS, NULL, NULL, reinterpret_cast<IDXGISwapChain1**>(m_pSwapChain.Get())))
	{
		assert(false);
		return false;
	}

	return true;
}
#endif

#if WDK_NTDDI_VERSION >= NTDDI_WIN10
template<>
bool KSwapChain<KSwapChainWrapper0>::CreateSwapChain(Microsoft::WRL::ComPtr<ID3D11Device> _pDevice, KPTR<KGameWindow> _window)
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

	SCDECS.BufferCount = 2;
	SCDECS.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SCDECS.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SCDECS.Windowed = true;

	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory(nullptr);

	m_pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (nullptr == pFactory)
	{
		assert(false);
		return false;
	}

	if (S_OK != pFactory->CreateSwapChain(_pDevice.Get(), &SCDECS, &m_pSwapChain))
	{
		assert(false);
		return false;
	}

	return true;
}
#endif