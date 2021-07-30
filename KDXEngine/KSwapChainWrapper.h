#pragma once

//타겟 OS에 따라 SwapChain 버전을 나눈다.
#if WDK_NTDDI_VERSION >= NTDDI_WIN10_19H1                    
#include <dxgi1_6.h>

class KSwapChainWrapper6
{
public:
	typedef IDXGIFactory7 FactoryType;
	typedef IDXGISwapChain4 SwapChainType;
	typedef IDXGIAdapter3 AdapterType;
	typedef IDXGIOutput5 OutputType;
};
#endif

#if WDK_NTDDI_VERSION >= NTDDI_WIN10_RS5
#include <dxgi1_5.h>

class KSwapChainWrapper5
{
public:
	typedef IDXGIFactory5 FactoryType;
	typedef IDXGISwapChain4 SwapChainType;
	typedef IDXGIAdapter3 AdapterType;
	typedef IDXGIOutput5 OutputType;
};
#endif

#if WDK_NTDDI_VERSION >= NTDDI_WIN10
#include <dxgi.h>

class KSwapChainWrapper0
{
public:
	typedef IDXGISwapChain SwapChainType;
	typedef IDXGIAdapter AdapterType;
	typedef IDXGIOutput OutputType;
};
#endif

#if WDK_NTDDI_VERSION >= NTDDI_WIN10_19H1
#define SwapChainBuildVersion KSwapChainWrapper6
#elif WDK_NTDDI_VERSION >= NTDDI_WIN10_RS5
#define SwapChainBuildVersion KSwapChainWrapper5
#elif WDK_NTDDI_VERSION >= NTDDI_WIN10
#define SwapChainBuildVersion KSwapChainWrapper0
#endif

#pragma comment(lib, "dxgi.lib")