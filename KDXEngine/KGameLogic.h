#pragma once
#include "KGameComponent.h"
#include "KTransform.h"
#include "KGameActor.h"
#include <KGAMEINPUT.h>
#include <KGAMETIME.h>
#include "KGameDebug3D.h"

class KGameLogic : public KActorGameComponent
{
public:
	KPTR<KGameActor> CreateObject(const KGameString& _Name = L"");
	KPTR<KTransform> Transform() { return Actor()->Transform(); };
};

