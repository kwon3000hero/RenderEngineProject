#pragma once

#include "KGameLogic.h"
#include <KTexture.h>
#include <KConstantBuffer.h>
#include <KSampler.h>

class KTitleObject :public KCuttingRender
{
public:
	KTitleObject();
	virtual ~KTitleObject();

public:
	void Init() override;
	void Update() override;

public:
	float m_progress;
};

