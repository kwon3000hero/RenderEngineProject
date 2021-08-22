#pragma once

#include "KGameDeviceManager.h"
#include "KRenderTarget.h"
#include "KGameDevice.h"

Microsoft::WRL::ComPtr<DeviceSelectedWrapper::DeviceType> KGameDeviceManager::m_pMainDevice = nullptr;
Microsoft::WRL::ComPtr<DeviceSelectedWrapper::ContextType> KGameDeviceManager::m_pMainContext = nullptr;

KGameDevice<DeviceSelectedWrapper>* KGameDeviceManager::m_pMainGameDevice = nullptr;

std::map<KGameString, KPTR<KGameDevice<DeviceSelectedWrapper>>> KGameDeviceManager::m_deviceContainer;

Microsoft::WRL::ComPtr<DeviceSelectedWrapper::DeviceType> KGameDeviceManager::MainDevice()
{
	return m_pMainDevice;
}

Microsoft::WRL::ComPtr<DeviceSelectedWrapper::ContextType> KGameDeviceManager::MainContext()
{
	return m_pMainContext;
}
KGameDevice<DeviceSelectedWrapper>* KGameDeviceManager::MainGameDevice()
{
	return m_pMainGameDevice;
}

void KGameDeviceManager::SetMainDevice(Microsoft::WRL::ComPtr<DeviceSelectedWrapper::DeviceType> _device)
{
	m_pMainDevice = _device;
}

void KGameDeviceManager::SetMainContext(Microsoft::WRL::ComPtr<DeviceSelectedWrapper::ContextType> _Context)
{
	m_pMainContext = _Context;
}

void KGameDeviceManager::SetMainGameDevice(KGameDevice<DeviceSelectedWrapper>* _pDevice)
{
	m_pMainGameDevice = _pDevice;
}


void KGameDeviceManager::SetMainRenderTarget()
{
	m_pMainGameDevice->MainTarget->Clear();
	m_pMainGameDevice->MainTarget->Setting();
}

KPTR<KGameDevice<DeviceSelectedWrapper>> KGameDeviceManager::Find(const KGameString& _DeviceName)
{
	if (m_deviceContainer.end() == m_deviceContainer.find(_DeviceName))
	{
		return nullptr;
	}

	return m_deviceContainer[_DeviceName];
}

void KGameDeviceManager::Create(const KGameString& _WindowName, KVector _ClearColor)
{
	if (nullptr != Find(_WindowName).get())
	{
		assert(false);
	}

	KPTR<KGameDevice<DeviceSelectedWrapper>> device = new KGameDevice<DeviceSelectedWrapper>();
	device->Create(KGameWindow::FindWin(_WindowName), _ClearColor);

	m_deviceContainer.insert(
		std::map<KGameString, KPTR<KGameDevice<DeviceSelectedWrapper>>>::value_type(_WindowName, device));
}