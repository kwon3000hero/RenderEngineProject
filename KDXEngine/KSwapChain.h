#pragma once
#include <dxgi.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#pragma comment(lib, "KEngineBase.lib")

class ID3D11Device;
class KGameWindow;
class IDXGISwapChain;

class KSwapChain
{
private:
	IDXGISwapChain* m_pSwapChain;

	struct
	{
		int m_adpaterIndex;
		IDXGIAdapter* m_pAdapter;
	};

	struct
	{
		int m_outputIndex;
		IDXGIOutput* m_pOutput;
	};

public:
	KSwapChain();//юс╫ц
	KSwapChain(int _adapterIndex, IDXGIAdapter* _pAdapter, int _outputIndex, IDXGIOutput* _pOutput);
	~KSwapChain();

	bool CreateSwapChain(ID3D11Device* _pDevice, KPTR<KGameWindow> _window);
	void Release();

public:
	IDXGISwapChain& SwapChain()
	{
		return *m_pSwapChain;
	}
};

