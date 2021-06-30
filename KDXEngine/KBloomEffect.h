#pragma once
#include "KCamera.h"

class KBloomEffect : public KPostEffect
{
public:
	KPTR<KRenderPlayer> m_CheckPlayer;
	KPTR<KRenderPlayer> m_BloomPlayer;
	KPTR<KRenderPlayer> m_LightBloomPlayer;

	KPTR<KRenderTarget> m_SmallTarget;
	KPTR<KRenderTarget> m_BloomEffectTarget1;
	KPTR<KRenderTarget> m_BloomEffectTarget2;

public:
	void Init() override;
	void MergeNextEffect() override;
};
