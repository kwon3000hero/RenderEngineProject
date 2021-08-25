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
	static Microsoft::WRL::ComPtr<DeviceSelectedWrapper::DeviceType> m_pMainDevice;
	static Microsoft::WRL::ComPtr<DeviceSelectedWrapper::ContextType> m_pMainContext;
	static KGameDevice<DeviceSelectedWrapper>* m_pMainGameDevice;

private:
	static std::map<KGameString, KPTR<KGameDevice<DeviceSelectedWrapper>>> m_deviceContainer;

public:
	static Microsoft::WRL::ComPtr<DeviceSelectedWrapper::DeviceType> MainDevice();
	static Microsoft::WRL::ComPtr<DeviceSelectedWrapper::ContextType> MainContext();
	static KGameDevice<DeviceSelectedWrapper>* MainGameDevice();

	static void SetMainDevice(Microsoft::WRL::ComPtr<DeviceSelectedWrapper::DeviceType> _device);
	static void SetMainContext(Microsoft::WRL::ComPtr<DeviceSelectedWrapper::ContextType> _Context);
	static void SetMainGameDevice(KGameDevice<DeviceSelectedWrapper>* _pDevice);

	static void SetMainRenderTarget();


public:
	static KPTR<KGameDevice<DeviceSelectedWrapper>> Find(const KGameString& _DeviceName);

	static void Create(const KGameString& _WindowName, KVector _ClearColor);	
};