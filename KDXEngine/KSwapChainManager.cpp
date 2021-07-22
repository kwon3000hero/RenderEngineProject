#include <vector>
#include <KGameWindow.h>
#include "KSwapChainManager.h"
#include "KGameMacros.h"

#pragma comment(lib, "dxgi.lib")

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

			m_swapchainContainer[key] = new KSwapChain(adapterNumber, pAdapter, outputNumber, pOutput);
		}

		++adapterNumber;
	}
}

KSwapChain* KSwapChainManager::SearchOutput(KPTR<KGameWindow> _window)
{
	swapChainKey findedKey(_window->Size().IX(), _window->Size().IY());

	return m_swapchainContainer.find(findedKey)->second;
}
