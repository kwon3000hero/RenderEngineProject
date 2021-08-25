#pragma once

#include "KSwapChain.h"
#include "KGameMacros.h"
#include <map>
#include <KGameReference.h>
#include <vector>
#include <KGameWindow.h>

template<typename T>
class KSwapChainManager final
{
private:
	typedef typename T::FactoryType FactoryType;
	typedef typename T::SwapChainType SwapChainType;
	typedef typename T::AdapterType AdapterType;
	typedef typename T::OutputType OutputType;

private:
	Microsoft::WRL::ComPtr<FactoryType> m_pFactory;

	typedef std::pair<int, int> swapChainKey;
	std::map<swapChainKey, KSwapChain<SwapChainSelectedWrapper>*> m_swapchainContainer;

	typedef std::pair<int, int> outputKey;
	std::map<outputKey, Microsoft::WRL::ComPtr<OutputType>> m_OutputContainer;

	std::map<int, Microsoft::WRL::ComPtr<AdapterType>> m_pAdapterContainer;

public:
	KSwapChainManager() :m_pFactory(nullptr)
	{

	}

	~KSwapChainManager()
	{

	}

	void SearchAdapterAndOutput();
	
	//임시.. output의 영역으로 찾게 수정해야...	
	KSwapChain<SwapChainSelectedWrapper>* GetOutput(int _adapterIndex, int _outputIndex)
	{
		swapChainKey findedKey(_adapterIndex, _outputIndex);

		return m_swapchainContainer.find(findedKey)->second;
	}
};