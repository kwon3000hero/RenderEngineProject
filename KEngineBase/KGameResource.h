#pragma once
#include "KGAMENAME.h"
#include "KGameReference.h"
#include "KGAMEFILEBASE.h"
#include "KGameType.h"
#include <unordered_map>
#include "KGAMEDEBUG.h"
#include "KGAMESTRING.h"
#include "KCriticalSection.h"

template<typename T>
class KGameResourceBase : public KGameReference, public KGAMENAME, public KGameType
{
protected:
	static KCriticalSection m_SyncObject;
	static std::unordered_multimap<KGameString, KPTR<T>, KGameStringHash> m_ResourceContainer;

protected:
	KGameString m_Path;

public:
	static KPTR<T> Find(const KGameString& _Key)
	{
		typename std::unordered_multimap<KGameString, KPTR<T>, KGameStringHash>::iterator findIter = m_ResourceContainer.find(_Key);

		if (m_ResourceContainer.end() == findIter)
		{
			return static_cast<KPTR<T>>(0);
		}

		return findIter->second;
	}

	static KPTR<T> FindSync(const KGameString& _Key)
	{
		m_SyncObject();

		return Find(_Key);
	}

protected:
	void InsertResource()
	{
		m_SyncObject();
#ifdef _DEBUG
		KGameString name = Name();
		m_ResourceContainer.insert(std::unordered_multimap<KGameString, KPTR<T>, KGameStringHash>::value_type(name, (T*)this));
#elif
		m_ResourceContainer.insert(std::unordered_multimap<KGameString, KPTR<T>, KGameStringHash>::value_type(Name(), (T*)this));

#endif
	}

	static void Delete(const KGameString& _Key)
	{
		if (m_ResourceContainer.end() == m_ResourceContainer.find(_Key))
		{
			return;
		}

		m_ResourceContainer.erase(_Key);
	}

	static void DeleteSync(const KGameString& _Key)
	{
		m_SyncObject();
		Delete(_Key);
	}

	static void AllDelete()
	{
		m_ResourceContainer.clear();
	}


public:
	static KPTR<T> Create(const KGameString& _name = L"")
	{
		T* NewT = new T();
		NewT->TypeSetting<T>();
		NewT->SetName(_name);
		NewT->InsertResource();
		return NewT;
	}

	static KPTR<T> CreateSync(const KGameString& _name = L"")
	{
		m_SyncObject();
		return Create(_name);
	}

public:
	KGameResourceBase() :m_Path(KGameString())
	{
	}

	virtual ~KGameResourceBase()
	{
	}
};

template<typename T>
class KGameResource : public KGameResourceBase<T>, public KGameFileBase
{

protected:
	KGameString m_Path;
};


template<typename T>
std::unordered_multimap<KGameString, KPTR<T>, KGameStringHash> KGameResourceBase<T>::m_ResourceContainer;
template<typename T>
KCriticalSection KGameResourceBase<T>::m_SyncObject;