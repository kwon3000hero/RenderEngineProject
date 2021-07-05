#pragma once

#include "KGameDevice.h"
#include "KGameResource.h"

class KBlend : public KGameResource<KBlend>
{
private:
	unsigned int BlendMask;
	D3D11_BLEND_DESC m_Desc;
	ID3D11BlendState* m_STATE;

public:
	static void Create(const KGameString& _Name, const D3D11_BLEND_DESC& _Desc)
	{
		KBlend* NewRes = new KBlend();
		NewRes->SetName(_Name);
		NewRes->m_Desc = _Desc;
		NewRes->Create(_Desc);
		NewRes->InsertResource();
		return;
	}

public:
	void Create(const D3D11_BLEND_DESC& _Desc);

	void Setting();

private:
	KBlend();
	~KBlend();

};

