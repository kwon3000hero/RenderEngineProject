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

// D3D11_RASTERIZER_DESC 레스터 라이저를 만들기 위한 정보

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