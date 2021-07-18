#pragma once

#include "KSwapChain.h"
#include <map>
#include <KGameReference.h>
//
//#pragma comment(lib, "d3d11")
//#pragma comment(lib, "d3dcompiler")
//#pragma comment(lib, "dxguid")


class KSwapChainManager
{
private:
	typedef std::pair<std::pair<int, int>, std::pair<int, int> > swapChainKey;
	std::map<swapChainKey, KSwapChain*> m_swapchainContainer;

public:
	void CreateOutputs();
	KSwapChain* SearchOutput(KPTR<KGameWindow> _window);
};