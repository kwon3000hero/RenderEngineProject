#pragma once
#include "KDeviceWrapper.h"
#include "KSwapChainWrapper.h"

#define DeviceWrapperVer5 0x0005
#define DeviceWrapperVer4 0x0004
#define DeviceWrapperVer3 0x0003
#define DeviceWrapperVer2 0x0002
#define DeviceWrapperVer1 0x0001
#define DeviceWrapperVer0 0x0000

#define SwapChainWrapper6 0x0016
#define SwapChainWrapper5 0x0015
#define SwapChainWrapper4 0x0014
#define SwapChainWrapper3 0x0013
#define SwapChainWrapper2 0x0012
#define SwapChainWrapper1 0x0011
#define SwapChainWrapper0 0x0010


#if WDK_NTDDI_VERSION >= NTDDI_WIN10_19H1
#define DeviceSelectedWrapper KDeviceWrapper5
#define SwapChainSelectedWrapper KSwapChainWrapper6
#define DeviceVersion DeviceWrapperVer5
#define SwapChainVersion SwapChainWrapper6
#elif WDK_NTDDI_VERSION >= NTDDI_WIN10_RS5
#define DeviceBuildVersion KDeviceWrapper5
#define SwapChainBuildVersion KSwapChainWrapper5
#define DeviceVersion DeviceWrapperVer5
#define SwapChainVersion SwapChainWrapper5
#elif WDK_NTDDI_VERSION >= NTDDI_WIN10
#define DeviceBuildVersion KDeviceWrapper0
#define SwapChainBuildVersion KSwapChainWrapper0
#define DeviceVersion DeviceWrapperVer0
#define SwapChainVersion SwapChainWrapper0
#endif
 