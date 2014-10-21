

#include "item.h"
#include "control.h"






#include <string>

#include <Windows.h>
#include <windowsx.h>

#pragma comment(lib, "usp10.lib")
#pragma comment(lib, "imm32.lib")

std::wstring text_ =
//L"\u306f\u309b"
//L"Å𠀋𠀋𠀋𡈽𡌛𡑮𠀋";
/*
L"A 漢字آ"

L"aĤĤ𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋"
L"𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋𠀋"
L"abc def"
L"آ آآ آآآ"
*/
L"𠀋あいうえおかabcdefghijklmnきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
/*L"abc defあいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
L"abc defあいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
L"abc漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字"
L"abc漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字"
L"abc defあいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
L"abc defあいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
L"abc defあいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
L"abc漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字"
L"abc漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字"
L"abc defあいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
L"abc defあいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
L"abc defあいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
L"abc漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字"
L"abc漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字"
L"abc defあいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
L"abc defあいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
L"abc defあいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめも"
L"abc漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字"
L"abc漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字漢字"
/*
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟"
L"	abc漢字123あいうえおآئثجرصمىکرـ؟";
*/
/*
L"آئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمى کـ"
//L"آئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـ"
//L"آئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـ"
//L"آئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـآئثجرصمىکرـ"
*/
;

SCRIPT_DIGITSUBSTITUTE g_sds = { 0 };

font::store g_font;

control g_control(&g_sds);




LRESULT CALLBACK WindowProc(
	HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_IME_STARTCOMPOSITION:
		g_control.get_render()->start_composition(hwnd);
		break;
	case WM_IME_COMPOSITION:
		g_control.get_render()->process_composition(hwnd, wParam, lParam);
		break;
	case WM_IME_ENDCOMPOSITION:
		g_control.get_render()->end_composition(hwnd);
		break;
	case WM_CHAR:
	{
		HDC hdc = ::GetDC(hwnd);
		//render_control c(g_sds, &g_control, g_ct, hdc);
		//c.on_char(wParam);
		g_control.on_char(wParam);
		::ReleaseDC(hwnd, hdc);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT:
			g_control.get_render()->move_caret_left();
			break;
		case VK_RIGHT:
			g_control.get_render()->move_caret_right();
			break;
		case VK_UP:
			g_control.get_render()->move_caret_up();
			break;
		case VK_DOWN:
			g_control.get_render()->move_caret_down();
			break;
		case VK_HOME:
			g_control.get_render()->move_caret_home();
			break;
		case VK_END:
			g_control.get_render()->move_caret_end();
			break;
		case VK_PRIOR:
			break;
		case VK_NEXT:
			break;
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hwnd, &ps);
		::SetMapMode(hdc, MM_TEXT);

		//render_control c(g_sds, &g_control, g_ct, hdc);
		g_control.get_render()->paint(hdc);

		::EndPaint(hwnd, &ps);
		break;
	}
	case WM_SIZE:
	{
		HDC hdc = ::GetDC(hwnd);
		//render_control c(g_sds, &g_control, g_ct, hdc);
		if (wParam != SIZE_MINIMIZED)
		{
			g_control.get_render()->set_size(
				hdc, lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
		}
		::ReleaseDC(hwnd, hdc);
		break;
	}
	case WM_SETFOCUS:
	{
		//render_control c(g_sds, &g_control, g_ct, hdc, hwnd);
		//c.set_focus();
		g_control.get_render()->set_focus(hwnd);
		break;
	}
	case WM_KILLFOCUS:
	{
		//render_control c(g_sds, &g_control, g_ct, nullptr, hwnd);
		//c.kill_focus();
		g_control.get_render()->kill_focus(hwnd);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		g_control.on_l_button_down(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
		break;
	}
	case WM_CREATE:
	{
		LRESULT result =
			::ScriptRecordDigitSubstitution(LOCALE_USER_DEFAULT, &g_sds);
		assert(result == S_OK);

		g_font = std::make_shared<font>(32, L"Meiryo");// Arial Unicode MS");

		HDC hdc = ::GetDC(hwnd);
		g_control.set_base_font(g_font);
		g_control.set_string(hdc, text_);
		//render_control c(g_sds, &g_control, g_ct, hdc);

		//c.set_string(L"");// text_);

		//c.set_style(g_font, 0, 0);
		::ReleaseDC(hwnd, hdc);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_GETFONT:
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}




int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR, int nCmdShow)
{
	HWND hwnd;
	WNDCLASS wc;
	MSG msg;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("Uniscribe");

	if (!RegisterClass(&wc)) return 0;

	hwnd = CreateWindowW(
		L"Uniscribe",
		L"Uniscribe sample",
		WS_OVERLAPPEDWINDOW,
		10, 10, 640, 480,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hwnd == NULL) return 0;

	::ShowWindow(hwnd, SW_SHOW);

	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}