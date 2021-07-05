#pragma once
#include <unordered_map>
#include "KCriticalSection.h"

template<typename T>
class KGameResource;

template<typename T>
class KGameResourceManager final
{
	friend KGameResource<T>;

private:
	KCriticalSection m_SyncObject;
	std::unordered_multimap<KGameString, KPTR<T>, KGameStringHash> m_ResourceContainer;

public:
	static KGameResourceManager* m_instance;

public:
	static KGameResourceManager& Instance()
	{
		if (nullptr == m_instance)
		{
			m_instance = new KGameResourceManager();
		}

		return *m_instance;
	}

private:
	KGameResourceManager()
	{
	}

	~KGameResourceManager()
	{
	}

public:
	KPTR<T> Find(const KGameString& _Key)
	{
		typename std::unordered_multimap<KGameString, KPTR<T>, KGameStringHash>::iterator findIter = m_ResourceContainer.find(_Key);

		if (m_ResourceContainer.end() == findIter)
		{
			return static_cast<KPTR<T>>(0);
		}

		return findIter->second;
	}

	KPTR<T> FindSync(const KGameString& _Key)
	{
		m_SyncObject();

		return Find(_Key);
	}


	void Delete(const KGameString& _Key)
	{
		if (m_ResourceContainer.end() == m_ResourceContainer.find(_Key))
		{
			return;
		}

		m_ResourceContainer.erase(_Key);
	}

	void DeleteSync(const KGameString& _Key)
	{
		m_SyncObject();
		Delete(_Key);
	}

	void AllDelete()
	{
		m_ResourceContainer.clear();
	}


	KPTR<T> Create(const KGameString& _name = L"")
	{
		T* NewT = new T();
		NewT->TypeSetting<T>();
		NewT->SetName(_name);
		NewT->InsertResource();
		return NewT;
	}

	KPTR<T> CreateSync(const KGameString& _name = L"")
	{
		m_SyncObject();
		return Create(_name);
	}


	const KCriticalSection SyncObject()
	{
		return m_SyncObject;
	}
};



template<typename T>
KGameResourceManager<T>* KGameResourceManager<T>::m_instance = nullptr;