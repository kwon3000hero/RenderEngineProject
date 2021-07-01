#pragma once

#include "KGameLogic.h"
#include <KTexture.h>
#include <KConstantBuffer.h>
#include <KSampler.h>

class KFrameAnimator;
class KPlayer :
	public KGameLogic
{
public:
	float MoveSpeed;
	KPTR<KFrameAnimator> ANIPTR;

public:
	float Speed;

public:
	void Init() override;
	void Update() override;

public:
	void PlayerColFunc(KPTR<KCollision> _this, KPTR<KCollision> _Other);
	void PlayerColFunc2(KPTR<KCollision> _this, KPTR<KCollision> _Other);

};

