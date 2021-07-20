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
	IDXGIAdapter* pAdapter;
	std::vector<IDXGIOutput*> vOutputs;
	
public:
	KSwapChain();
	~KSwapChain();

	bool CreateSwapChain(ID3D11Device* _pDevice, KPTR<KGameWindow> _window);
	void Release();

public:
	IDXGISwapChain& SwapChain()
	{
		return *m_pSwapChain;
	}
};

