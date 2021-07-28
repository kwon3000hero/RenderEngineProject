#pragma once
#include <dxgi.h>

#pragma comment(lib, "dxguid")

//Ÿ�� OS�� ���� SwapChain ������ ������.
#if WDK_NTDDI_VERSION == NTDDI_WIN10_19H1                    
#define SwapChainBuildVersion KSwapChainWrapper0
#elif WDK_NTDDI_VERSION == NTDDI_WIN10_RS5
#else
#define SwapChainBuildVersion KSwapChainWrapper0
#endif

class KSwapChainWrapper0
{
public:
	typedef IDXGISwapChain SwapChainType;
	typedef IDXGIAdapter AdapterType;
	typedef IDXGIOutput OutputType;
};