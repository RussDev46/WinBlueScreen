#include "WinGUI.h"
#include "resource.h"
#include "blueScreen.h"

/**
 * @file WinGUI.cpp
 *
 * @author RussDev
 * GitHub: https://github.com/RussDev46
 *
 */

Window::Window()
{
	try
	{
		this->initNativeWindowObj();
		this->createNativeControls();
	}
	catch (const std::exception& e)
	{
		MessageBoxW(NULL, ExeptToWSTR(e.what()), L"Error", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
}

Window::~Window(){}

int Window::Run()
{
	MSG _msg{};
	ShowWindow(this->hWnd, SW_SHOWDEFAULT);
	UpdateWindow(this->hWnd);

	while (GetMessage(&_msg, nullptr, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}

	return static_cast<int>(_msg.wParam);

}

void Window::initNativeWindowObj()
{
	using std::runtime_error;
	using namespace std::string_literals;

	Window::hbrBackgroundColor = CreateSolidBrush(RGB(30, 144, 255));

	WNDCLASSEX _wc{ sizeof(WNDCLASSEX) };
	_wc.cbWndExtra		= 0;
	_wc.cbClsExtra		= 0;
	_wc.lpszMenuName	= NULL;
	_wc.style			= CS_HREDRAW | CS_VREDRAW;
	_wc.lpfnWndProc		= Window::applicationProc;
	_wc.hInstance		= GetModuleHandle(NULL);
	_wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	_wc.hIcon			= LoadIconA(_wc.hInstance, MAKEINTRESOURCE( IDI_ICON1));
	_wc.hIconSm			= LoadIcon(_wc.hInstance, IDI_APPLICATION);
	_wc.lpszClassName	= Window::AppClassName;
	_wc.hbrBackground	= Window::hbrBackgroundColor;

	if (!RegisterClassEx(&_wc)) {
		throw runtime_error("Error, can't register main window class"s);
	}

	RECT _windowRC{0, 0, this->AppWidth, this->AppHeight};
	AdjustWindowRect(&_windowRC, WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE, false);

	this->hWnd = CreateWindowExW(
		0,
		this->AppClassNameW,
		this->AppName,
		WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE,
		(GetSystemMetrics(SM_CXSCREEN) - _windowRC.right) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - _windowRC.bottom) / 2,
		_windowRC.right, _windowRC.bottom, nullptr, nullptr, nullptr, this
	);

	if (!this->hWnd) {
		throw runtime_error("Error, can't create main window"s);
	}
	
}

void Window::createNativeControls()
{
	using std::runtime_error;
	using namespace std::string_literals;

	//Title				":("
	this->hfFont = CreateFont(
		140, 0, 0, 0, 0,
		0, 0, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH,
		"Segoe UI"
	);

	this->hTitleText = CreateWindowEx(
		WS_EX_TRANSPARENT,
		"static",
		":(",
		WS_VISIBLE | WS_CHILD | SS_LEFT,
		60, 20, 400, 300,
		this->hWnd,
		reinterpret_cast<HMENU>(IDC_ID::TEXT_TITLE_ID),
		nullptr,
		nullptr
	);

	SendMessageW(this->hTitleText, WM_SETFONT, reinterpret_cast<WPARAM>(hfFont), TRUE);

	//Default Text "..."
	this->hfFont = CreateFont(
		50, 0, 0, 0, 0,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH,
		"Segoe UI"
	);

	this->hDefaultText = CreateWindowEx(
		WS_EX_TRANSPARENT,
		"static",
		"Your PC ran into a problem and needs to restart. We're just collecting some error info, and then we'll rester for you.",
		WS_VISIBLE | WS_CHILD | SS_LEFT,
		60, 160, 950, 300,
		this->hWnd,
		reinterpret_cast<HMENU>(IDC_ID::TEXT_DEFAULT_ID),
		nullptr,
		nullptr
	);

	SendMessage(this->hDefaultText, WM_SETFONT, reinterpret_cast<WPARAM>(hfFont), TRUE);
	
	//Button "Start"
	this->hfFont = CreateFont(
		50, 0, 0, 0,
		FW_BOLD,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH,
		"Segoe UI"
	);

	this->hRunButton = CreateWindowEx(
		0,
		"button",
		"Start",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		750, 400, 200, 100,
		this->hWnd,
		reinterpret_cast<HMENU>(IDC_ID::BUTTON_START_ID),
		nullptr,
		nullptr
	);
	
	SendMessage(this->hRunButton, WM_SETFONT, reinterpret_cast<WPARAM>(hfFont), TRUE);

}

LRESULT CALLBACK Window::applicationProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* winGuiWindow{};

	if (uMsg == WM_NCCREATE)
	{
		winGuiWindow = static_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);

		if (SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(winGuiWindow)) == 0)
		{
			if (GetLastError() != 0)
			{
				return FALSE;
				
			}
		}
		winGuiWindow->hWnd = hWnd;

	}
	else {
		winGuiWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}
	
	if (winGuiWindow) {
		return winGuiWindow->windowProc(hWnd, uMsg, wParam, lParam);
	}
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdcStatic = NULL;

	switch (uMsg)
	{
	case WM_CTLCOLORSTATIC:
	{
		hdcStatic = reinterpret_cast<HDC>(wParam);

		SetTextColor(hdcStatic, RGB(255, 255, 255));
		SetBkMode(hdcStatic, TRANSPARENT);

		return reinterpret_cast<LRESULT>(GetStockObject(NULL_BRUSH));
	}
	break;
	case WM_CTLCOLORBTN:
	{
		COLORREF colorref;
		HBRUSH hBrushBtn;

		colorref = RGB(30, 144, 255);
		hBrushBtn = CreateSolidBrush(colorref);

		return reinterpret_cast<LRESULT>(hBrushBtn);
	}
	break;
	case WM_CREATE:
	{
		g_hbmBall = LoadBitmapA(GetModuleHandle(NULL), MAKEINTRESOURCEA(IDB_BITMAP1));

	}
	return 0;
	case WM_PAINT:
	{
		BITMAP bm{};
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hWnd, &ps);
		HDC hdcMem = CreateCompatibleDC(hdc);
		
		hbmOld = reinterpret_cast<HBITMAP>(SelectObject(hdcMem, g_hbmBall));

		GetObject(g_hbmBall, sizeof(bm), &bm);
		BitBlt(hdc, 60, 360, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, hbmOld);
		
		DeleteDC(hdcMem);
		EndPaint(hWnd, &ps);
		
	}
	return 0;
	case WM_COMMAND:
	{
		switch (static_cast<IDC_ID>(LOWORD(wParam))) {
		case IDC_ID::BUTTON_START_ID:
			int valMSGBox = MessageBox(hWnd, "are you sure you want to do it?", "are you sure?", MB_ICONWARNING | MB_YESNO);

			if (valMSGBox == IDYES) {

				BlueScreen bs;
				bs.run();

			}
			else {
				return 0;
			}
		}
		return 0;
	}
	case WM_CLOSE:
	case WM_DESTROY:
	{
		DeleteObject(g_hbmBall);
		PostQuitMessage(EXIT_SUCCESS);
	}
	return 0;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

LPCWSTR Window::ExeptToWSTR(const char* exept)
{
	std::string cErrorString	= exept;
	std::wstring wErrorString	= std::wstring(cErrorString.begin(), cErrorString.end());
	LPCWSTR wstrErrorString		= wErrorString.c_str();

	return wstrErrorString;
}
