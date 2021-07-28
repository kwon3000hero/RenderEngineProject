#pragma once

#include "KSwapChain.h"
#include <map>
#include <KGameReference.h>

class KSwapChainManager final
{
private:
	Microsoft::WRL::ComPtr<IDXGIFactory> m_pFactory;

	typedef std::pair<int, int> swapChainKey;
	std::map<swapChainKey, KSwapChain<SwapChainVersion>*> m_swapchainContainer;

	typedef std::pair<int, int> outputKey;
	std::map<outputKey, IDXGIOutput*> m_OutputContainer;

	std::map<int, IDXGIAdapter*> m_pAdapterContainer;

public:
	KSwapChainManager();
	~KSwapChainManager();

	void SearchAdapterAndOutput();
	
	//임시.. output의 영역으로 찾게 수정해야...
	KSwapChain<SwapChainVersion>* GetOutput(int _adapterIndex, int _outputIndex);
};