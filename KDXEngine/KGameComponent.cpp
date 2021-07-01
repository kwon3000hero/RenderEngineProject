#include "KGameComponent.h"
#include <assert.h>

void KSceneGameComponent::StartComData(void* Ptr)
{
	KPTR<KGameScene> PTR(reinterpret_cast<KGameScene*>(Ptr));

	if (false == PTR->Equal<KGameScene>())
	{
		assert(false);
	}

	SetScene(PTR);
}


void KActorGameComponent::StartComData(void* Ptr)
{
	KPTR<KGameActor> PTR(reinterpret_cast<KGameActor*>(Ptr));

	if (false == PTR->Equal<KGameActor>())
	{
		assert(false);
	}

	// 액터의 컴포넌트는 자신의 부모와 부모를 가진 씬도 다 알게된다 .
	SetActor(PTR);
	SetScene(PTR->Scene());
}