#pragma once
#include "KGameLogic.h"

class K3DPlayer : public KGameLogic
{
private:
	KPTR<KMeshAnimator> m_MeshAnimator;

public:
	void Init() override;
	void Update() override;

public:
	KPTR<KMeshAnimator> MeshAnimator()
	{
		return m_MeshAnimator;
	}
};

