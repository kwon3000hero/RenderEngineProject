#include "KGameDevice.h"
#include "KBlend.h"

void KGameDevice::InitBlendState() {

	{
		D3D11_BLEND_DESC Data;
		ZeroMemory(&Data, sizeof(Data));
		Data.AlphaToCoverageEnable = false;
		
		Data.IndependentBlendEnable = false;
				
		ZeroMemory(&Data.RenderTarget[0], sizeof(Data.RenderTarget[0]));		

		Data.RenderTarget[0].BlendEnable = true;
		Data.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // + - 
		Data.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // + - 
		Data.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // + - 
		Data.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // + -
		Data.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // + - 
		Data.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // + - 
		Data.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; // + - 

		KBlend::Create("ALPHABLEND", Data);
	}
	{
		D3D11_BLEND_DESC Data;
		ZeroMemory(&Data, sizeof(Data));
		Data.AlphaToCoverageEnable = false; 
		Data.IndependentBlendEnable = false;
		
		ZeroMemory(&Data.RenderTarget[0], sizeof(Data.RenderTarget[0]));
		
		Data.RenderTarget[0].BlendEnable = true;
		Data.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // + - 
		Data.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // + - 
		Data.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; // + - 
		Data.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO; // + -
		Data.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // + - 
		Data.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // + - 
		Data.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // + - 

		KBlend::Create("SHADOWBLEND", Data);
	}

	{
		D3D11_BLEND_DESC Data;
		ZeroMemory(&Data, sizeof(Data));
		Data.AlphaToCoverageEnable = false;
		Data.IndependentBlendEnable = false;

		ZeroMemory(&Data.RenderTarget[0], sizeof(Data.RenderTarget[0]));

		Data.RenderTarget[0].BlendEnable = true;
		Data.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // + - 
		Data.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // + - 
		Data.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; // + - 
		Data.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; // + -
		Data.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // + - 
		Data.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // + - 
		Data.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; // + - 

		KBlend::Create("LIGHTMERGE", Data);
	}

}