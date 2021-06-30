#pragma once
#include "KCamera.h"
class KFxaa :
    public KPostEffect
{
public:
	KPTR<KRenderPlayer> m_FXAAPlayer;
	KPTR<KRenderTarget> m_FXAATarget;


public:
	void Init() override;
	void MergeNextEffect() override;
};

