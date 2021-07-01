#pragma once

#include "KGameComponentManager.h"
#include <list>

class KTransform;
class KGameScene;
class KGameComponent;
class KGameActor : public KGameComponentManager, public KGameReference, public KGAMENAME, public KGameType
{
public:
	friend KGameScene;
	friend KTransform;

private:
	KPTR<KTransform> m_pTransform;
	void SetTransform(KPTR<KTransform> _pTrans)
	{
		m_pTransform = _pTrans;
	}

public:
	KPTR<KTransform> Transform()
	{
		return m_pTransform;
	}

protected:
	KWeakPTR<KGameScene> m_Scene;

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
	KGameActor();
	~KGameActor();
};

