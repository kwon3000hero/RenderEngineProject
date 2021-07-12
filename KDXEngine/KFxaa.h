#pragma once
#include "KCamera.h"
class KFxaa :
    public KPostEffect
{
public:
	KPTR<KRenderer> m_FXAAPlayer;
	KPTR<KRenderTarget> m_FXAATarget;


public:
	void Init() override;
	void MergeNextEffect() override;
};

