#pragma once

#include "KGameResource.h"
#include "KShader.h"

class KSampler : public KGameResource<KSampler>
{
private:
	D3D11_SAMPLER_DESC m_Desc;
	ID3D11SamplerState* m_STATE;

public:
	static void Create(const KGameString& _Name, const D3D11_SAMPLER_DESC& _Desc)
	{
		KSampler* NewRes = new KSampler();
		NewRes->SetName(_Name);
		NewRes->m_Desc = _Desc;
		NewRes->Create(_Desc);
		NewRes->InsertResource();
		return;
	}

public:
	void Create(const D3D11_SAMPLER_DESC& _Desc);
	void Setting(SHADER_TYPE _Type, unsigned int _Index);

public:
	KSampler() : m_STATE(nullptr) {}
	~KSampler()
	{
		if (nullptr != m_STATE)
		{
			m_STATE->Release();
			m_STATE = nullptr;
		}
	}
};