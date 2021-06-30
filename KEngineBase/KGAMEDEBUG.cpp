#include "KGAMEDEBUG.h"
#include <Windows.h>
#include <stdio.h>
#include <iostream>

bool KGameDebug::IsConsole = false;
KGameDebug::KGameDebugStarter KGameDebug::Inst;

void  KGameDebug::LeakCheck()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

void KGameDebug::ConsoleOn()
{
	setlocale(LC_ALL, "Kor");

	if (AllocConsole())
	{
		AllocConsole();
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		FILE* acStreamOut;
		FILE* acStreamIn;
		freopen_s(&acStreamOut, "CONOUT$", "wt", stdout);
		freopen_s(&acStreamIn, "CONIN$", "r", stdin);
		std::locale Loc = std::locale("kor");
		std::wcout.imbue(Loc);
		std::wcout << L"Console Debug Start OK" << std::endl;
	}
	KGameDebug::IsConsole = true;
}

void KGameDebug::ConsoleOff() {
	if (true == IsConsole)
	{
		FreeConsole();
	}
}