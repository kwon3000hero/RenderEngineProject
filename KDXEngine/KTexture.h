#pragma once
#include "KGameResource.h"
#include "KGameDevice.h"
#include <DirectXTex.h>
#include "KShader.h"

#pragma comment(lib, "DirectXTex.lib")

class KRenderTarget;
class KDepthStencilTarget;
class KMeshAnimator;
class KTexture : public KGameResource<KTexture>
{
public:
	friend KRenderTarget;
	friend KDepthStencilTarget;
	friend KMeshAnimator;

public:
	static void Load(const KGameString& _Path)
	{
		KTexture* pTexture = new KTexture();
		pTexture->m_Path = _Path;
#ifdef _DEBUG
		KGameString fileName = KGAMEPATH::GetFileName(_Path);
		pTexture->SetName(fileName);
#elif
		pTexture->SetName(KGAMEPATH::GetFileName(_Path));
#endif
		pTexture->Load();
		pTexture->InsertResource();
	}

private:
	DirectX::ScratchImage m_Image;
	D3D11_TEXTURE2D_DESC m_Desc;
	ID3D11Resource* m_pTexture;
	ID3D11RenderTargetView* m_pRTV;
	ID3D11DepthStencilView* m_pDSV;
	ID3D11ShaderResourceView* m_pSRV;

public:
	ID3D11RenderTargetView* RTV() { return m_pRTV; }
	ID3D11DepthStencilView* DepthStencilView() { return m_pDSV; }
	ID3D11ShaderResourceView* SRV() { return m_pSRV; }

public:
	KVector Size();

	void SettingData();

public:
	void Load();
	void Create(KVector _Size, DXGI_FORMAT _Fmt, UINT _BindFlag, D3D11_USAGE _eUsage);
	void Create(ID3D11Texture2D* _pTEX, UINT _BindFlag);
	void Create(const D3D11_TEXTURE2D_DESC& _Desc);

public:
	void Setting(SHADER_TYPE _Type, unsigned int _Index);

	KVector Pixel(size_t _x, size_t _y);
	void SetPixel(void* _pPixelData, int size);

public:
	KTexture();
	~KTexture();
};

