#pragma once

#include "KGameDeviceManager.h"

Microsoft::WRL::ComPtr<ID3D11Device> KGameDeviceManager::m_pMainDevice = nullptr;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> KGameDeviceManager::m_pMainContext = nullptr;

KGameDevice* KGameDeviceManager::m_pMainGameDevice = nullptr;

std::map<KGameString, KPTR<KGameDevice>> KGameDeviceManager::m_deviceContainer;
