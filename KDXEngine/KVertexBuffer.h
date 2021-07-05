#pragma once
#include "K3DBufferBase.h"

class KVertexBuffer : public KGameResource<KVertexBuffer>, public K3DBufferBase
{
public:
	template<typename T>
	static void Create(const KGameString& _Name, const std::vector<T>& _Vtx, D3D11_USAGE _VTXUSAGE = D3D11_USAGE::D3D11_USAGE_DEFAULT)
	{
		KVertexBuffer* NewRest = new KVertexBuffer();
		NewRest->SetName(_Name);
		NewRest->Create(_Vtx.size(), sizeof(T), &_Vtx[0], _VTXUSAGE);
		NewRest->InsertResource();
	}

private:
	size_t m_SIZE;
	size_t m_COUNT;

	unsigned int m_uSIZE;

public:
	void Create(size_t _Count, size_t _Size, const void* _Data, D3D11_USAGE _USAGE);


public:
	void Setting();
};
