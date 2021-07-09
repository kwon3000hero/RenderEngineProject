#pragma once
#include <KGAMENAME.h>
#include <KGameReference.h>
#include <KGameType.h>
#include "KUpdateBase.h"
#include "KGameActor.h"
#include "KGameScene.h"

class KGameComponentManager;
class KGameScene;
class KGameActor;
class KGameComponent : public KGameReference, public KGAMENAME, public KGameType, public KUpdateBase
{
protected:
	int m_Order;
	KWeakPTR<KGameScene> m_Scene;
	KWeakPTR<KGameActor> m_Actor;

public:
	friend KGameComponentManager;

public:
	int Order()
	{
		return m_Order;
	}

	void SetOrder(int _Order)
	{
		m_Order = _Order;
	}

	KPTR<KGameScene> Scene()
	{
		return m_Scene;
	}

	void SetScene(KPTR<KGameScene> _PTR)
	{
		m_Scene = _PTR;
	}

	KPTR<KGameActor> Actor()
	{
		return m_Actor;
	}

	void SetActor(KPTR<KGameActor> _pActor)
	{
		m_Actor = _pActor;
	}

	void SetComponentData(KGameComponentManager* _ptr);

public:
	KGameComponent() : m_Order(0)
	{
	}

	virtual ~KGameComponent() {	}
};
//
//class KSceneGameComponent : public KGameComponent
//{
//protected:
//	KWeakPTR<KGameScene> m_Scene;
//
//public:
//	void SetComponentData(void* Ptr) override;
//
//protected:
//	void SetScene(KPTR<KGameScene> _PTR)
//	{
//		m_Scene = _PTR;
//	}
//
//public:
//	KPTR<KGameScene> Scene()
//	{
//		return m_Scene;
//	}
//
//public:
//	KSceneGameComponent() : m_Scene(nullptr)
//	{
//
//	}
//
//	virtual ~KSceneGameComponent()
//	{
//	}
//};
//
//
//class KActorGameComponent : public KSceneGameComponent
//{
//protected:
//	KWeakPTR<KGameActor> m_Actor;
//
//public:
//	void SetComponentData(void* Ptr) override;
//
//private:
//	void SetActor(KPTR<KGameActor> _pActor)
//	{
//		m_Actor = _pActor;
//	}
//
//public:
//	KPTR<KGameActor> Actor()
//	{
//		return m_Actor;
//	}
//};