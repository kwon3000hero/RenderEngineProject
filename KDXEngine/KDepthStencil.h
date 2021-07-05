#pragma once
#include "KGameDevice.h"
#include "KGameResource.h"

class KDepthStencil : public KGameResource<KDepthStencil>
{
private:
	D3D11_DEPTH_STENCIL_DESC m_Desc;
	ID3D11DepthStencilState* m_OPTION;

public:
	static void Create(const KGameString& _Name, const D3D11_DEPTH_STENCIL_DESC& _Desc)
	{
		KDepthStencil* NewRes = new KDepthStencil();
		NewRes->SetName(_Name);
		NewRes->m_Desc = _Desc;
		NewRes->Create(_Desc);
		NewRes->InsertResource();
		return;
	}

public:
	void Create(const D3D11_DEPTH_STENCIL_DESC& _Desc);
	void Setting();

public:
	KDepthStencil();
	~KDepthStencil();
};

