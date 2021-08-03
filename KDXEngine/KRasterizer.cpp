#include "KRasterizer.h"

KRasterizer::KRasterizer() : m_OPTION(nullptr)
{
}

KRasterizer::~KRasterizer()
{
	if (nullptr != m_OPTION)
	{
		m_OPTION->Release();
		m_OPTION = nullptr;
	}
}

// D3D11_RASTERIZER_DESC ������ �������� ����� ���� ����

void KRasterizer::Create(const D3D11_RASTERIZER_DESC& _Desc) 
{
	if (S_OK != KGameDeviceManager::MainDevice()->CreateRasterizerState(&m_Desc, &m_OPTION))
	{
		assert(false);
	}

	return;
}


void KRasterizer::Setting()
{
	KGameDeviceManager::MainContext()->RSSetState(m_OPTION);
}