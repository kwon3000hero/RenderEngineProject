#pragma once
#include <Windows.h>
#include <string>
#include <set>
#include <map>
#include "KGAMEMATH.h"
#include "KGAMEDEBUG.h"
#include "KGameReference.h"
#include "KGAMESTRING.h"

class KGameWindow : public KGameReference
{
private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	class KGameWindowCreater
	{
	private:
		friend KGameWindow;

	public:
		KGameWindowCreater();
		~KGameWindowCreater();
	};

	friend KGameWindowCreater;

private:
	static KGameWindowCreater instance;


private:
	static bool m_bProgress;

private:
	static std::set<KGameString> m_classNameContainer;
	static std::map<KGameString, KPTR<KGameWindow>> m_winContainer;

	static KWeakPTR<KGameWindow> m_MainWindow;

public:
	static KWeakPTR<KGameWindow> MainWindow()
	{
		return m_MainWindow;
	}

	HDC WINDC()
	{
		return m_dc;
	}

	HWND WINHWND()
	{
		return m_HWND;
	}

public:
	static KPTR<KGameWindow> CreateWin(const wchar_t* title, const wchar_t* className = L"DefWin");
	static KPTR<KGameWindow> FindWin(const KGameString& _Name);
	static void DeleteAllWin();

public:
	static void Progress(void(__stdcall* _ProgressFunc)(), void(__stdcall* _InitFunc)() = nullptr);

private:
	int m_style;
	HWND m_HWND;
	HDC m_dc;
	KVector m_size;

	static void RegWinClass(const wchar_t* name);
public:
	KGameWindow() :m_HWND(nullptr), m_dc(nullptr), m_size(), m_style(0)
	{

	}

private:
	~KGameWindow()
	{

	}

public:
	void Show();
	void Update();

public:
	void SetSize(int width, int height);
	void SetSize(const KVector& _Size);

public:
	KVector Size()
	{
		return m_size;
	}

};

