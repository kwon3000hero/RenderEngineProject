#pragma once
#include "BuildTarget.h"

/*#if WDK_NTDDI_VERSION >= NTDDI_WIN10_19H1
* #define DeviceBuildVersion ID3D11Device6
#define SwapChainBuildVersion KSwapChainWrapper6
#elif WDK_NTDDI_VERSION >= NTDDI_WIN10_RS5
#define DeviceBuildVersion ID3D11Device5
#define SwapChainBuildVersion KSwapChainWrapper5
#el*/
#if WDK_NTDDI_VERSION >= NTDDI_WIN10
#define DeviceBuildVersion ID3D11Device
#define SwapChainBuildVersion KSwapChainWrapper0
#endif
 