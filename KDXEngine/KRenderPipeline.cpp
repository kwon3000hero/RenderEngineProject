#include "KRenderPipeline.h"
#include "KRasterizer.h"
#include "KDEPTHSTENCIL.h"
#include "KVertexShader.h"
#include "KPixelShader.h"
#include "KBlend.h"


KPTR<KVertexShader> KRenderPipeline::m_pCurrentVertexShader;
KPTR<KPixelShader> KRenderPipeline::m_pCurrentPixelShader;
KPTR<KRasterizer> KRenderPipeline::m_pCurrentRasterizer;
KPTR<KDepthStencil> KRenderPipeline::m_pCurrentDepthStencil;
KPTR<KBlend> KRenderPipeline::m_pCurrentBlend;

void KRenderPipeline::Reset()
{
	m_pCurrentRasterizer = nullptr;
	m_pCurrentDepthStencil = nullptr;
	m_pCurrentBlend = nullptr;
	m_pCurrentVertexShader = nullptr;
	m_pCurrentPixelShader = nullptr;
	KGameDevice::MainContext()->RSSetState(nullptr);
	KGameDevice::MainContext()->OMSetDepthStencilState(nullptr, 0);
	KGameDevice::MainContext()->OMSetBlendState(nullptr, nullptr, 0xfffffff);
}

void KRenderPipeline::ResetAll()
{
	KGameDevice::MainContext()->VSSetShader(nullptr, nullptr, 0);
	KGameDevice::MainContext()->GSSetShader(nullptr, nullptr, 0);
	KGameDevice::MainContext()->HSSetShader(nullptr, nullptr, 0);
	KGameDevice::MainContext()->DSSetShader(nullptr, nullptr, 0);
	KGameDevice::MainContext()->PSSetShader(nullptr, nullptr, 0);

	ID3D11RenderTargetView* pArrView[8] = { nullptr, };
	KGameDevice::MainContext()->OMSetRenderTargets(8, pArrView, nullptr);

	ID3D11ShaderResourceView* pArrSRView = nullptr;
	ID3D11SamplerState* pArrSampler = nullptr;
	for (unsigned int i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++)
	{
		KGameDevice::MainContext()->VSSetShaderResources(i, 1, &pArrSRView);
		KGameDevice::MainContext()->HSSetShaderResources(i, 1, &pArrSRView);
		KGameDevice::MainContext()->GSSetShaderResources(i, 1, &pArrSRView);
		KGameDevice::MainContext()->DSSetShaderResources(i, 1, &pArrSRView);
		KGameDevice::MainContext()->PSSetShaderResources(i, 1, &pArrSRView);
		KGameDevice::MainContext()->VSSetSamplers(i, 1, &pArrSampler);
		KGameDevice::MainContext()->HSSetSamplers(i, 1, &pArrSampler);
		KGameDevice::MainContext()->GSSetSamplers(i, 1, &pArrSampler);
		KGameDevice::MainContext()->DSSetSamplers(i, 1, &pArrSampler);
		KGameDevice::MainContext()->PSSetSamplers(i, 1, &pArrSampler);
	}

	Reset();
}

void KRenderPipeline::SetVertexShader(const KGameString& _Name)
{
	m_vertexShader = KVertexShader::Find(_Name);

	if (nullptr == m_vertexShader.get())
	{
		AssertMsg(L"버텍스 쉐이더가 존재하지 않습니다. = " + _Name);
	}

	m_shaderContainer.push_back(m_vertexShader);

}
void KRenderPipeline::SetPixelShader(const KGameString& _Name)
{
	m_pixelShader = KPixelShader::Find(_Name);

	if (nullptr == m_pixelShader.get())
	{
		AssertMsg(L"픽셀 쉐이더가 존재하지 않습니다. = " + _Name);
	}

	m_shaderContainer.push_back(m_pixelShader.get());
}



void KRenderPipeline::SetRasterizer(const KGameString& _Name)
{
	m_rasterizer = KRasterizer::Find(_Name);

	if (nullptr == m_rasterizer.get())
	{
		AssertMsg(L"레스터 라이저가 존재하지 않습니다. = " + _Name);
	}

}

void KRenderPipeline::SetDepthStencil(const KGameString& _Name)
{
	m_depthStencil = KDepthStencil::Find(_Name);

	if (nullptr == m_depthStencil.get())
	{
		AssertMsg(L"뎁스스텐실 설정이 존재하지 않습니다. = " + _Name);
	}

}
void KRenderPipeline::SetBlend(const KGameString& _Name)
{
	m_blend = KBlend::Find(_Name);

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
	if (nullptr != m_vertexShader.get() && m_vertexShader != m_pCurrentVertexShader)
	{
		m_vertexShader->Setting();
		m_pCurrentVertexShader = m_vertexShader;
	}

	if (nullptr != m_pixelShader.get() && m_pixelShader != m_pCurrentPixelShader)
	{
		m_pixelShader->Setting();
		m_pCurrentPixelShader = m_pixelShader;
	}


	if (nullptr != m_rasterizer.get() && m_rasterizer != m_pCurrentRasterizer)
	{
		m_rasterizer->Setting();
		m_pCurrentRasterizer = m_rasterizer;
	}

	if (nullptr != m_depthStencil.get() && m_depthStencil != m_pCurrentDepthStencil)
	{
		m_depthStencil->Setting();
		m_pCurrentDepthStencil = m_depthStencil;
	}

	if (nullptr != m_blend.get() && m_blend != m_pCurrentBlend)
	{
		m_blend->Setting();
		m_pCurrentBlend = m_blend;
	}
}