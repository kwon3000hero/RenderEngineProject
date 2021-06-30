#pragma once
#include "KGameComponentManager.h"
#include "KTransform.h"
#include "KGameActor.h"
#include <KGAMEINPUT.h>
#include <KGAMETIME.h>
#include "KGameDebug3D.h"

class KGameLogic : public KActorGameComponent
{
public:
	KPTR<KGameActor> CreateObject(const KGameString& _Name = L"");
	KTransform* Transform() { return Actor()->Transform(); };
};

