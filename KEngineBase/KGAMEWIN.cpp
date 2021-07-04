#include "KGAMEWIN.h"

bool KGameWindow::m_bProgress = true;
KPTR<KGameWindow> KGameWindow::m_MainWindow = static_cast<KWeakPTR<KGameWindow>>(0);

std::set<KGameString> KGameWindow::m_classNameContainer;
std::map<KGameString, KPTR<KGameWindow>> KGameWindow::m_winContainer;
KGameWindow::KGAMEWINCREATER KGameWindow::instance = KGameWindow::KGAMEWINCREATER();

KGameWindow::KGAMEWINCREATER::KGAMEWINCREATER()
{
	RegWinClass(L"DefWin");
}

KGameWindow::KGAMEWINCREATER::~KGAMEWINCREATER()
{
	DeleteAllWin();
}

LRESULT CALLBACK KGameWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
	{
		m_bProgress = false;
	}
	break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

void KGameWindow::RegWinClass(const wchar_t* name)
{
	WNDCLASSEXW wclassEx;

	memset(&wclassEx, 0, sizeof(WNDCLASSEXW));

	wclassEx.style = CS_HREDRAW | CS_VREDRAW;
	wclassEx.cbSize = sizeof(WNDCLASSEX);
	wclassEx.lpfnWndProc = WndProc;
	wclassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wclassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wclassEx.lpszClassName = name;

	if (m_classNameContainer.end() != m_classNameContainer.find(name))
	{
		BOOM;
	}

	if (0 == RegisterClassExW(&wclassEx))
	{
		BOOM;
	}

	m_classNameContainer.insert(name);
}

KPTR<KGameWindow> KGameWindow::FindWin(const KGameString& name)
{
	if (m_winContainer.end() == m_winContainer.find(name))
	{
		BOOM;
	}

	return m_winContainer[name];
}

KPTR<KGameWindow> KGameWindow::CreateWin(const wchar_t* title, const wchar_t* className)
{
	if (m_classNameContainer.end() == m_classNameContainer.find(className))
	{
		BOOM;
	}

	if (m_winContainer.end() != m_winContainer.find(title))
	{
		BOOM;
	}

	KPTR<KGameWindow> pWin = make_KPTR<KGameWindow>();

	pWin->m_HWND = CreateWindow(className, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, nullptr, nullptr);

	pWin->m_dc = GetDC(pWin->m_HWND);

	if (nullptr == pWin->m_HWND || nullptr == pWin->m_dc)
	{
		return static_cast<KPTR<KGameWindow>>(0);
	}

	pWin->Show();
	pWin->Update();

	m_winContainer.insert(std::map<std::wstring, KPTR<KGameWindow>>::value_type(title, pWin.get()));

	if (nullptr == m_MainWindow.get())
	{
		m_MainWindow = pWin;
	}

	return pWin;
}

void KGameWindow::Progress(void(__stdcall* _ProgressFunc)(), void(__stdcall* _InitFunc)())
{
	if (nullptr == _ProgressFunc)
	{
		BOOM;
	}

	if (nullptr != _InitFunc)
	{
		_InitFunc();
	}

	MSG msg = {};
	while (m_bProgress)
	{
		if (0 == PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			_ProgressFunc();
		}
		else
		{
			DispatchMessage(&msg);
		}
	}
}


void KGameWindow::DeleteAllWin()
{
}

void KGameWindow::Show()
{
	ShowWindow(m_HWND, SW_SHOWDEFAULT);
}

void KGameWindow::Update()
{
	UpdateWindow(m_HWND);
}

void KGameWindow::Size(const KVector& size)
{
	m_size = size;

	RECT rect = { 0, 0, size.IX(), size.IY() };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	int realWidth = rect.right - rect.left;
	int realHeight = rect.bottom - rect.top;

	SetWindowPos(m_HWND, nullptr, rect.left, rect.top, realWidth, realHeight, SWP_NOMOVE);
}