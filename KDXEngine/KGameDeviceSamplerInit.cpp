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
			// 색상의 미리 샘플링된 데이터를 사용?
			// 그냥 일단은 절대 안쓸것이다.
			Data.ComparisonFunc = D3D11_COMPARISON_NEVER;
			// 보간값이 안하겠다.
			Data.MaxAnisotropy = 0;
			// 거리가 멀어지거나 텍스처의 크기에 따른 보간값인데
			// 최대값으로 쓰겠다.
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