#include "KGameComponent.h"
#include <assert.h>

void KGameComponent::SetComponentData(KGameComponentManager*  _ptr/*const KPTR<KGameActor>& _actor*/)
{


	if (true == _ptr->Equal<KGameActor>())
	{
		KGameActor* ptr = _ptr->TryConvertTo<KGameActor>().get();
		SetActor(ptr);
		SetScene(ptr->Scene());
	}
	else if (true == _ptr->Equal<KGameScene>())
	{
		KGameScene* ptr = _ptr->TryConvertTo<KGameScene>().get();
		SetScene(ptr);
	}
}

//
//void KActorGameComponent::SetComponentData(void* Ptr)
//{
//	KGameActor* PTR = reinterpret_cast<KGameActor*>(Ptr);
//
//	if (false == PTR->Equal<KGameActor>())
//	{
//		assert(false);
//	}
//
//	SetActor(PTR);
//	SetScene(PTR->Scene());
//}