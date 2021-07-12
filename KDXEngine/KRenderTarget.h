#pragma once
#include "KTexture.h"
#include "KDepthStencilTarget.h"

class KRenderer;
class KRenderTarget : public KGameResource<KRenderTarget>
{
public:
	static KPTR<KRenderer> m_MergeRenderPlayer;
	static KPTR<KRenderer> m_AlwaysMergeRenderPlayer;

public:
	static void Init();
	static void End();

private:
	KVector m_ClearColor;

	KPTR<KDepthStencilTarget> m_Depth;
	std::vector<KPTR<KTexture>> m_RenderTargetTextureContainer;
	std::vector<D3D11_VIEWPORT>m_ViewPortContainer;
	std::vector<ID3D11RenderTargetView*> m_RenderTargetViewContainer;

public:
	void CreateDepth();
	void CreateViewPort(KPTR<KTexture> _texture);

	KPTR<KDepthStencilTarget> DepthStencil();

	void Create(KVector _Size, KVector _Color, DXGI_FORMAT _Fmt = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, UINT _BindFlag = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	void Create(ID3D11Texture2D* _pTEX, KVector _Color, UINT _BindFlag = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	void Clear(bool _isEnableDepth = true);
	void Setting();
	void Setting(unsigned int _index);

	void SetDepthStencil(KPTR<KDepthStencilTarget> _depthStencil);

public:
	void Merge(unsigned int _srcIndex, unsigned int  _destIndex, KPTR<KRenderTarget> _dest);
	void Merge(KPTR<KRenderTarget> _dest);

	void AlwaysMerge(unsigned int _srcIndex, unsigned int  _destIndex, KPTR<KRenderTarget> _dest);
	void AlwaysMerge(KPTR<KRenderTarget> _dest);

	void AlwaysCopy(unsigned int _srcIndex, unsigned int _destIndex, KPTR<KRenderTarget> _dest);
	void AlwaysCopy(KPTR<KRenderTarget> _dest);
	void AlwaysCopy(KPTR<KTexture> _dest);

	void Effect(KPTR<KRenderer> _renderPlayer, bool _IsClear = true, bool _IsDepthClear = false);
	void Effect(int _index, KPTR<KRenderer> _renderPlayer, bool _IsClear = true, bool _IsDepthClear = false);

	KPTR<KTexture> Texture(unsigned int _Index);

public:
	KRenderTarget() {}
	~KRenderTarget() {}
};

