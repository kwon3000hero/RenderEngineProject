#include "KGameComponent.h"
#include <assert.h>

void KSceneGameComponent::StartComData(void* Ptr)
{
	KGameScene* PTR = reinterpret_cast<KGameScene*>(Ptr);

	if (false == PTR->Equal<KGameScene>())
	{
		assert(false);
	}

	SetScene(PTR);
}


void KActorGameComponent::StartComData(void* Ptr)
{
	KGameActor* PTR = reinterpret_cast<KGameActor*>(Ptr);

	if (false == PTR->Equal<KGameActor>())
	{
		assert(false);
	}

	// ������ ������Ʈ�� �ڽ��� �θ�� �θ� ���� ���� �� �˰Եȴ� .
	SetActor(PTR);
	SetScene(PTR->Scene());
}