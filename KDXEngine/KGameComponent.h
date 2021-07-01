#pragma once
#include <KGAMENAME.h>
#include <KGameReference.h>
#include <KGameType.h>
#include "KUpdateBase.h"
#include "KGameActor.h"

class KGameComponentManager;
class KGameComponent : public KGameReference, public KGAMENAME, public KGameType, public KUpdateBase
{
private:
	int m_Order;

public:
	friend KGameComponentManager;
	void StartData() {}


public:
	int Order()
	{
		return m_Order;
	}

	void SetOrder(int _Order)
	{
		m_Order = _Order;
	}

private:
	virtual void StartComData(void* Ptr) = 0;

public:
	KGameComponent() : m_Order(0)
	{
	}

	virtual ~KGameComponent() {	}
};

#include "KGameScene.h"

class KGameScene;
class KSceneGameComponent : public KGameComponent
{
protected:
	KWeakPTR<KGameScene> m_Scene;

public:
	void StartComData(void* Ptr) override;

protected:
	void SetScene(KGameScene* _PTR)
	{
		m_Scene = _PTR;
	}

public:
	KGameScene* Scene()
	{
		return m_Scene;
	}

public:
	KSceneGameComponent() : m_Scene(nullptr)
	{

	}

	virtual ~KSceneGameComponent()
	{
	}
};


class KGameActor;
class KActorGameComponent : public KSceneGameComponent
{
protected:
	KWeakPTR<KGameActor> m_Actor;

public:
	void StartComData(void* Ptr) override;

private:
	void SetActor(KGameActor* _pActor)
	{
		m_Actor = _pActor;
	}

public:
	KGameActor* Actor()
	{
		return m_Actor;
	}
};