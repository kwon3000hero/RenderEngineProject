#pragma once

#include "KSwapChain.h"
#include <map>
#include <KGameReference.h>

class KSwapChainManager final
{
private:
	typedef std::tuple<int, int, int, int> swapChainKey;
	std::map<swapChainKey, KSwapChain*> m_swapchainContainer;

public:
	KSwapChainManager();
	~KSwapChainManager();

	void SearchAdapterAndOutput();
	KSwapChain* SearchOutput(KPTR<KGameWindow> _window);
};