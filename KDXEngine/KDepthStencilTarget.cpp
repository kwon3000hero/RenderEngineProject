#include "KDepthStencilTarget.h"
#include "KTexture.h"


ID3D11DepthStencilView* KDepthStencilTarget::DSV()
{
	return m_Texture->DepthStencilView();
}

void KDepthStencilTarget::Create(KVector _Size, float _Depth, int _Stencil)
{
	m_Depth = _Depth;
	m_Stencil = _Stencil;

	m_Texture = make_KPTR<KTexture>();
	D3D11_TEXTURE2D_DESC DepthDesc = {};

	DepthDesc.ArraySize = 1;
	DepthDesc.Width = _Size.UIX();
	DepthDesc.Height = _Size.UIY();

	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthDesc.SampleDesc.Count = 1;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.MipLevels = 1;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;

	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_Texture->Create(DepthDesc);
}

void KDepthStencilTarget::Clear()
{
	KGameDevice::MainContext()->ClearDepthStencilView(m_Texture->DepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, m_Depth, m_Stencil);
}

void KDepthStencilTarget::Setting()
{
}
