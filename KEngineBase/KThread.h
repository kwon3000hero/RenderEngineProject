#pragma once
#include <Windows.h>
#include <map>
#include "KGAMESTRING.h"
#include "KCriticalSection.h"
#include <functional>
#include "KGameResource.h"

class KThread : public KGameResourceBase<KThread>
{
private:
	static std::map<KGameString, KPTR<KThread>> m_ThreadContainer;

private:
	KCriticalSection m_ThreadSync;

	bool m_IsPlay;
	HANDLE m_ThreadHandle;
	void* m_pData;
	std::function<void(KThread*)> m_Function;

public:
	template<typename T>
	static void Start(const KGameString& _name, void(T::* _func)(KThread*), T* _pObject)
	{
		m_SyncObject();
		KPTR<KThread> ptr = Find(_name);

		if (nullptr == ptr.get())
		{
			AssertMsg(L"존재하지 않는 쓰레드로 콜백 진행 불가.");
		}

		ptr->Start(_func, _pObject);
	}

	template<typename T>
	static void Start(const KGameString& _name, void(T::* _func)(KThread*))
	{
		m_SyncObject();
		KPTR<KThread> ptr = Find(_name);

		if (nullptr == ptr.get())
		{
			AssertMsg(L"존재하지 않는 쓰레드로 콜백 진행 불가.0");
		}

		ptr->Start(_func);
	}

public:
	template<typename T>
	void Start(void(T::* _func)(KThread*), T* _pObject)
	{
		m_SyncObject();
		if (nullptr == _func)
		{
			AssertMsg(L"존재하지 않는 쓰레드로 콜백 진행 불가.1");
		}

		m_Function = std::bind(_func, _func, std::placeholders::_1);
		Start();
		//Start(_pObject);
	}

	void Start(void(* _func)(KThread*))
	{
		m_SyncObject();
		if (nullptr == _func)
		{
			AssertMsg(L"존재하지 않는 쓰레드로 콜백 진행 불가.2");
		}

		m_Function = std::bind(_func, std::placeholders::_1);
		Start();
	}

private:
	static unsigned __stdcall ThreadBaseFunction(void* _arg);

private:
	void Start();

public:
	void SetData(void* _pData)
	{
		m_pData = _pData;
	}

	void* Data()
	{
		return m_pData;
	}

public:
	bool IsPlay()
	{
		return m_IsPlay;
	}

public:
	KThread();
	~KThread();
};

