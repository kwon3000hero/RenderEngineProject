#include "KGameDevice.h"
#include "KGAMEDIR.h"
#include "KRenderPipeline.h"

void KGameDevice::InitRenderPipeline()
{
	{
		// 머지용 랜더파이스라인
		KPTR<KRenderPipeline> RPL = KRenderPipeline ::Create(L"FXAA");
		RPL->SetVertexShader(L"VS_FXAA");
		RPL->SetPixelShader(L"PS_FXAA");
		RPL->SetDepthStencil(L"ALWAYSDEPTH");
		RPL->SetRasterizer(L"BACK");
		RPL->SetBlend(L"LIGHTMERGE");
	}

	{
		// 머지용 랜더파이스라인
		KPTR<KRenderPipeline> RPL = KRenderPipeline::Create(L"LIGHTBLOOM");
		RPL->SetVertexShader(L"VS_LightBloom");
		RPL->SetPixelShader(L"PS_LightBloom");
		RPL->SetDepthStencil(L"ALWAYSDEPTH");
		RPL->SetRasterizer(L"BACK");
		RPL->SetBlend(L"LIGHTMERGE");
	}

	{
		// 머지용 랜더파이스라인
		KPTR<KRenderPipeline> RPL = KRenderPipeline::Create(L"EMISSIONBLOOM");
		RPL->SetVertexShader(L"VS_EmissionBloom");
		RPL->SetPixelShader(L"PS_EmissionBloom");
		RPL->SetDepthStencil(L"ALWAYSDEPTH");
		RPL->SetRasterizer(L"BACK");
		RPL->SetBlend(L"ALPHABLEND");
	}



	{
		// 머지용 랜더파이스라인
		KPTR<KRenderPipeline> RPL = KRenderPipeline::Create(L"EMISSIONBLOOMCHECK");
		RPL->SetVertexShader(L"VS_EmssionBloomCheck");
		RPL->SetPixelShader(L"PS_EmssionBloomCheck");
		RPL->SetDepthStencil(L"ALWAYSDEPTH");
		RPL->SetRasterizer(L"BACK");
		RPL->SetBlend(L"ALPHABLEND");
	}



	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"SHADOWANI");
		NewMat->SetVertexShader(L"VS_SHADOWANI");
		NewMat->SetPixelShader(L"PS_SHADOWANI");
		NewMat->SetDepthStencil(L"DEFDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"SHADOWBLEND");
	}

	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"SHADOW");
		NewMat->SetVertexShader(L"VS_SHADOW");
		NewMat->SetPixelShader(L"PS_SHADOW");
		NewMat->SetDepthStencil(L"DEFDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"SHADOWBLEND");
	}

	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"HMAPDEFFERD");
		NewMat->SetVertexShader(L"VS_HMAPDEFFERD");
		NewMat->SetPixelShader(L"PS_HMAPDEFFERD");
		NewMat->SetDepthStencil(L"DEFDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"ALPHABLEND");
	}


	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"HMAPFORWARD");
		NewMat->SetVertexShader(L"VS_HMAPFORWARD");
		NewMat->SetPixelShader(L"PS_HMAPFORWARD");
		NewMat->SetDepthStencil(L"DEFDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"GRID");
		NewMat->SetVertexShader(L"VS_GRID");
		NewMat->SetPixelShader(L"PS_GRID");
		NewMat->SetDepthStencil(L"DEFDEPTH");
		NewMat->SetRasterizer(L"NONE");
		NewMat->SetBlend(L"ALPHABLEND");
	}
	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"3DSKY");
		NewMat->SetVertexShader(L"VS_3DSKY");
		NewMat->SetPixelShader(L"PS_3DSKY");
		NewMat->SetDepthStencil(L"DEFDEPTH");
		NewMat->SetRasterizer(L"FRONT");
		NewMat->SetBlend(L"ALPHABLEND");
	}
	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"ALWAYSMERGETARGET");
		NewMat->SetVertexShader(L"VS_MergeTarget");
		NewMat->SetPixelShader(L"PS_MergeTarget");
		NewMat->SetDepthStencil(L"ALWAYSDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"MERGETARGET");
		NewMat->SetVertexShader(L"VS_MergeTarget");
		NewMat->SetPixelShader(L"PS_MergeTarget");
		NewMat->SetDepthStencil(L"DEFDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"DEFFERDMERGE");
		NewMat->SetVertexShader(L"VS_DEFFERDMERGE");
		NewMat->SetPixelShader(L"PS_DEFFERDMERGE");
		NewMat->SetDepthStencil(L"ALWAYSDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"DEFFERDCALCULATEDLIGHT");
		NewMat->SetVertexShader(L"VS_DEFFERDLIGHTING");
		NewMat->SetPixelShader(L"PS_DEFFERDLIGHTING");
		NewMat->SetDepthStencil(L"ALWAYSDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"LIGHTMERGE");
	}

	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"3DCOLORDEFFERD");
		NewMat->SetVertexShader(L"VS_3DCOLORDEFFERD");
		NewMat->SetPixelShader(L"PS_3DCOLORDEFFERD");
		NewMat->SetDepthStencil(L"DEFDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"3DMESHDEFFERD");
		NewMat->SetVertexShader(L"VS_3DMESHDEFFERD");
		NewMat->SetPixelShader(L"PS_3DMESHDEFFERD");
		NewMat->SetDepthStencil(L"DEFDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"3DMESHFORWARD");
		NewMat->SetVertexShader(L"VS_3DMESHFORWARD");
		NewMat->SetPixelShader(L"PS_3DMESHFORWARD");
		NewMat->SetDepthStencil(L"DEFDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> NewMat = KRenderPipeline::Create(L"2DLIGHT");
		NewMat->SetVertexShader(L"VS_2DLIGHT");
		NewMat->SetPixelShader(L"PS_2DLIGHT");
		NewMat->SetDepthStencil(L"ALWAYSDEPTH");
		NewMat->SetRasterizer(L"BACK");
		NewMat->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> RPL = KRenderPipeline::Create(L"DEBUGBACK");
		RPL->SetVertexShader(L"VS_DEBUGIMAGE");
		RPL->SetPixelShader(L"PS_DEBUGBACK");
		RPL->SetDepthStencil(L"ALWAYSDEPTH");
		RPL->SetRasterizer(L"BACK");
		RPL->SetBlend(L"ALPHABLEND");
	}


	{
		KPTR<KRenderPipeline> RPL = KRenderPipeline::Create(L"DEBUGIMAGE");
		RPL->SetVertexShader(L"VS_DEBUGIMAGE");
		RPL->SetPixelShader(L"PS_DEBUGIMAGE");
		RPL->SetDepthStencil(L"ALWAYSDEPTH");
		RPL->SetRasterizer(L"BACK");
		RPL->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> RPL = KRenderPipeline::Create(L"2DLight");
		RPL->SetVertexShader(L"VS_2DLIGHT");
		RPL->SetPixelShader(L"PS_2DLIGHT");
		RPL->SetDepthStencil(L"ALWAYSDEPTH");
		RPL->SetRasterizer(L"BACK");
		RPL->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> RPL = KRenderPipeline::Create(L"TileMap");
		RPL->SetVertexShader(L"VS_TILEMAP");
		RPL->SetPixelShader(L"PS_TILEMAP");
		RPL->SetDepthStencil(L"DEFDEPTH");
		RPL->SetRasterizer(L"BACK");
		RPL->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> RPL = KRenderPipeline::Create(L"COLOR");
		RPL->SetVertexShader(L"VS_COLOR");
		RPL->SetPixelShader(L"PS_COLOR");
		RPL->SetDepthStencil(L"DEFDEPTH");
		RPL->SetRasterizer(L"BACK");
		RPL->SetBlend(L"ALPHABLEND");
	}

	{
		KPTR<KRenderPipeline> RPL = KRenderPipeline::Create(L"HDEBUG");
		RPL->SetVertexShader(L"VS_DEBUGFI");
		RPL->SetPixelShader(L"PS_DEBUGFI");
		RPL->SetDepthStencil(L"DEFDEPTH");
		RPL->SetRasterizer(L"BACK");
		RPL->SetBlend(L"ALPHABLEND");
	}

}