#include "KGameDevice.h"
#include "KSampler.h"

void KGameDevice::InitSamplerState()
{

	{
		{
			D3D11_SAMPLER_DESC Data;
			ZeroMemory(&Data, sizeof(Data));

			Data.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			Data.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			Data.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			Data.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			// ������ �̸� ���ø��� �����͸� ���?
			// �׳� �ϴ��� ���� �Ⱦ����̴�.
			Data.ComparisonFunc = D3D11_COMPARISON_NEVER;
			// �������� ���ϰڴ�.
			Data.MaxAnisotropy = 0;
			// �Ÿ��� �־����ų� �ؽ�ó�� ũ�⿡ ���� �������ε�
			// �ִ밪���� ���ڴ�.
			Data.MinLOD = -FLT_MAX;
			Data.MaxLOD = FLT_MAX;
			Data.MipLODBias = 1.0F;

			KSampler::Create("LWSMP", Data);
		}

		{
			D3D11_SAMPLER_DESC Data;
			ZeroMemory(&Data, sizeof(Data));

			Data.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			Data.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
			Data.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
			Data.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
			Data.ComparisonFunc = D3D11_COMPARISON_NEVER;
			Data.MaxAnisotropy = 0;
			Data.MinLOD = -FLT_MAX;
			Data.MaxLOD = FLT_MAX;
			Data.MipLODBias = 1.0F;
			KSampler::Create("LCSMP", Data);
		}

		{
			D3D11_SAMPLER_DESC Data;
			ZeroMemory(&Data, sizeof(Data));

			Data.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
			Data.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			Data.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			Data.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			Data.ComparisonFunc = D3D11_COMPARISON_NEVER;
			Data.MaxAnisotropy = 0;
			Data.MinLOD = -FLT_MAX;
			Data.MaxLOD = FLT_MAX;
			Data.MipLODBias = 1.0F;
			KSampler::Create("PWSMP", Data);
		}

		{
			D3D11_SAMPLER_DESC Data;
			ZeroMemory(&Data, sizeof(Data));

			Data.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
			Data.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
			Data.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
			Data.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
			Data.ComparisonFunc = D3D11_COMPARISON_NEVER;
			Data.MaxAnisotropy = 0;
			Data.MinLOD = -FLT_MAX;
			Data.MaxLOD = FLT_MAX;
			Data.MipLODBias = 1.0F;
			KSampler::Create("PCSMP", Data);
		}

		{
			D3D11_SAMPLER_DESC Data;
			ZeroMemory(&Data, sizeof(Data));

			Data.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
			Data.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
			Data.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
			Data.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
			Data.ComparisonFunc = D3D11_COMPARISON_NEVER;
			Data.MaxAnisotropy = 0;
			Data.MinLOD = -FLT_MAX;
			Data.MaxLOD = FLT_MAX;
			Data.MipLODBias = 1.0F;
			KSampler::Create("PMSMP", Data);
		}
	}
}