#include "WinGUI.h"

/**
 * @file main.cpp
 *
 * @author RussDev
 * GitHub: https://github.com/RussDev46 
 *
 */

int CALLBACK wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	Window winApp{};
	return winApp.Run();
}
