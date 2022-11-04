#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <Commctrl.h>
#include <string>
#include <exception>
#include <stdexcept>

#include "blueScreen.h"

/**
 * @file winGUI.h
 *
 * @author RussDev
 * GitHub: https://github.com/RussDev46
 *
 */

class Window
{
	enum class IDC_ID
	{
		TEXT_TITLE_ID,
		TEXT_DEFAULT_ID,
		BUTTON_START_ID,
	};
public:
	explicit Window();
	~Window();

	int Run();

private:
	static LRESULT CALLBACK applicationProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT	CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void initNativeWindowObj();
	void createNativeControls();
	LPCWSTR ExeptToWSTR(const char* exept);

private:
	const LPCWSTR	AppName{ L"BlueScreen" };
	const LPCWSTR	AppClassNameW{ L"BlueScreenn_class" };
	const LPCSTR	AppClassName{ "BlueScreenn_class" };
	const int			AppWidth{ 1072 };
	const int			AppHeight{ 600 };

	HWND	hWnd{};
	HWND	hTitleText{}, hDefaultText{}, hRunButton{}, hTest{};
	HBRUSH	hbrBackgroundColor{};
	HFONT	hfFont{};

	HBITMAP g_hbmBall = NULL;
	HBITMAP hbmOld = NULL;
};


