#pragma once

#include "KSwapChain.h"
#include <map>
#include <KGameReference.h>

class KSwapChainManager
{
private:
	typedef std::tuple<int, int, int, int> swapChainKey;
	std::map<swapChainKey, KSwapChain*> m_swapchainContainer;

public:
	void CreateOutputs();
	KSwapChain* SearchOutput(KPTR<KGameWindow> _window);
};