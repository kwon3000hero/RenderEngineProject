#pragma once
#include <KGAMENAME.h>
#include <KGameReference.h>
#include <list>
#include "KUpdateBase.h"
#include <KGameType.h>

class KGameComponent;
class KGameComponentManager : public KUpdateBase, public KGameType
{
private:
	std::list<KPTR<KGameComponent>> m_componentList;

public:
	template<typename T, typename ... STARTDATA>
	KPTR<T> CreateComponent(STARTDATA ... _Arg)
	{
		T* newComponent = new T(_Arg...);
		newComponent->TypeSetting<T>();
		newComponent->SetComponentData(this);
		newComponent->Init();
		m_componentList.push_back(newComponent);
		return KPTR<T>(newComponent);
	}

	template<typename T>
	KPTR<T> GetComponent()
	{
		for (auto& Com : m_componentList)
		{
			if (true == Com->Equal<T>())
			{
				return Com->TryConvertTo<T>();
			}
		}

		return static_cast<KPTR<T>>(0);
	}


protected:
	void Init();

	void PrevUpdate() override;

	void Update() override;

	void NextUpdate() override;

	void PrevRender() override;

	void DebugRender() override;

protected:
	KGameComponentManager() {}
	virtual ~KGameComponentManager() {}
};