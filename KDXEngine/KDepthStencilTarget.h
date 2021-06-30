#pragma once
#include "KTexture.h"

class KDepthStencilTarget : public KGameResource<KDepthStencilTarget>
{
private:
	KPTR<KTexture> m_Texture;
	float m_Depth;
	int m_Stencil;

public:
	ID3D11DepthStencilView* DSV();

public:
	void Create(KVector _Size, float _Depth, int _Stencil);
	void Clear();
	void Setting();
};

