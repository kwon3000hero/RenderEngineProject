#pragma once
#include <wrl.h>
#include <dxgi.h>
#include <d3d11_4.h>
#include <assert.h>
#include "KGameMacros.h"
#include <KGameWindow.h>
#include "KSwapChainWrapper.h"

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#pragma comment(lib, "KEngineBase.lib")

class ID3D11Device;
class KGameWindow;

template<typename T>
class KSwapChain final
{
private:
	typedef typename T::SwapChainType SwapChainType;
	typedef typename T::AdapterType AdapterType;
	typedef typename T::OutputType OutputType;

private:
	Microsoft::WRL::ComPtr<SwapChainType> m_pSwapChain;

	struct
	{
		int m_adpaterIndex;
		Microsoft::WRL::ComPtr<AdapterType> m_pAdapter;
	};

	struct
	{
		int m_outputIndex;
		Microsoft::WRL::ComPtr<OutputType> m_pOutput;
	};

public:
	KSwapChain(int _adapterIndex, Microsoft::WRL::ComPtr<AdapterType> _pAdapter, int _outputIndex, Microsoft::WRL::ComPtr<OutputType> _pOutput)
		: m_pSwapChain(nullptr), m_adpaterIndex(_adapterIndex), m_pAdapter(_pAdapter), m_outputIndex(_outputIndex), m_pOutput(_pOutput)
	{

	}

	~KSwapChain()
	{

	}

	bool CreateSwapChain(Microsoft::WRL::ComPtr<DeviceBuildVersion::DeviceType> _pDevice, KPTR<KGameWindow> _window);

	Microsoft::WRL::ComPtr<OutputType> Output()
	{
		return m_pOutput;
	}

	Microsoft::WRL::ComPtr<AdapterType> Adapter()
	{
		return m_pAdapter;
	}

public:
	Microsoft::WRL::ComPtr<SwapChainType> SwapChain()
	{
		return m_pSwapChain;
	}

public:
	operator SwapChainType* ()
	{
		return m_pSwapChain.Get();
	}
};