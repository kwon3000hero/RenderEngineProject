#pragma once
#include <wrl.h>
#include <d3dcompiler.h>
#include "KGameDevice.h"
#include "KGameReference.h"

class KRenderTarget;
class KGameDeviceManager final : public KGameReference
{

private:
	static Microsoft::WRL::ComPtr<ID3D11Device> m_pMainDevice;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pMainContext;
	static KGameDevice<DeviceBuildVersion>* m_pMainGameDevice;

public:
	static Microsoft::WRL::ComPtr<ID3D11Device> MainDevice();
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> MainContext();
	static KGameDevice<DeviceBuildVersion>* MainGameDevice();

	static void SetMainDevice(Microsoft::WRL::ComPtr<ID3D11Device> _device);
	static void SetMainContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _Context);
	static void SetMainGameDevice(KGameDevice<DeviceBuildVersion>* _pDevice);

	static void SetMainRenderTarget();


private:
	static std::map<KGameString, KPTR<KGameDevice<DeviceBuildVersion>>> m_deviceContainer;

public:
	static KPTR<KGameDevice<DeviceBuildVersion>> Find(const KGameString& _DeviceName)
	{
		if (m_deviceContainer.end() == m_deviceContainer.find(_DeviceName))
		{
			return nullptr;
		}

		return m_deviceContainer[_DeviceName];
	}

	static void Create(const KGameString& _WindowName, KVector _ClearColor)
	{
		if (nullptr != Find(_WindowName).get())
		{
			assert(false);
		}

		/*KPTR<KGameDevice<DeviceBuildVersion>> NewDevice = new KGameDevice<DeviceBuildVersion>();
		NewDevice->Create(KGameWindow::FindWin(_WindowName), _ClearColor);

		m_deviceContainer.insert(
			std::map<KGameString, KPTR<KGameDevice<DeviceBuildVersion>>>::value_type(_WindowName, NewDevice));*/
	}
};