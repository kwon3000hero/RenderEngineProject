#include "KSampler.h"

void KSampler::Create(const D3D11_SAMPLER_DESC& _Desc)
{
	if (S_OK != KGameDevice::MainDevice()->CreateSamplerState(&m_Desc, &m_STATE))
	{
		assert(false);
	}

	return;
}
void KSampler::Setting(SHADER_TYPE _Type, unsigned int _Index)
{
	switch (_Type)
	{
	case SHADER_TYPE::ST_VS:
		// 한번에 여러개 세팅 가능하네요
		//                                        s몇?   몇개  
		KGameDevice::MainContext()->VSSetSamplers(_Index, 1, &m_STATE);
		break;
	case SHADER_TYPE::ST_HS:
		KGameDevice::MainContext()->HSSetSamplers(_Index, 1, &m_STATE);
		break;
	case SHADER_TYPE::ST_DS:
		KGameDevice::MainContext()->DSSetSamplers(_Index, 1, &m_STATE);
		break;
	case SHADER_TYPE::ST_GS:
		KGameDevice::MainContext()->GSSetSamplers(_Index, 1, &m_STATE);
		break;
	case SHADER_TYPE::ST_PS:
		KGameDevice::MainContext()->PSSetSamplers(_Index, 1, &m_STATE);
		break;
	case SHADER_TYPE::ST_MAX:
		break;
	default:
		break;
	}
}
