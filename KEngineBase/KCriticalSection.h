#pragma once

#include <Windows.h>
#include <process.h>

class KAutoCriticalSection;
class KCriticalSection
{
public:
	friend KAutoCriticalSection;

private:
	CRITICAL_SECTION m_CriticalSection;

public:
	KAutoCriticalSection operator()();

public:
	KCriticalSection()
	{
		InitializeCriticalSection(&m_CriticalSection);
	}

	~KCriticalSection()
	{
		DeleteCriticalSection(&m_CriticalSection);
	}
};

class KAutoCriticalSection
{
public:
	KCriticalSection m_CriticalSection;

public:
	KAutoCriticalSection(KCriticalSection& _ref) : m_CriticalSection(_ref)
	{
		EnterCriticalSection(&m_CriticalSection.m_CriticalSection);
	}

	~KAutoCriticalSection()
	{
		LeaveCriticalSection(&m_CriticalSection.m_CriticalSection);
	}
};