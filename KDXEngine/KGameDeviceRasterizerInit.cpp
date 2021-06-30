
#include "KGameDevice.h"
#include "KRasterizer.h"

void KGameDevice::InitRasterizer() {

	{
		D3D11_RASTERIZER_DESC Desc;
		memset(&Desc, 0, sizeof(D3D11_RASTERIZER_DESC));
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		Desc.MultisampleEnable = TRUE;
		Desc.AntialiasedLineEnable = TRUE;
		Desc.DepthClipEnable = TRUE;
		Desc.ScissorEnable = FALSE;
		KRasterizer::Create(L"BACK", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc;
		memset(&Desc, 0, sizeof(D3D11_RASTERIZER_DESC));
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		Desc.MultisampleEnable = TRUE;
		Desc.AntialiasedLineEnable = TRUE;
		Desc.DepthClipEnable = TRUE;
		Desc.ScissorEnable = FALSE;
		KRasterizer::Create(L"FRONT", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc;
		memset(&Desc, 0, sizeof(D3D11_RASTERIZER_DESC));
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.MultisampleEnable = TRUE;
		Desc.AntialiasedLineEnable = TRUE;
		Desc.DepthClipEnable = TRUE;
		Desc.ScissorEnable = FALSE;
		KRasterizer::Create(L"NONE", Desc);
	}

}