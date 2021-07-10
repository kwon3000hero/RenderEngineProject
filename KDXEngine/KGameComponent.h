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