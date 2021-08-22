#pragma once
#include "KSwapChainManager.h"

#if SwapChainVersion == SwapChainWrapper6
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

	Microsoft::WRL::ComPtr<IDXGIFactory3> factory3(nullptr);
	factoryResult = factory2->QueryInterface(IID_PPV_ARGS(&factory3));
	if (FAILED(factoryResult))
	{
		ShowHResultString(factoryResult, "KSwapChainManager::SearchAdapterAndOutput1");
	}

	Microsoft::WRL::ComPtr<IDXGIFactory4> factory4(nullptr);
	factoryResult = factory3->QueryInterface(IID_PPV_ARGS(&factory4));
	if (FAILED(factoryResult))
	{
		ShowHResultString(factoryResult, "KSwapChainManager::SearchAdapterAndOutput2");
	}

	Microsoft::WRL::ComPtr<IDXGIFactory5> factory5(nullptr);
	factoryResult = factory4->QueryInterface(IID_PPV_ARGS(&factory5));
	if (FAILED(factoryResult))
	{
		ShowHResultString(factoryResult, "KSwapChainManager::SearchAdapterAndOutput3");
	}

	Microsoft::WRL::ComPtr<IDXGIFactory6> factory6(nullptr);
	factoryResult = factory5->QueryInterface(IID_PPV_ARGS(&factory6));
	if (FAILED(factoryResult))
	{
		ShowHResultString(factoryResult, "KSwapChainManager::SearchAdapterAndOutput4");
	}

	factoryResult = factory6->QueryInterface(IID_PPV_ARGS(&m_pFactory));
	if (FAILED(factoryResult))
	{
		ShowHResultString(factoryResult, "KSwapChainManager::SearchAdapterAndOutput5");
	}

	UINT adapterNumber = 0;
	IDXGIAdapter4* pAdapter = nullptr;

	while (DXGI_ERROR_NOT_FOUND != m_pFactory->EnumAdapters1(adapterNumber, reinterpret_cast<IDXGIAdapter1**>(&pAdapter)))
	{
		m_pAdapterContainer[adapterNumber] = pAdapter;

		UINT outputNumber = 0;
		IDXGIOutput6* pOutput;

		while (DXGI_ERROR_NOT_FOUND != pAdapter->EnumOutputs(outputNumber, reinterpret_cast<IDXGIOutput**>(&pOutput)))
		{
			outputKey key(adapterNumber, outputNumber);
			m_OutputContainer[key] = pOutput;
			++outputNumber;

			m_swapchainContainer[key] = new KSwapChain<SwapChainSelectedWrapper>(adapterNumber, pAdapter, outputNumber, pOutput);
		}

		++adapterNumber;
	}
}
#endif 
#if SwapChainVersion == SwapChainWrapper0
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

			m_swapchainContainer[key] = new KSwapChain<SwapChainSelectedWrapper>(adapterNumber, pAdapter, outputNumber, pOutput);
		}

		++adapterNumber;
	}
}
#endif