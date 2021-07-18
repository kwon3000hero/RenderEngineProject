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

public:
	KSwapChain();
	~KSwapChain();

	bool CreateSwapChain(ID3D11Device* _pDevice, KPTR<KGameWindow> _window);

public:
	IDXGISwapChain& SwapChain()
	{
		return *m_pSwapChain;
	}
};

