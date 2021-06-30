#pragma once
#include "KGameLogic.h"

class KRenderManager;
class KTest : public KGameLogic
{
public:
	KPTR<KTransform> TestTrans;

public:
	void Update() override;
};

