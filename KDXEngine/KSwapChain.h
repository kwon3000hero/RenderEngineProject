#pragma once
#include <wrl.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#pragma comment(lib, "KEngineBase.lib")

class ID3D11Device;
class KGameWindow;
class IDXGISwapChain;

class KSwapChain final
{
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;

	struct
	{
		int m_adpaterIndex;
		Microsoft::WRL::ComPtr<IDXGIAdapter> m_pAdapter;
	};

	struct
	{
		int m_outputIndex;
		Microsoft::WRL::ComPtr<IDXGIOutput> m_pOutput;
	};

public:
	KSwapChain(int _adapterIndex, Microsoft::WRL::ComPtr<IDXGIAdapter> _pAdapter, int _outputIndex, Microsoft::WRL::ComPtr<IDXGIOutput> _pOutput);
	~KSwapChain();

	bool CreateSwapChain(Microsoft::WRL::ComPtr<ID3D11Device> _pDevice, KPTR<KGameWindow> _window);

	Microsoft::WRL::ComPtr<IDXGIOutput> Output()
	{
		return m_pOutput;
	}

	Microsoft::WRL::ComPtr<IDXGIAdapter> Adpater()
	{
		return m_pAdapter;
	}

public:
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain()
	{
		return m_pSwapChain;
	}

public:
	operator IDXGISwapChain* ()
	{
		return m_pSwapChain.Get();
	}
};

