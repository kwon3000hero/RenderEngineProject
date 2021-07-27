#include "KTexture.h"
#include "KGameDevice.h"

KTexture::KTexture() :
	m_pTexture(nullptr),
	m_pRTV(nullptr),
	m_pDSV(nullptr),
	m_pSRV(nullptr),
	m_Desc({ 0, })
{
}

KTexture::~KTexture()
{
	if (nullptr != m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	if (nullptr != m_pRTV)
	{
		m_pRTV->Release();
		m_pRTV = nullptr;
	}

	if (nullptr != m_pDSV)
	{
		m_pDSV->Release();
		m_pDSV = nullptr;
	}

	if (nullptr != m_pSRV)
	{
		m_pSRV->Release();
		m_pSRV = nullptr;
	}

	m_Image.Release();
}

KVector KTexture::Size()
{
	return KVector((float)m_Desc.Width, (float)m_Desc.Height);
}

void KTexture::Load()
{
	if (KGAMEPATH::GetExt(m_Path) == L"DDS" || KGAMEPATH::GetExt(m_Path) == L"dds")
	{
		AssertMsg(L"텍스처 로딩에 실패했습니다.");
	}
	else if (KGAMEPATH::GetExt(m_Path) == L"TGA" || KGAMEPATH::GetExt(m_Path) == L"tga")
	{
		if (S_OK != DirectX::LoadFromTGAFile(m_Path, nullptr, m_Image))
		{
			AssertMsg(L"텍스처 로딩에 실패했습니다.");
			return;
		}
	}
	else
	{
		if (S_OK != DirectX::LoadFromWICFile(m_Path, DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			AssertMsg(L"텍스처 로딩에 실패했습니다.");
			return;
		}
	}

	m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	HRESULT Return = DirectX::CreateShaderResourceView(
		KGameDevice::MainDevice().Get(),
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(),
		&m_pSRV);

	if (S_OK != Return)
	{

		AssertMsg(L"쉐이더 리소스 생성에 실패했습니다.");
	}

	m_Desc.Width = (UINT)m_Image.GetMetadata().width;
	m_Desc.Height = (UINT)m_Image.GetMetadata().height;

}

void KTexture::Setting(SHADER_TYPE _Type, unsigned int _Index)
{
	switch (_Type)
	{
	case SHADER_TYPE::ST_VS:
		KGameDevice::MainContext()->VSSetShaderResources(_Index, 1, &m_pSRV);
		break;
	case SHADER_TYPE::ST_HS:
		KGameDevice::MainContext()->HSSetShaderResources(_Index, 1, &m_pSRV);
		break;
	case SHADER_TYPE::ST_DS:
		KGameDevice::MainContext()->DSSetShaderResources(_Index, 1, &m_pSRV);
		break;
	case SHADER_TYPE::ST_GS:
		KGameDevice::MainContext()->GSSetShaderResources(_Index, 1, &m_pSRV);
		break;
	case SHADER_TYPE::ST_PS:
		KGameDevice::MainContext()->PSSetShaderResources(_Index, 1, &m_pSRV);
		break;
	case SHADER_TYPE::ST_MAX:
		break;
	default:
		break;
	}
}

KVector KTexture::Pixel(size_t _x, size_t _y)
{
	if (_x >= Size().x - 1)
	{
		return KVector::ZERO;
	}

	if (_y >= Size().y - 1)
	{
		return KVector::ZERO;
	}

	uint8_t* pImagePixels = m_Image.GetPixels();

	unsigned int intervalSize = 4;
	pImagePixels = pImagePixels + (intervalSize * _x);
	pImagePixels = pImagePixels + (intervalSize * Size().UIX() * _y);

	KVector rgba
	{
		pImagePixels[0] / 255.0f,
		pImagePixels[1] / 255.0f,
		pImagePixels[2] / 255.0f,
		pImagePixels[3] / 255.0f
	};

	return rgba;
}

void KTexture::SetPixel(void* _pPixelData, int _size)
{
	D3D11_MAPPED_SUBRESOURCE subResource;

	KGameDevice::MainContext()->Map(m_pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	memcpy_s(subResource.pData, _size, _pPixelData, _size);
	KGameDevice::MainContext()->Unmap(m_pTexture, 0);
}

void KTexture::Create(KVector _Size, DXGI_FORMAT _Fmt, UINT _BindFlag, D3D11_USAGE _eUsage)
{
	m_Desc.Width = (UINT)_Size.x;
	m_Desc.Height = (UINT)_Size.y;
	m_Desc.ArraySize = 1;

	m_Desc.Usage = _eUsage;
	m_Desc.Format = _Fmt;

	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;
	m_Desc.MipLevels = 1;
	m_Desc.BindFlags = _BindFlag;

	if (m_Desc.Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_Desc.CPUAccessFlags = 0;
	}

	if (S_OK != KGameDevice::MainDevice()->CreateTexture2D(&m_Desc, nullptr, (ID3D11Texture2D**)&m_pTexture))
	{
		assert(false);
	}

	SettingData();

}

void KTexture::Create(ID3D11Texture2D* _pTEX, UINT _BindFlag)
{
	m_pTexture = _pTEX;

	_pTEX->GetDesc(&m_Desc);

	SettingData();
}

void KTexture::SettingData()
{
	if (0 != (D3D11_BIND_SHADER_RESOURCE & m_Desc.BindFlags))
	{
		if (S_OK != KGameDevice::MainDevice()->CreateShaderResourceView(m_pTexture, nullptr, &m_pSRV))
		{
			assert(false);
		}
	}

	if (0 != (D3D11_BIND_DEPTH_STENCIL & m_Desc.BindFlags))
	{
		if (S_OK != KGameDevice::MainDevice()->CreateDepthStencilView(m_pTexture, nullptr, &m_pDSV))
		{
			assert(false);
		}
	}

	if (0 != (D3D11_BIND_RENDER_TARGET & m_Desc.BindFlags))
	{
		if (S_OK != KGameDevice::MainDevice()->CreateRenderTargetView(m_pTexture, nullptr, &m_pRTV))
		{
			assert(false);
		}
	}
}

void KTexture::Create(const D3D11_TEXTURE2D_DESC& _Desc)
{
	m_Desc = _Desc;

	if (m_Desc.Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else {
		m_Desc.CPUAccessFlags = 0;
	}

	if (S_OK != KGameDevice::MainDevice()->CreateTexture2D(&m_Desc, nullptr, (ID3D11Texture2D**)&m_pTexture))
	{
		assert(false);
	}

	SettingData();

}