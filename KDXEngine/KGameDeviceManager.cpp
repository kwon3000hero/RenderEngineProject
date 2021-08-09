#pragma once

#include "KGameDeviceManager.h"
#include "KRenderTarget.h"

Microsoft::WRL::ComPtr<ID3D11Device> KGameDeviceManager::m_pMainDevice = nullptr;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> KGameDeviceManager::m_pMainContext = nullptr;

KGameDevice<DeviceBuildVersion>* KGameDeviceManager::m_pMainGameDevice = nullptr;

std::map<KGameString, KPTR<KGameDevice<DeviceBuildVersion>>> KGameDeviceManager::m_deviceContainer;

Microsoft::WRL::ComPtr<ID3D11Device> KGameDeviceManager::MainDevice()
{
	return m_pMainDevice;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> KGameDeviceManager::MainContext()
{
	return m_pMainContext;
}
KGameDevice<DeviceBuildVersion>* KGameDeviceManager::MainGameDevice()
{
	return m_pMainGameDevice;
}

void KGameDeviceManager::SetMainDevice(Microsoft::WRL::ComPtr<ID3D11Device> _device)
{
	m_pMainDevice = _device;
}

void KGameDeviceManager::SetMainContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> _Context)
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