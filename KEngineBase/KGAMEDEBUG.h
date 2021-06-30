#pragma once
#include <crtdbg.h>
#include <assert.h>
#include <iostream>
#include "KGAMESTRING.h"

#define BOOM assert(false)

#ifdef _DEBUG
#define AssertMsg(msg) _wassert(msg, _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
#else
#define AssertMsg(msg);
#endif

class KGameDebug
{
private:

	class KGameDebugStarter
	{
		friend KGameDebug;

	public:
		KGameDebugStarter() {  }
		~KGameDebugStarter()
		{
			ConsoleOff();
		}
	};
	static KGameDebugStarter Inst;
	friend KGameDebugStarter;
	static bool IsConsole;

public:
	static void ConsoleOn();
	static void ConsoleOff();

	static void LeakCheck();

	template<typename ... Args>
	static void ConsoleText(const KGameString& _str, Args ... _arg)
	{
		wchar_t text[256];
		swprintf_s(text, _str, _arg...);
		wprintf_s(text);
	}

public:
	virtual ~KGameDebug() = 0 {}
};

