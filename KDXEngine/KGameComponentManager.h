#pragma once
#include <KGAMENAME.h>
#include <KGameReference.h>
#include <list>
#include "KUpdateBase.h"

class KGameComponent;
class KGameComponentManager : public KUpdateBase
{
private:
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
		return KPTR<T>(NewCom);
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