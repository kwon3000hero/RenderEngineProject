#pragma once
#include "KCamera.h"

class KBloomEffect : public KPostEffect
{
public:
	KPTR<KRenderer> m_CheckPlayer;
	KPTR<KRenderer> m_BloomPlayer;
	KPTR<KRenderer> m_LightBloomPlayer;

	KPTR<KRenderTarget> m_SmallTarget;
	KPTR<KRenderTarget> m_BloomEffectTarget1;
	KPTR<KRenderTarget> m_BloomEffectTarget2;

public:
	void Init() override;
	void MergeNextEffect() override;
};
