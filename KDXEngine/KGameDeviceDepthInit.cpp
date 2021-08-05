#include "KGameDeviceManager.h"
#include "KDepthStencil.h"

void KGameDevice::InitDepthStencilState()
{
	{
		D3D11_DEPTH_STENCIL_DESC Desc;
		memset(&Desc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));
		Desc.DepthEnable = true;
		Desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.StencilEnable = false;

		KDepthStencil::Create(L"DEFDEPTH", Desc);
	}

	{
		D3D11_DEPTH_STENCIL_DESC Desc;
		memset(&Desc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));
		Desc.DepthEnable = true;
		Desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		Desc.StencilEnable = false;

		KDepthStencil::Create(L"ALWAYSDEPTH", Desc);
	}
}