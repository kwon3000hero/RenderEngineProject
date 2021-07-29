#include <vector>
#include <KGameWindow.h>
#include "KSwapChainManager.h"
#include "KGameMacros.h"


KSwapChainManager::KSwapChainManager() :m_pFactory(nullptr)
{

}

KSwapChainManager::~KSwapChainManager()
{

}

void KSwapChainManager::SearchAdapterAndOutput()
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

KSwapChain<SwapChainBuildVersion>* KSwapChainManager::GetOutput(int _adapterIndex, int _outputIndex)
{
	swapChainKey findedKey(_adapterIndex, _outputIndex);

	return m_swapchainContainer.find(findedKey)->second;
}
