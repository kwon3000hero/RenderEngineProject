#pragma once
#include "KGameResource.h"
#include "KGameDevice.h"

class K3DBufferBase
{
protected:
	D3D11_MAPPED_SUBRESOURCE m_SubRes; // 추후 설명
	D3D11_BUFFER_DESC m_BufferDesc; // 추후 설명
	ID3D11Buffer* m_pBuffer;

public:
	~K3DBufferBase() 
	{
		if (nullptr != m_pBuffer)
		{
			m_pBuffer->Release();
			m_pBuffer = nullptr;
		}

	}

};

