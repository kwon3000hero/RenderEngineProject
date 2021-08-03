#pragma once
#include <wrl.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include "KGameDevice.h"
#include "KGameReference.h"

class KRenderTarget;
//template<typename T>
class KGameDeviceManager final : public KGameReference
{
private:
	static Microsoft::WRL::ComPtr<ID3D11Device> m_pMainDevice;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pMainContext;
	static KGameDevice* m_pMainGameDevice;

public:
	static Microsoft::WRL::ComPtr<ID3D11Device> MainDevice()
	{
		return m_pMainDevice;
	}

	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> MainContext()
	{
		return m_pMainContext;
	}
	static KGameDevice* MainGameDevice()
	{
		return m_pMainGameDevice;
	}



	static void SetMainRenderTarget()
	{
		//m_pMainGameDevice->MainTarget->Clear();
		//m_pMainGameDevice->MainTarget->Setting();
	}


private:
	static std::map<KGameString, KPTR<KGameDevice>> m_deviceContainer;

public:
	static KPTR<KGameDevice> Find(const KGameString& _DeviceName)
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

		KPTR<KGameDevice> NewDevice = new KGameDevice();
		NewDevice->Create(KGameWindow::FindWin(_WindowName), _ClearColor);

		m_deviceContainer.insert(std::map<KGameString, KPTR<KGameDevice>>::value_type(_WindowName, NewDevice));
	}
};