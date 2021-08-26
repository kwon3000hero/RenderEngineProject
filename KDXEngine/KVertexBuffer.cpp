#include "KVertexBuffer.h"


void KVertexBuffer::Create(
	size_t _Count,
	size_t _Size,
	const void* _pData, 
	D3D11_USAGE _VTXUSAGE
)
{
	m_SIZE = _Size;
	m_uSIZE = (unsigned int)m_SIZE;

	m_COUNT = _Count;

	m_BufferDesc.ByteWidth = (unsigned int)(m_SIZE * m_COUNT);
	m_BufferDesc.Usage = _VTXUSAGE;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == m_BufferDesc.Usage)
	{
		m_BufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	}

	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA DS = {};
	DS.pSysMem = _pData;

	HRESULT RS = KGameDeviceManager::MainDevice()->CreateBuffer(&m_BufferDesc, &DS, m_pBuffer.GetAddressOf());

	if (S_OK != RS)
	{
		assert(false);
	}

	return;
}


void KVertexBuffer::Setting()
{
	unsigned int Offset = 0;

	KGameDeviceManager::MainContext()->IASetVertexBuffers(0, 1, &m_pBuffer, &m_uSIZE, &Offset);
}