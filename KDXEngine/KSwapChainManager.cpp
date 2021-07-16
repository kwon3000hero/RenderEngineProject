#include "KSwapChainManager.h"
#include <vector>

void KSwapChainManager::CreateOutputs()
{

	/*UINT i = 0;
	IDXGIFactory* pFactory;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
	IDXGIAdapter* pAdapter;
	std::vector <IDXGIAdapter*> vAdapters;*/
	/*while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		vAdapters.push_back(pAdapter);
		++i;
	}*/

}

KSwapChain* KSwapChainManager::SearchOutput(KPTR<KGameWindow> _window)
{
	return nullptr;//m_swapchainContainer.find(_window.Size().x);
}
