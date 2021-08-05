#include "KBlend.h"

KBlend::KBlend() : m_STATE(nullptr), BlendMask(0xffffffff)
{
}
KBlend::~KBlend()
{
	if (nullptr != m_STATE)
	{
		m_STATE->Release();
		m_STATE = nullptr;
	}
}

void KBlend::Create(const D3D11_BLEND_DESC& _Desc)
{
	if (S_OK != KGameDeviceManager::MainDevice()->CreateBlendState(&m_Desc, &m_STATE))
	{
		assert(false);
	}
	return;
}

void KBlend::Setting()
{
	KGameDeviceManager::MainContext()->OMSetBlendState(m_STATE, nullptr, BlendMask);
}