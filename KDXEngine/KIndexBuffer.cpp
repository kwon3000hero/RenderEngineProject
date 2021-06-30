#include "KIndexBuffer.h"


void KIndexBuffer::Create(size_t _Count, size_t _Size, const void* _Data, D3D11_USAGE _USAGE)
{
	if (_Size == 2)
	{
		m_dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	}
	else if (_Size == 4)
	{
		m_dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	}

	m_SIZE = _Size;
	m_COUNT = _Count;
	m_uCOUNT = (unsigned int)_Count;

	m_BufferDesc.ByteWidth = (unsigned int)(m_SIZE * m_COUNT);
	m_BufferDesc.Usage = _USAGE;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == m_BufferDesc.Usage)
	{
		m_BufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	}

	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA DS = {};
	DS.pSysMem = _Data;

	HRESULT RS = KGameDevice::MainDevice()->CreateBuffer(&m_BufferDesc, &DS, &m_pBuffer);

	if (S_OK != RS)
	{
		assert(false);
	}

	return;
}


void KIndexBuffer::Setting() {

	unsigned int Offset = 0;
	KGameDevice::MainContext()->IASetIndexBuffer(m_pBuffer, m_dxgiFormat, Offset);
}