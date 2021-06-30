#pragma once
#include <list>
#include <map>
#include "KGameReference.h"
#include "KGAMESTRING.h"
#include "KGAMEDEBUG.h"
#include "KCriticalSection.h"

template<typename T>
class KObjectManager
{
protected:
	KCriticalSection m_SyncObject;
	KPTR<T> m_MainObject;
	std::list<KPTR<T>> m_objectList;
	std::map<KGameString, KPTR<T>> m_objectMap;

public:
	KPTR<T> Find(const KGameString& _name)
	{
		typename std::map<KGameString, KPTR<T>>::iterator iter = m_objectMap.find(_name);

		if (m_objectMap.end() != iter)
		{
			return iter->second;
		}

		return nullptr;
	}

	void FindSync(const KGameString& _name)
	{
		m_SyncObject();
		return Find(_name);
	}

	template<typename ObjectType, typename ... Rest>
	KPTR<ObjectType> CreateObject(Rest ... _Args)
	{
		ObjectType* pNewObject = new ObjectType(_Args...);
		m_objectList.push_back(pNewObject);

		if (nullptr == m_MainObject)
		{
			m_MainObject = pNewObject;
		}

		return pNewObject;
	}

	template<typename ObjectType, typename ... Rest>
	KPTR<ObjectType> CreateObjectSync(Rest ... _Args)
	{
		m_SyncObject();		
		return CreateObject(_Args...);
	}

	template<typename ... Rest>
	void CreateObjectName(const KGameString& _Name, Rest... _Arg)
	{
		if (m_objectMap.end() != m_objectMap.find(_Name))
		{
			AssertMsg(L"이미 존재하는 이름의 관리 오브젝트가 있습니다.");
		}

		T* pNewT = new T(_Arg...);
		m_objectList.push_back(pNewT);
		m_objectMap[_Name] = pNewT;
		if (nullptr == m_MainObject)
		{
			m_MainObject = pNewT;
		}
	}

	template<typename ... Rest>
	void CreateObjectNameSync(const KGameString& _Name, Rest... _Arg)
	{
		m_SyncObject();
		CreateObjectName(_Name, _Arg...);
	}
};

