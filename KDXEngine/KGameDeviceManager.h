#pragma once
#include <wrl.h>
#include <d3dcompiler.h>
#include "KGameReference.h"
#include "BuildTarget.h"
#include <map>

template<typename T>
class KGameDevice;
class KRenderTarget;
class KVector;
class KGameDeviceManager final : public KGameReference
{

private:
	static Microsoft::WRL::ComPtr<DeviceBuildVersion::DeviceType> m_pMainDevice;
	static Microsoft::WRL::ComPtr<DeviceBuildVersion::ContextType> m_pMainContext;
	static KGameDevice<DeviceBuildVersion>* m_pMainGameDevice;

private:
	static std::map<KGameString, KPTR<KGameDevice<DeviceBuildVersion>>> m_deviceContainer;

public:
	static Microsoft::WRL::ComPtr<DeviceBuildVersion::DeviceType> MainDevice();
	static Microsoft::WRL::ComPtr<DeviceBuildVersion::ContextType> MainContext();
	static KGameDevice<DeviceBuildVersion>* MainGameDevice();

	static void SetMainDevice(Microsoft::WRL::ComPtr<DeviceBuildVersion::DeviceType> _device);
	static void SetMainContext(Microsoft::WRL::ComPtr<DeviceBuildVersion::ContextType> _Context);
	static void SetMainGameDevice(KGameDevice<DeviceBuildVersion>* _pDevice);

	static void SetMainRenderTarget();


public:
	static KPTR<KGameDevice<DeviceBuildVersion>> Find(const KGameString& _DeviceName);

	static void Create(const KGameString& _WindowName, KVector _ClearColor);	
};