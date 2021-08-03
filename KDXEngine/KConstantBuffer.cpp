#include "KConstantBuffer.h"

void KConstantBuffer::CreateCB(size_t _Size, SHADER_TYPE _eType, unsigned int _Reg, D3D11_USAGE _USAGE)
{
	ShaderType(_eType);
	uiReg = _Reg;


	// �׷���ī�忡 �˷��� ���ۻ������� ������ ����Ʈ ũ��
	// ���Ƿ� �־�þ��
	// ��������� ũ��� ������ 16�� ������� �Ѵ�.
	m_BufferDesc.ByteWidth = (unsigned int)_Size;

	if (0 != (m_BufferDesc.ByteWidth % 16))
	{
		assert(false);
	}

	m_BufferDesc.Usage = _USAGE;
	// �츮�� ����� �������ֹǷ� �̳༮�� ������ �츮�� �����ϰ� �ȴ�.
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// �뵵
	m_BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// ���� ���ҽ��� ä�� �־��� ���� �ʱⰪ
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
	// �Լ������͸� �������ִ� �ͻ��̴�.
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