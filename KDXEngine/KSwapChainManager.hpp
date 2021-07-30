#pragma once
#include "KSwapChainManager.h"

#if WDK_NTDDI_VERSION >= NTDDI_WIN10_19H1
template<>
void KSwapChainManager<KSwapChainWrapper6>::SearchAdapterAndOutput()
{
	Microsoft::WRL::ComPtr<IDXGIFactory2> factory2(nullptr);
	UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	HRESULT factoryResult = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory2));

	if (FAILED(factoryResult))
	{
		ShowHResultString(factoryResult, "KSwapChainManager::SearchAdapterAndOutput");
	}

	UINT adapterNumber = 0;
	IDXGIAdapter* pAdapter = nullptr;

	while (DXGI_ERROR_NOT_FOUND != m_pFactory->EnumAdapters(adapterNumber, &pAdapter))
	{
		m_pAdapterContainer[adapterNumber] = pAdapter;

		UINT outputNumber = 0;
		IDXGIOutput* pOutput;

		while (DXGI_ERROR_NOT_FOUND != pAdapter->EnumOutputs(outputNumber, &pOutput))
		{
			outputKey key(adapterNumber, outputNumber);
			m_OutputContainer[key] = pOutput;
			++outputNumber;

			m_swapchainContainer[key] = new KSwapChain<SwapChainBuildVersion>(adapterNumber, pAdapter, outputNumber, pOutput);
		}

		++adapterNumber;
	}
}
#endif
#if WDK_NTDDI_VERSION >= NTDDI_WIN10
template<>
void KSwapChainManager<KSwapChainWrapper0>::SearchAdapterAndOutput()
{
	HRESULT factoryResult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pFactory);

	if (FAILED(factoryResult))
	{
		ShowHResultString(factoryResult, "KSwapChainManager::SearchAdapterAndOutput");
	}

	UINT adapterNumber = 0;
	IDXGIAdapter* pAdapter = nullptr;

	while (DXGI_ERROR_NOT_FOUND != m_pFactory->EnumAdapters(adapterNumber, &pAdapter))
	{
		m_pAdapterContainer[adapterNumber] = pAdapter;

		UINT outputNumber = 0;
		IDXGIOutput* pOutput;

		while (DXGI_ERROR_NOT_FOUND != pAdapter->EnumOutputs(outputNumber, &pOutput))
		{
			outputKey key(adapterNumber, outputNumber);
			m_OutputContainer[key] = pOutput;
			++outputNumber;

			m_swapchainContainer[key] = new KSwapChain<SwapChainBuildVersion>(adapterNumber, pAdapter, outputNumber, pOutput);
		}

		++adapterNumber;
	}
}
#endif