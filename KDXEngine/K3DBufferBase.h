#pragma once
#include "KGameResource.h"
#include "KGameDeviceManager.h"

class K3DBufferBase
{
protected:
	D3D11_MAPPED_SUBRESOURCE m_SubRes;
	D3D11_BUFFER_DESC m_BufferDesc;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;

public:
	~K3DBufferBase()
	{
		m_pBuffer = nullptr;
	}
};

