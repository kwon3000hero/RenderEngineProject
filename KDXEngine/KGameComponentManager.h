#pragma once
#include <KGAMENAME.h>
#include <KGameReference.h>
#include <KGameType.h>

#include <list>

#include "KUpdateBase.h"

class KGameComponent;
class KGameComponentManager : public KUpdateBase
{
protected:
	std::list<KPTR<KGameComponent>> m_componentList;

public:
	template<typename T, typename ... STARTDATA>
	KPTR<T> CreateComponent(STARTDATA ... _Arg)
	{
		T* NewCom = new T();
		NewCom->TypeSetting<T>();
		NewCom->StartComData(this);
		NewCom->StartData(_Arg...);
		NewCom->Init();
		m_componentList.push_back(NewCom);
		return NewCom;
	}

	template<typename T>
	KPTR<T> GetComponent()
	{
		for (auto& Com : m_componentList)
		{
			if (true == Com->Equal<T>())
			{
				return Com->VirtualChildConvert<T>();
			}
		}

		return nullptr;
	}


protected:
	void Init();
	void PrevUpdate() override;
	void Update() override;
	void NextUpdate() override;
	void PrevRender() override;
	void DebugRender() override;

public:
	KGameComponentManager() {}
	virtual ~KGameComponentManager() {}
};