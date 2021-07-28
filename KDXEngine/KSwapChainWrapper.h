#pragma once
#include <dxgi.h>

#pragma comment(lib, "dxguid")

//타겟 OS에 따라 SwapChain 버전을 나눈다.
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