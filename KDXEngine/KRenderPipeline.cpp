#include "KRenderPipeline.h"
#include "KRasterizer.h"
#include "KDEPTHSTENCIL.h"
#include "KVertexShader.h"
#include "KPixelShader.h"
#include "KBlend.h"


KPTR<KVertexShader> KRenderPipeline::m_pCurrentVertexShader = static_cast<KPTR<KVertexShader>>(0);
KPTR<KPixelShader> KRenderPipeline::m_pCurrentPixelShader = static_cast<KPTR<KPixelShader>>(0);
KPTR<KRasterizer> KRenderPipeline::m_pCurrentRasterizer = static_cast<KPTR<KRasterizer>>(0);
KPTR<KDepthStencil> KRenderPipeline::m_pCurrentDepthStencil = static_cast<KPTR<KDepthStencil>>(0);
KPTR<KBlend> KRenderPipeline::m_pCurrentBlend = static_cast<KPTR<KBlend>>(0);

void KRenderPipeline::Reset()
{
	m_pCurrentRasterizer = static_cast<KPTR<KRasterizer>>(0);
	m_pCurrentDepthStencil = static_cast<KPTR<KDepthStencil>>(0);
	m_pCurrentBlend = static_cast<KPTR<KBlend>>(0);
	m_pCurrentVertexShader = static_cast<KPTR<KVertexShader>>(0);
	m_pCurrentPixelShader = static_cast<KPTR<KPixelShader>>(0);
	KGameDeviceManager::MainContext()->RSSetState(nullptr);
	KGameDeviceManager::MainContext()->OMSetDepthStencilState(nullptr, 0);
	KGameDeviceManager::MainContext()->OMSetBlendState(nullptr, nullptr, 0xfffffff);
}

void KRenderPipeline::ResetAll()
{
	KGameDeviceManager::MainContext()->VSSetShader(nullptr, nullptr, 0);
	KGameDeviceManager::MainContext()->GSSetShader(nullptr, nullptr, 0);
	KGameDeviceManager::MainContext()->HSSetShader(nullptr, nullptr, 0);
	KGameDeviceManager::MainContext()->DSSetShader(nullptr, nullptr, 0);
	KGameDeviceManager::MainContext()->PSSetShader(nullptr, nullptr, 0);

	ID3D11RenderTargetView* pArrView[8] = { nullptr, };
	KGameDeviceManager::MainContext()->OMSetRenderTargets(8, pArrView, nullptr);

	ID3D11ShaderResourceView* pArrSRView = nullptr;
	ID3D11SamplerState* pArrSampler = nullptr;
	for (unsigned int i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++)
	{
		KGameDeviceManager::MainContext()->VSSetShaderResources(i, 1, &pArrSRView);
		KGameDeviceManager::MainContext()->HSSetShaderResources(i, 1, &pArrSRView);
		KGameDeviceManager::MainContext()->GSSetShaderResources(i, 1, &pArrSRView);
		KGameDeviceManager::MainContext()->DSSetShaderResources(i, 1, &pArrSRView);
		KGameDeviceManager::MainContext()->PSSetShaderResources(i, 1, &pArrSRView);
		KGameDeviceManager::MainContext()->VSSetSamplers(i, 1, &pArrSampler);
		KGameDeviceManager::MainContext()->HSSetSamplers(i, 1, &pArrSampler);
		KGameDeviceManager::MainContext()->GSSetSamplers(i, 1, &pArrSampler);
		KGameDeviceManager::MainContext()->DSSetSamplers(i, 1, &pArrSampler);
		KGameDeviceManager::MainContext()->PSSetSamplers(i, 1, &pArrSampler);
	}

	Reset();
}

void KRenderPipeline::SetVertexShader(const KGameString& _Name)
{
	m_vertexShader = KGameResourceManager<KVertexShader>::Instance().Find(_Name);

	if (nullptr == m_vertexShader.get())
	{
		AssertMsg(L"버텍스 쉐이더가 존재하지 않습니다. = " + _Name);
	}

	m_shaderContainer.push_back(m_vertexShader.get());

}
void KRenderPipeline::SetPixelShader(const KGameString& _Name)
{
	m_pixelShader = KGameResourceManager<KPixelShader>::Instance().Find(_Name);

	if (nullptr == m_pixelShader.get())
	{
		AssertMsg(L"픽셀 쉐이더가 존재하지 않습니다. = " + _Name);
	}

	m_shaderContainer.push_back(m_pixelShader.get());
}



void KRenderPipeline::SetRasterizer(const KGameString& _Name)
{
	m_rasterizer = KGameResourceManager<KRasterizer>::Instance().Find(_Name);

	if (nullptr == m_rasterizer.get())
	{
		AssertMsg(L"레스터 라이저가 존재하지 않습니다. = " + _Name);
	}

}

void KRenderPipeline::SetDepthStencil(const KGameString& _Name)
{
	m_depthStencil = KGameResourceManager<KDepthStencil>::Instance().Find(_Name);

	if (nullptr == m_depthStencil.get())
	{
		AssertMsg(L"뎁스스텐실 설정이 존재하지 않습니다. = " + _Name);
	}

}
void KRenderPipeline::SetBlend(const KGameString& _Name)
{
	m_blend = KGameResourceManager<KBlend>::Instance().Find(_Name);

	if (nullptr == m_blend.get())
	{
		AssertMsg(L"블랜드가 존재하지 않습니다. = " + _Name);
	}

}

KPTR<KPixelShader> KRenderPipeline::PixelShader()
{
	return m_pixelShader;
}



void KRenderPipeline::Create() {}
void KRenderPipeline::Setting()
{
	if (nullptr != m_vertexShader.get() && m_vertexShader.get() != m_pCurrentVertexShader.get())
	{
		m_vertexShader->Setting();
		m_pCurrentVertexShader = m_vertexShader;
	}

	if (nullptr != m_pixelShader.get() && m_pixelShader.get() != m_pCurrentPixelShader.get())
	{
		m_pixelShader->Setting();
		m_pCurrentPixelShader = m_pixelShader;
	}


	if (nullptr != m_rasterizer.get() && m_rasterizer.get() != m_pCurrentRasterizer.get())
	{
		m_rasterizer->Setting();
		m_pCurrentRasterizer = m_rasterizer;
	}

	if (nullptr != m_depthStencil.get() && m_depthStencil.get() != m_pCurrentDepthStencil.get())
	{
		m_depthStencil->Setting();
		m_pCurrentDepthStencil = m_depthStencil;
	}

	if (nullptr != m_blend.get() && m_blend.get() != m_pCurrentBlend.get())
	{
		m_blend->Setting();
		m_pCurrentBlend = m_blend;
	}
}