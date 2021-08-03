#include "KConstantBuffer.h"

void KConstantBuffer::CreateCB(size_t _Size, SHADER_TYPE _eType, unsigned int _Reg, D3D11_USAGE _USAGE)
{
	ShaderType(_eType);
	uiReg = _Reg;


	// 그래픽카드에 알려줄 버퍼생성시의 버퍼의 바이트 크기
	// 임의로 넣어봤어요
	// 상수버퍼의 크기는 무조건 16의 배수여야 한다.
	m_BufferDesc.ByteWidth = (unsigned int)_Size;

	if (0 != (m_BufferDesc.ByteWidth % 16))
	{
		assert(false);
	}

	m_BufferDesc.Usage = _USAGE;
	// 우리가 행렬을 세팅해주므로 이녀석은 언제나 우리가 접근하게 된다.
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 용도
	m_BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// 서브 리소스로 채워 넣어준 값은 초기값
	// D3D11_SUBRESOURCE_DATA DS = {};
	// DS.pSysMem = _Data;

	HRESULT RS = KGameDeviceManager::MainDevice()->CreateBuffer(&m_BufferDesc, nullptr, &m_pBuffer);

	if (S_OK != RS)
	{
		assert(false);
	}


}

void KConstantBuffer::VSetting() {
	KGameDeviceManager::MainContext()->VSSetConstantBuffers(uiReg, 1, &m_pBuffer);
}
void KConstantBuffer::HSetting() {
	KGameDeviceManager::MainContext()->HSSetConstantBuffers(uiReg, 1, &m_pBuffer);
}
void KConstantBuffer::DSetting() {
	KGameDeviceManager::MainContext()->DSSetConstantBuffers(uiReg, 1, &m_pBuffer);
}
void KConstantBuffer::GSetting() {
	KGameDeviceManager::MainContext()->GSSetConstantBuffers(uiReg, 1, &m_pBuffer);
}
void KConstantBuffer::PSetting() {
	KGameDeviceManager::MainContext()->PSSetConstantBuffers(uiReg, 1, &m_pBuffer);
}

void KConstantBuffer::ShaderType(SHADER_TYPE _Type)
{
	m_eType = _Type;
	// 함수포인터를 세팅해주는 것뿐이다.
	switch (m_eType)
	{
	case SHADER_TYPE::ST_VS:
		m_UpdateFunc = &KConstantBuffer::VSetting;
		break;
	case SHADER_TYPE::ST_HS:
		m_UpdateFunc = &KConstantBuffer::HSetting;
		break;
	case SHADER_TYPE::ST_DS:
		m_UpdateFunc = &KConstantBuffer::DSetting;
		break;
	case SHADER_TYPE::ST_GS:
		m_UpdateFunc = &KConstantBuffer::GSetting;
		break;
	case SHADER_TYPE::ST_PS:
		m_UpdateFunc = &KConstantBuffer::PSetting;
		break;
	case SHADER_TYPE::ST_MAX:
		assert(false);
		break;
	default:
		break;
	}
}



void KConstantBuffer::Setting()
{
	(this->*m_UpdateFunc)();
}