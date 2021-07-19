#include <vector>
#include <KGameWindow.h>
#include "KSwapChainManager.h"
#include "KGameMacros.h"

#pragma comment(lib, "dxgi.lib")

void KSwapChainManager::CreateOutputs()
{

	UINT adapterNumber = 0;
	IDXGIFactory* pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);

	IDXGIAdapter* pAdapter = nullptr;
	std::vector<IDXGIOutput*> vOutputs;

	while (DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters(adapterNumber, &pAdapter))
	{
		UINT outputNumber = 0;
		IDXGIOutput* pOutput;

		while (DXGI_ERROR_NOT_FOUND != pAdapter->EnumOutputs(outputNumber, &pOutput))
		{
			vOutputs.push_back(pOutput);
			++outputNumber;
		}

		++adapterNumber;
	}
}

KSwapChain* KSwapChainManager::SearchOutput(KPTR<KGameWindow> _window)
{
	swapChainKey key(_window->Size().IX(), _window->Size().IY(), _window->Size().IX(), _window->Size().IY());
	return m_swapchainContainer.find(key)->second;
}
