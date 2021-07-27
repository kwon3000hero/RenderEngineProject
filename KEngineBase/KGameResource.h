#pragma once
#include "KGAMENAME.h"
#include "KGameReference.h"
#include "KGameType.h"
#include "KGameResourceManager.h"
#include "KGAMESTRING.h"
#include "KGAMEFILEBASE.h"

template<typename Ts>
class KGameResourceManager;

template<typename T>
class KGameResource : public KGameReference, public KGameName, public KGameType
{
	friend KGameResourceManager<T>;

protected:
	KGameString m_Path;


public:
	KGameResource() :m_Path(KGameString())
	{
	}

	virtual ~KGameResource()
	{
	}

protected:
	void InsertResource()
	{
		KGameResourceManager<T>::Instance().SyncObject();
#ifdef _DEBUG
		KGameString name = Name();
		KGameResourceManager<T>::Instance().m_ResourceContainer.insert(std::unordered_multimap<KGameString, KPTR<T>, KGameStringHash>::value_type(name, (T*)this));
#else
		KGameResourceManager<T>::Instance().m_ResourceContainer.insert(std::unordered_multimap<KGameString, KPTR<T>, KGameStringHash>::value_type(Name(), (T*)this));

#endif
	}
};

