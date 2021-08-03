#include "KPixelShader.h"


void KPixelShader::Load(const KGameString& _FuncName, unsigned int _VH, unsigned int _VL)
{
	m_highVersion = _VH;
	m_lowVersion = _VL;

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif

	char Arr[256] = { 0 };
	sprintf_s(Arr, "ps_%d_%d", _VH, _VL);

	m_Name = _FuncName;
	HRESULT RS = D3DCompileFromFile(m_Path
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, m_Name.c_str(), Arr, Flag, 0, &m_Blob, &m_ErrBlob);

	if (S_OK != RS)
	{
		KGameString Error = (char*)m_ErrBlob->GetBufferPointer();
		_wassert(Error, _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
	}

	RS = KGameDeviceManager::MainDevice()->CreatePixelShader(
		m_Blob->GetBufferPointer(),
		m_Blob->GetBufferSize(),
		nullptr,
		&m_ShaderPtr
	);

	if (nullptr == m_ShaderPtr || FAILED(RS))
	{
		_wassert(L"쉐이더 파일이 제대로 만들어지지 않았습니다", _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
	}

	SetAutoShaderResource(_FuncName);
}


void KPixelShader::Setting()
{
	KGameDeviceManager::MainContext()->PSSetShader(m_ShaderPtr.Get(), nullptr, 0);
}