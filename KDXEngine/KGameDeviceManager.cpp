#pragma once

#include "KGameDeviceManager.h"
#include "KRenderTarget.h"
#include "KGameDevice.h"

Microsoft::WRL::ComPtr<DeviceBuildVersion::DeviceType> KGameDeviceManager::m_pMainDevice = nullptr;
Microsoft::WRL::ComPtr<DeviceBuildVersion::ContextType> KGameDeviceManager::m_pMainContext = nullptr;

KGameDevice<DeviceBuildVersion>* KGameDeviceManager::m_pMainGameDevice = nullptr;

std::map<KGameString, KPTR<KGameDevice<DeviceBuildVersion>>> KGameDeviceManager::m_deviceContainer;

Microsoft::WRL::ComPtr<DeviceBuildVersion::DeviceType> KGameDeviceManager::MainDevice()
{
	return m_pMainDevice;
}

Microsoft::WRL::ComPtr<DeviceBuildVersion::ContextType> KGameDeviceManager::MainContext()
{
	return m_pMainContext;
}
KGameDevice<DeviceBuildVersion>* KGameDeviceManager::MainGameDevice()
{
	return m_pMainGameDevice;
}

void KGameDeviceManager::SetMainDevice(Microsoft::WRL::ComPtr<DeviceBuildVersion::DeviceType> _device)
{
	m_pMainDevice = _device;
}

void KGameDeviceManager::SetMainContext(Microsoft::WRL::ComPtr<DeviceBuildVersion::ContextType> _Context)
{
	m_pMainContext = _Context;
}

void KGameDeviceManager::SetMainGameDevice(KGameDevice<DeviceBuildVersion>* _pDevice)
{
	m_pMainGameDevice = _pDevice;
}


void KGameDeviceManager::SetMainRenderTarget()
{
	m_pMainGameDevice->MainTarget->Clear();
	m_pMainGameDevice->MainTarget->Setting();
}

KPTR<KGameDevice<DeviceBuildVersion>> KGameDeviceManager::Find(const KGameString& _DeviceName)
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

	KPTR<KGameDevice<DeviceBuildVersion>> device = new KGameDevice<DeviceBuildVersion>();
	device->Create(KGameWindow::FindWin(_WindowName), _ClearColor);

	m_deviceContainer.insert(
		std::map<KGameString, KPTR<KGameDevice<DeviceBuildVersion>>>::value_type(_WindowName, device));
}