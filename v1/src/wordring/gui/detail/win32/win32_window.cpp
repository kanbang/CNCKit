/**
 * @file    wordring/gui/detail/win32/win32_window.cpp
 *
 * @brief   ウィンドウの環境固有実装ファイル
 *
 * @details
 *          win32専用のウィンドウ実装。\n
 *          pimplによって実装を隠ぺいします。
 *
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */


#ifdef _WIN32

#include <wordring/gui/detail/win32/win32_window.h>
#include <wordring/gui/window.h>
#include <wordring/geometry/shape.h>

#include <Windows.h>
#include <windowsx.h>

#include <algorithm>
#include <cassert>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

using namespace wordring::gui;
using namespace wordring::gui::detail;

std::map<HWND, window*> win32_message_map::g_map;

// ウィンドウ・クラス ---------------------------------------------------------


// 基本ウィンドウ -------------------------------------------------------------

win32_window_impl::win32_window_impl()
{
	//win32_window_impl::g_window_class.m_map.set_current(this);


	//win32_window_impl::g_window_class.m_map.remove_current();

	//win32_window_impl::g_window_class.m_map.add(m_hwnd, this);

	


}

void win32_window_impl::create(window* parent)
{
	HWND hparent = NULL;
	if (parent)
	{
		hparent = dynamic_cast<win32_window_impl*>(parent->get_native_window())->m_hwnd;
	}

	m_hwnd = ::CreateWindow(
		(LPCTSTR)(DWORD)win32_window_impl::g_window_class.m_atom,
		L"Test", WS_OVERLAPPEDWINDOW,
		100,
		100,
		200,
		200,
		hparent,
		NULL,
		win32_window_impl::g_window_class.m_hinstance,
		this);
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
}

void win32_window_impl::close()
{
	BOOL result = ::CloseWindow(m_hwnd);
	assert(result != 0);
}

void win32_window_impl::destroy()
{
	BOOL result = ::DestroyWindow(m_hwnd);
	assert(result != 0);
}

native_window* win32_window_impl::get_parent()
{
	HWND hwnd = ::GetAncestor(m_hwnd, GA_PARENT);
	native_window* result = win32_message_map::find(hwnd)->get_native_window();
	assert(result != nullptr);

	return result;
}

void win32_window_impl::set_parent(native_window* parent)
{
	win32_window_impl* p = dynamic_cast<win32_window_impl*>(parent);
	::SetParent(m_hwnd, p->m_hwnd);
}

void win32_window_impl::set_size(size_int size)
{
	UINT flags = SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	BOOL result = ::SetWindowPos(m_hwnd, 0, 0, 0, size.h, size.w, flags);
	assert(result != 0);
}

size_int win32_window_impl::get_size() const
{
	RECT rect;
	BOOL result = ::GetClientRect(m_hwnd, &rect);
	assert(result != 0);
	return size_int(rect.right, rect.bottom);
}

void win32_window_impl::set_position(point_int point)
{
	UINT flags = SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	BOOL result = ::SetWindowPos(m_hwnd, 0, point.x, point.y, 0, 0, flags);
	assert(result != 0);
}

point_int win32_window_impl::get_position() const
{
	RECT rect;
	BOOL result = ::GetWindowRect(m_hwnd, &rect);
	assert(result != 0);
	//return size_int(rect.right, rect.bottom);
	return point_int(rect.left, rect.top);
}

LRESULT win32_window_impl::window_proc(
	HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
/*
// マウス・メッセージ ---------------------------------------------------------

void win32_window_impl::onDropFiles(HWND hwnd, HDROP hdrop)
{

}

void win32_window_impl::onLButtonDblClk(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}

void win32_window_impl::onLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}

void win32_window_impl::onLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{

}

int win32_window_impl::onMouseActivate(HWND hwnd, HWND hwndTopLevel, UINT codeHitTest, UINT msg)
{
	return MA_ACTIVATE;
}

void win32_window_impl::onMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{

}

void win32_window_impl::onMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{

}

void win32_window_impl::onRButtonDblClk(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}

void win32_window_impl::onRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}

void win32_window_impl::onRButtonUp(HWND hwnd, int x, int y, UINT flags)
{

}

// キーボード・メッセージ -----------------------------------------------------

void win32_window_impl::onChar(HWND hwnd, TCHAR ch, int cRepeat)
{

}

void win32_window_impl::onDeadChar(HWND hwnd, TCHAR ch, int cRepeat)
{

}

void win32_window_impl::onHotKey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk)
{

}

void win32_window_impl::onKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{

}

void win32_window_impl::onKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{

}

void win32_window_impl::onSysChar(HWND hwnd, TCHAR ch, int cRepeat)
{

}

void win32_window_impl::onSysDeadChar(HWND hwnd, TCHAR ch, int cRepeat)
{

}

void win32_window_impl::onSysKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{

}

void win32_window_impl::onSysKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{

}

// 一般メッセージ -------------------------------------------------------------


void win32_window_impl::onActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{

}

void win32_window_impl::onActivateApp(HWND hwnd, BOOL fActivate, DWORD dwThreadId)
{

}

void win32_window_impl::onClose(HWND hwnd)
{

}

void win32_window_impl::onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}

void win32_window_impl::onCompacting(HWND hwnd, UINT compactRatio)
{

}

BOOL win32_window_impl::onCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds)
{
	return 0;
}

BOOL win32_window_impl::onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	//get_window()->on_create();
	return 0;
}

void win32_window_impl::onDestroy(HWND hwnd)
{
	win32_message_map::remove_window(m_hwnd);
}

BOOL win32_window_impl::onEraseBkgnd(HWND hwnd, HDC hdc)
{
	return 0;

}

void win32_window_impl::onKillFocus(HWND hwnd, HWND hwndNewFocus)
{

}

void win32_window_impl::onMove(HWND hwnd, int x, int y)
{

}

void win32_window_impl::onPaint(HWND hwnd)
{

}

void win32_window_impl::onPower(HWND hwnd, int code)
{

}

BOOL win32_window_impl::onQueryEndSession(HWND hwnd)
{
	return 0;

}

void win32_window_impl::onQuit(HWND hwnd, int exitCode)
{

}

void win32_window_impl::onSetFocus(HWND hwnd, HWND hwndOldFocus)
{

}

void win32_window_impl::onShowWindow(HWND hwnd, BOOL fShow, UINT status)
{

}

void win32_window_impl::onSize(HWND hwnd, UINT state, int cx, int cy)
{

}

void win32_window_impl::onSysCommand(HWND hwnd, UINT cmd, int x, int y)
{

}

void win32_window_impl::onTimer(HWND hwnd, UINT id)
{

}

void win32_window_impl::onWindowPosChanged(HWND hwnd, const LPWINDOWPOS lpwpos)
{

}

BOOL win32_window_impl::onWindowPosChanging(HWND hwnd, LPWINDOWPOS lpwpos)
{
	return 0;

}

*/


/*
LRESULT win32_window_impl::on_command(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)lParam;
	assert(hwnd);
	window* win = win32_message_map::find_window(hwnd)->get_window();
	win->on_click();

	return 0;
}

LRESULT win32_window_impl::on_create(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	return get_window()->on_create() ? 0 : -1;
}

LRESULT win32_window_impl::on_click()
{
	get_window()->on_click();
}
*/

WNDCLASSEX win32_window_impl::window_class::create()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = win32_window_class<window_class, window, win32_window_impl>::WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(win32_window_impl*);
	wcex.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("win32_window_impl_class");
	wcex.hIconSm = NULL;

	return wcex;
}

win32_window_impl::window_class win32_window_impl::g_window_class;



// win32_control_window_impl --------------------------------------------------

win32_control_window_impl::win32_control_window_impl()
{
}

void win32_control_window_impl::create(window* parent)
{
	HWND hparent = NULL;
	if (parent)
	{
		hparent = dynamic_cast<win32_control_window_impl*>(parent->get_native_window())->m_hwnd;
	}

	m_hwnd = ::CreateWindow(
		(LPCTSTR)(DWORD)win32_control_window_impl::g_window_class.m_atom,
		L"Tes", WS_CHILD | WS_VISIBLE,
		100,
		100,
		200,
		200,
		hparent,
		NULL,
		win32_control_window_impl::g_window_class.m_hinstance,
		this);
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
}

WNDCLASSEX win32_control_window_impl::window_class::create()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = win32_window_class<window_class, window, win32_control_window_impl>::WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(win32_control_window_impl*);
	wcex.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("win32_control_window_impl");
	wcex.hIconSm = NULL;

	return wcex;
}

win32_control_window_impl::window_class win32_control_window_impl::g_window_class;


// win32_container_window_impl ------------------------------------------------

void win32_container_window_impl::create(window* parent)
{
	HWND hparent = NULL;
	if (parent)
	{
		hparent = dynamic_cast<win32_window_impl*>(parent->get_native_window())->m_hwnd;
	}
	m_hwnd = ::CreateWindow(
		(LPCTSTR)(DWORD)win32_container_window_impl::g_window_class.m_atom,
		L"win32_container_window_impl", WS_OVERLAPPEDWINDOW,
		100,
		100,
		200,
		200,
		hparent,
		NULL,
		win32_container_window_impl::g_window_class.m_hinstance,
		this);
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
}

WNDCLASSEX win32_container_window_impl::window_class::create()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = win32_window_class<window_class, window, win32_container_window_impl>::WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(win32_container_window_impl*);
	wcex.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("win32_container_window_impl");
	wcex.hIconSm = NULL;

	return wcex;
}


win32_container_window_impl::window_class win32_container_window_impl::g_window_class;

void win32_button_window_impl::create(window* parent)
{
	HWND hparent = NULL;
	if (parent)
	{
		hparent = dynamic_cast<win32_window_impl*>(parent->get_native_window())->m_hwnd;
	}

	m_hwnd = ::CreateWindow(
		TEXT("BUTTON"),
		L"Button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		60,
		60,
		60,
		60,
		hparent,
		NULL,
		win32_window_impl::g_window_class.m_hinstance,
		NULL);

	win32_message_map::assign(m_hwnd, parent);
	//::ShowWindow(m_hwnd, SW_SHOW);
	//::UpdateWindow(m_hwnd);

}





#endif // _WIN32
