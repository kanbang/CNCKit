/**
 * @file    3AxisEmulator/main.cpp
 * @brief   3軸CNCエミュレータ
 *
 * @author  Kouichi Minami
 * @date    2014/07/03
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#include <crtdbg.h>


#include <Windows.h>
#include <gl/GL.h>

#include <cstdint>
#include <iostream>

#include <wordring/gui/window_service.h>


int main()
{
	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
/*
	HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);

	WNDCLASS wndClass;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.lpszClassName = L"WindowWithConsole";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!::RegisterClass(&wndClass)) { return -1; }

	HWND hWnd = ::CreateWindow(L"WindowWithConsole", L"Test", WS_OVERLAPPEDWINDOW, 100, 100, 200, 200, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL) { return -1; }

	::ShowWindow(hWnd, SW_SHOW);
	::UpdateWindow(hWnd);
*/
	wordring::gui::window_service ws;
	ws.run();

	//std::cout << sizeof(int32_t);
	//new int;

	return 0;
}
