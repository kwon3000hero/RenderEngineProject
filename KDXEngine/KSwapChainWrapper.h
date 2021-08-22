#pragma once
#include "BuildTarget.h"

//타겟 OS에 따라 SwapChain 버전을 나눈다.
#if SwapChainVersion == SwapChainWrapper6                   
#include <dxgi1_6.h>
class KSwapChainWrapper6
{
public:
	typedef IDXGIFactory7 FactoryType;
	typedef IDXGISwapChain4 SwapChainType;
	typedef IDXGIAdapter3 AdapterType;
	typedef IDXGIOutput5 OutputType;

	const static UUID uuid;

	KSwapChainWrapper6() = delete;
};
#endif

#if SwapChainVersion == SwapChainWrapper5  
#include <dxgi1_5.h>

class KSwapChainWrapper5
{
public:
	typedef IDXGIFactory5 FactoryType;
	typedef IDXGISwapChain4 SwapChainType;
	typedef IDXGIAdapter3 AdapterType;
	typedef IDXGIOutput5 OutputType;

	KSwapChainWrapper5() = delete;
};
#endif

#if SwapChainVersion == SwapChainWrapper0  
#include <dxgi.h>

class KSwapChainWrapper0
{
public:
	typedef IDXGIFactory FactoryType;
	typedef IDXGISwapChain SwapChainType;
	typedef IDXGIAdapter AdapterType;
	typedef IDXGIOutput OutputType;

	KSwapChainWrapper0() = delete;
};
#endif

#pragma comment(lib, "dxgi.lib")