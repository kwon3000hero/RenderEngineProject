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
	if (S_OK != KGameDevice::MainDevice()->CreateBlendState(&m_Desc, &m_STATE))
	{
		assert(false);
	}
	return;
}

void KBlend::Setting()
{
	KGameDevice::MainContext()->OMSetBlendState(m_STATE, nullptr, BlendMask);
}