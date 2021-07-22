#pragma once

#include "KSwapChain.h"
#include <map>
#include <KGameReference.h>

class KSwapChainManager final
{
private:
	IDXGIFactory* m_pFactory;

	typedef std::pair<int, int> swapChainKey;
	std::map<swapChainKey, KSwapChain*> m_swapchainContainer;

	typedef std::pair<int, int> outputKey;
	std::map<outputKey, IDXGIOutput*> m_OutputContainer;

	std::map<int, IDXGIAdapter*> m_pAdapterContainer;

public:
	KSwapChainManager();
	~KSwapChainManager();

	void SearchAdapterAndOutput();
	KSwapChain* SearchOutput(KPTR<KGameWindow> _window);
};