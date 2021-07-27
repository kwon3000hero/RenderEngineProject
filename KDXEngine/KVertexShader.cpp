#include "KVertexShader.h"

void KVertexShader::KInputLayout::AddLayout(
	const char* _semanticName,
	unsigned int _index,
	DXGI_FORMAT _format,
	unsigned int _inputSlot,
	unsigned int _instDataStepRate /*= 0*/,
	D3D11_INPUT_CLASSIFICATION _inputClass/* = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA*/)
{
	if (nullptr != m_LayOut)
	{
		AssertMsg(L"버텍스 레이아웃이 제대로 만들어지지 않았습니다.");
	}

	D3D11_INPUT_ELEMENT_DESC Data = D3D11_INPUT_ELEMENT_DESC();

	Data.SemanticName = _semanticName;
	Data.SemanticIndex = _index;
	Data.Format = _format;
	Data.InputSlot = _inputSlot;
	Data.InstanceDataStepRate = _instDataStepRate;
	Data.InputSlotClass = _inputClass;

	Data.AlignedByteOffset = m_Offset;

	if (_format == DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT)
	{
		m_Offset += 16;
	}

	m_ArrInputLayOutData.push_back(Data);
}

void KVertexShader::KInputLayout::EndLayout(
	const char* _semanticName,
	unsigned int _index,
	DXGI_FORMAT _format,
	unsigned int _inputSlot,
	unsigned int _instDataStepRate /*= 0*/,
	D3D11_INPUT_CLASSIFICATION _inputClass/* = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA*/)
{
	AddLayout(_semanticName, _index, _format, _inputSlot, _instDataStepRate, _inputClass);

	CreateLayout();
}

void KVertexShader::KInputLayout::CreateLayout()
{
	HRESULT result = KGameDevice::MainDevice()->CreateInputLayout(&m_ArrInputLayOutData[0],
		(unsigned int)m_ArrInputLayOutData.size(),
		m_pParent->m_Blob->GetBufferPointer(),
		m_pParent->m_Blob->GetBufferSize(), &m_LayOut);

	if (S_OK != result)
	{
		assert(false);
	}
}

///////////////////////////////////////// 


void KVertexShader::Load(const KGameString& _FuncName, unsigned int _VH, unsigned int _VL)
{
	m_highVersion = _VH;
	m_lowVersion = _VL;

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif

	char Arr[256] = { 0 };
	// vs_5_0
	sprintf_s(Arr, "vs_%d_%d", _VH, _VL);

	std::string Name = _FuncName;
	HRESULT RS = D3DCompileFromFile(m_Path
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, Name.c_str(), Arr, Flag, 0, &m_Blob, &m_ErrBlob);

	if (S_OK != RS)
	{
		KGameString Error = (char*)m_ErrBlob->GetBufferPointer();
		_wassert(Error, _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
	}

	RS = KGameDevice::MainDevice()->CreateVertexShader(
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

	InputLayOut.m_pParent = this;

	CheckInputLayout(_FuncName);
}

void KVertexShader::CheckInputLayout(const KGameString& _shaderName)
{
	ID3D11ShaderReflection* pReflector = nullptr;

	if (S_OK != D3DReflect(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector))
	{
		AssertMsg(L"쉐이더 리소스 참조가 불가능 합니다.");
	}

	D3D11_SHADER_DESC ShaderDesc;
	pReflector->GetDesc(&ShaderDesc);

	for (unsigned int i = 0; i < ShaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC InputDesc = {};
		pReflector->GetInputParameterDesc(i, &InputDesc);

		DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;

		switch (InputDesc.Mask)
		{
		case 1:
			switch (InputDesc.ComponentType)
			{
			case D3D_REGISTER_COMPONENT_UINT32:
				format = DXGI_FORMAT_R32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				format = DXGI_FORMAT_R32_SINT;
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				format = DXGI_FORMAT_R32_FLOAT;
				break;
			default:
				AssertMsg(L"레이아웃 해석 실패");
				break;
			}
			break;
		case 3:
			switch (InputDesc.ComponentType)
			{
			case D3D_REGISTER_COMPONENT_UINT32:
				format = DXGI_FORMAT_R32G32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				format = DXGI_FORMAT_R32G32_SINT;
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			default:
				AssertMsg(L"레이아웃 해석 실패");
				break;
			}

			break;
		case 7:
			switch (InputDesc.ComponentType)
			{
			case D3D_REGISTER_COMPONENT_UINT32:
				format = DXGI_FORMAT_R32G32B32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				format = DXGI_FORMAT_R32G32B32_SINT;
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			default:
				AssertMsg(L"레이아웃 해석 실패");
				break;
			}

			break;
		case 15:
			switch (InputDesc.ComponentType)
			{
			case D3D_REGISTER_COMPONENT_UINT32:
				format = DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				format = DXGI_FORMAT_R32G32B32A32_SINT;
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			default:
				AssertMsg(L"레이아웃 해석 실패");
				break;
			}
			break;
		default:
			break;
		}

		if (format == DXGI_FORMAT::DXGI_FORMAT_UNKNOWN)
		{
			AssertMsg(L"인풋 레이아웃 포맷 판단에 실패했습니다.");
		}

		InputLayOut.AddLayout(InputDesc.SemanticName, 0, format, 0);
	}

	InputLayOut.CreateLayout();
}

void KVertexShader::Setting()
{
	KGameDevice::MainContext()->IASetInputLayout(InputLayOut.m_LayOut);
	KGameDevice::MainContext()->VSSetShader(m_ShaderPtr.Get(), nullptr, 0);
}