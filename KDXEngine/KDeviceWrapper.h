#pragma once
#include "BuildTarget.h"

#if WDK_NTDDI_VERSION >= NTDDI_WIN10_19H1                    

#include <d3d11_4.h>

class KDeviceWrapper5;
#endif

#if WDK_NTDDI_VERSION >= NTDDI_WIN10_RS5
#include <d3d11_4.h>

class KDeviceWrapper5
{
public:
	typedef ID3D11Device5 DeviceType;
	typedef ID3D11DeviceContext4 ContextType;
};
#endif

#if WDK_NTDDI_VERSION >= NTDDI_WIN10
#include <dxgi.h>

class KDeviceWrapper0
{
public:
	typedef ID3D11Device DeviceType;
	typedef ID3D11DeviceContext ContextType;
};
#endif

#pragma comment(lib, "dxgi.lib")

ID3D11Device