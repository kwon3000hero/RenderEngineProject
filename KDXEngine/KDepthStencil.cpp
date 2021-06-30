#include "KDepthStencil.h"

KDepthStencil::KDepthStencil() : m_OPTION(nullptr)
{
}

KDepthStencil::~KDepthStencil()
{
	if (nullptr != m_OPTION)
	{
		m_OPTION->Release();
		m_OPTION = nullptr;
	}
}

void KDepthStencil::Create(const D3D11_DEPTH_STENCIL_DESC& _Desc)
{
	if (S_OK != KGameDevice::MainDevice()->CreateDepthStencilState(&m_Desc, &m_OPTION))
	{
		assert(false);
	}

	return;
}


void KDepthStencil::Setting()
{
	KGameDevice::MainContext()->OMSetDepthStencilState(m_OPTION, 0);
}