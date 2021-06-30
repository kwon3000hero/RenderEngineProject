#pragma once
#include "KGameResource.h"
#include "K3DBufferBase.h"
#include "KShader.h"


class KRasterizer;
class KDepthStencil;
class KBlend;
class KVertexShader;
class KPixelShader;
class KRenderPipeline : public KGameResource<KRenderPipeline>
{
private:
	static KVertexShader* m_pCurrentVertexShader;
	static KPixelShader* m_pCurrentPixelShader;

	static KRasterizer* m_pCurrentRasterizer;
	static KDepthStencil* m_pCurrentDepthStencil;
	static KBlend* m_pCurrentBlend;

public:
	static void Reset();
	static void ResetAll();

private:
	std::vector<KShader*> m_shaderContainer;
	KPTR<KVertexShader> m_vertexShader;
	KPTR<KPixelShader> m_pixelShader;

private:
	KPTR<KRasterizer>	m_rasterizer;
	KPTR<KDepthStencil> m_depthStencil;
	KPTR<KBlend>		m_blend;


public:
	void SetVertexShader(const KGameString& _Name);
	void SetPixelShader(const KGameString& _Name);
	void SetRasterizer(const KGameString& _Name);
	void SetDepthStencil(const KGameString& _Name);
	void SetBlend(const KGameString& _Name);

	KPTR<KPixelShader> PixelShader();

public:
	void Setting();


public:
	static KPTR<KRenderPipeline> Create(const KGameString& _Name)
	{
		KRenderPipeline* pNewRes = new KRenderPipeline();
		pNewRes->SetName(_Name);
		pNewRes->InsertResource();

		return pNewRes;
	}

public:
	void Create();

public:
	const std::vector<KShader*>& ShaderContainer() const
	{
		return m_shaderContainer;
	}

};

