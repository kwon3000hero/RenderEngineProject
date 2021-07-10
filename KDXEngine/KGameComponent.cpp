#include "KGameComponent.h"
#include <assert.h>

void KGameComponent::SetComponentData(KGameComponentManager* _ptr)
{
	KGameActor* actorPtr;
	KGameScene* scenePtr;
	if (nullptr != (actorPtr = _ptr->TryConvertTo<KGameActor>().get()))
	{
		SetActor(actorPtr);
		SetScene(actorPtr->Scene());
	}
	else if (nullptr != (scenePtr = _ptr->TryConvertTo<KGameScene>().get()))
	{
		SetScene(scenePtr);
	}
}