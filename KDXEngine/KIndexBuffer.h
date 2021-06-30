#pragma once
#include "K3DBufferBase.h"
#include "KGameResource.h"

class KIndexBuffer : public KGameResourceBase<KIndexBuffer>, public K3DBufferBase
{
public:
	template<typename T>
	static void Create(const KGameString& _Name, const std::vector<T>& _Vtx, D3D11_USAGE _VTXUSAGE = D3D11_USAGE::D3D11_USAGE_DEFAULT)
	{
		KIndexBuffer* NewBuffer = new KIndexBuffer();
		NewBuffer->SetName(_Name);
		NewBuffer->Create(_Vtx.size(), sizeof(T), &_Vtx[0], _VTXUSAGE);
		NewBuffer->InsertResource();
	}

private:
	DXGI_FORMAT m_dxgiFormat;
	size_t m_SIZE;
	size_t m_COUNT;
	unsigned int m_uCOUNT;

public:
	void SetFormat(DXGI_FORMAT _format)
	{
		m_dxgiFormat = _format;
	}

	unsigned int Count()
	{
		return m_uCOUNT;
	}

public:
	void Setting();

public:
	void Create(size_t _Count, size_t _Size, const void* _Data, D3D11_USAGE _USAGE);
};

