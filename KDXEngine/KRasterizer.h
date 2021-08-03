#pragma once
#include "KGameDeviceManager.h"
#include "KGameResource.h"

class KRasterizer : public KGameResource<KRasterizer>
{
private:
	D3D11_RASTERIZER_DESC m_Desc;
	ID3D11RasterizerState* m_OPTION;

public:
	static void Create(const KGameString& _Name, const D3D11_RASTERIZER_DESC& _Desc)
	{
		KRasterizer* NewRes = new KRasterizer();
		NewRes->SetName(_Name);
		NewRes->m_Desc = _Desc;
		NewRes->Create(_Desc);
		NewRes->InsertResource();
		return;
	}

public:
	void Create(const D3D11_RASTERIZER_DESC& _Desc);

	void Setting();

public:
	KRasterizer();
	~KRasterizer();

};

