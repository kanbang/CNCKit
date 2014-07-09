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
#include <wordring/gui/detail/win32/win32_window_service.h>
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

//std::map<HWND, window*> win32_message_map::g_map;

// ウィンドウ・クラス ---------------------------------------------------------


// 基本ウィンドウ -------------------------------------------------------------

native_window_impl::native_window_impl()
{
	//native_window_impl::g_window_class.m_map.set_current(this);


	//native_window_impl::g_window_class.m_map.remove_current();

	//native_window_impl::g_window_class.m_map.add(m_hwnd, this);

	


}

void native_window_impl::create(window* parent)
{
	HWND hparent = NULL;
	if (parent)
	{
		hparent = dynamic_cast<native_window_impl*>(parent->get_native_window())->m_hwnd;
	}

	m_hwnd = ::CreateWindow(
		(LPCTSTR)(DWORD)native_window_impl::g_window_class.m_atom,
		L"Test", WS_OVERLAPPEDWINDOW,
		100,
		100,
		200,
		200,
		hparent,
		NULL,
		native_window_impl::g_window_class.m_hinstance,
		this);
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
	
}

void native_window_impl::close()
{
	BOOL result = ::CloseWindow(m_hwnd);
	assert(result != 0);
}

void native_window_impl::destroy()
{
	BOOL result = ::DestroyWindow(m_hwnd);
	assert(result != 0);
}

native_window* native_window_impl::get_parent()
{
	HWND hwnd = ::GetAncestor(m_hwnd, GA_PARENT);
	native_window* result = win32_window_service_impl::find(hwnd)->get_native_window();
	assert(result != nullptr);

	return result;
}

void native_window_impl::set_parent(native_window* parent)
{
	native_window_impl* p = dynamic_cast<native_window_impl*>(parent);
	::SetParent(m_hwnd, p->m_hwnd);
}

void native_window_impl::set_size(size_int size)
{
	UINT flags = SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	BOOL result = ::SetWindowPos(m_hwnd, 0, 0, 0, size.h, size.w, flags);
	assert(result != 0);
}

size_int native_window_impl::get_size() const
{
	RECT rect;
	BOOL result = ::GetClientRect(m_hwnd, &rect);
	assert(result != 0);
	return size_int(rect.right, rect.bottom);
}

void native_window_impl::set_position(point_int point)
{
	UINT flags = SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	BOOL result = ::SetWindowPos(m_hwnd, 0, point.x, point.y, 0, 0, flags);
	assert(result != 0);
}

point_int native_window_impl::get_position() const
{
	RECT rect;
	BOOL result = ::GetWindowRect(m_hwnd, &rect);
	assert(result != 0);
	//return size_int(rect.right, rect.bottom);
	return point_int(rect.left, rect.top);
}

/// ウィンドウ・プロシージャの雛型です
LRESULT native_window_impl::WindowProc(
	HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// マウス・メッセージ -------------------------------------------------

		HANDLE_MSG(hwnd, WM_DROPFILES, onDropFiles);
		HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, onLButtonDblClk);
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, onLButtonDown);
		HANDLE_MSG(hwnd, WM_LBUTTONUP, onLButtonUp);
		HANDLE_MSG(hwnd, WM_MOUSEACTIVATE, onMouseActivate);
		HANDLE_MSG(hwnd, WM_MOUSEMOVE, onMouseMove);
		HANDLE_MSG(hwnd, WM_MOUSEWHEEL, onMouseWheel);
		HANDLE_MSG(hwnd, WM_RBUTTONDBLCLK, onRButtonDblClk);
		HANDLE_MSG(hwnd, WM_RBUTTONDOWN, onRButtonDown);
		HANDLE_MSG(hwnd, WM_RBUTTONUP, onRButtonUp);

		// キーボード ---------------------------------------------------------

		HANDLE_MSG(hwnd, WM_CHAR, onChar);
		HANDLE_MSG(hwnd, WM_DEADCHAR, onDeadChar);
		HANDLE_MSG(hwnd, WM_HOTKEY, onHotKey);
		HANDLE_MSG(hwnd, WM_KEYDOWN, onKeyDown);
		HANDLE_MSG(hwnd, WM_KEYUP, onKeyUp);
		HANDLE_MSG(hwnd, WM_SYSCHAR, onSysChar);
		HANDLE_MSG(hwnd, WM_SYSDEADCHAR, onSysDeadChar);
		HANDLE_MSG(hwnd, WM_SYSKEYDOWN, onSysKeyDown);
		HANDLE_MSG(hwnd, WM_SYSKEYUP, onSysKeyUp);

		// 一般メッセージ -----------------------------------------------------

		HANDLE_MSG(hwnd, WM_ACTIVATE, onActivate);
		HANDLE_MSG(hwnd, WM_ACTIVATEAPP, onActivateApp);
		HANDLE_MSG(hwnd, WM_CLOSE, onClose);
		HANDLE_MSG(hwnd, WM_COMMAND, onCommand);
		HANDLE_MSG(hwnd, WM_COMPACTING, onCompacting); // メモリー不足
		HANDLE_MSG(hwnd, WM_COPYDATA, onCopyData);
		//HANDLE_MSG(hwnd, WM_CREATE, onCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, onDestroy);
		HANDLE_MSG(hwnd, WM_ERASEBKGND, onEraseBkgnd);
		HANDLE_MSG(hwnd, WM_KILLFOCUS, onKillFocus);
		HANDLE_MSG(hwnd, WM_MOVE, onMove);
		//HANDLE_MSG(hwnd, WM_PAINT, onPaint);
		HANDLE_MSG(hwnd, WM_POWER, onPower); // システム中断
		HANDLE_MSG(hwnd, WM_QUERYENDSESSION, onQueryEndSession);
		HANDLE_MSG(hwnd, WM_QUIT, onQuit);
		HANDLE_MSG(hwnd, WM_SETFOCUS, onSetFocus);
		HANDLE_MSG(hwnd, WM_SHOWWINDOW, onShowWindow);
		HANDLE_MSG(hwnd, WM_SIZE, onSize);
		//HANDLE_MSG(hwnd, WM_SYSCOMMAND, onSysCommand);
		HANDLE_MSG(hwnd, WM_TIMER, onTimer);
		HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGED, onWindowPosChanged);
		HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGING, onWindowPosChanging);

	}

	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);

	return 0;
}

// マウス・メッセージ -----------------------------------------------------

/// ファイルがドラッグ・アンド・ドロップされた
void onDropFiles(HWND hwnd, HDROP hdrop)
{
	//WinT* pT = static_cast<WinT*>(win32_window_service_impl::find(hwnd));
	//assert(pT);

	UINT n = ::DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0); // ファイル数
	assert(n);

	UINT j = MAX_PATH;
	std::unique_ptr<TCHAR[]> sz(new TCHAR[j]);
	for (UINT i = 0; i < n; i++)
	{
		UINT k = ::DragQueryFile(hdrop, i, NULL, 0); // 文字数
		if (j < k)
		{
			sz.reset(new TCHAR[k]);
			j = k;
		}
		k = ::DragQueryFile(hdrop, i, sz.get(), j);

		std::basic_string<TCHAR> str(sz.get(), sz.get() + k);
		// todo: Windowコールバック
	}
}

void onLButtonDblClk(
	HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	int i = 0;
}

void onLButtonDown(
	HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
}

void onLButtonUp(HWND hwnd, int x, int y, UINT keyFlags){}

int onMouseActivate(
	HWND hwnd, HWND hwndTopLevel, UINT codeHitTest, UINT msg)
{
	return MA_ACTIVATE;
}

void onMouseMove(HWND hwnd, int x, int y, UINT keyFlags){}

void onMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{

}

void onRButtonDblClk(
	HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags){}

void onRButtonDown(
	HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags){}

void onRButtonUp(HWND hwnd, int x, int y, UINT flags){}

// キーボード・メッセージ -------------------------------------------------

void onChar(HWND hwnd, TCHAR ch, int cRepeat){}

void onDeadChar(HWND hwnd, TCHAR ch, int cRepeat){}

void onHotKey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk){}

void onKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

void onKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

void onSysChar(HWND hwnd, TCHAR ch, int cRepeat){}

void onSysDeadChar(HWND hwnd, TCHAR ch, int cRepeat){}

void onSysKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

void onSysKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

// 一般メッセージ ---------------------------------------------------------

void onActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized){}

void onActivateApp(HWND hwnd, BOOL fActivate, DWORD dwThreadId){}

void onClose(HWND hwnd){}

void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify){}

void onCompacting(HWND hwnd, UINT compactRatio){}

BOOL onCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds)
{
	return 0;
}

BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	/*
	WinT* pT = (WinT*)lpCreateStruct->lpCreateParams;

	win32_message_map::assign(hwnd, pT);
	static_cast<ImplT*>(pT->get_native_window())->m_hwnd = hwnd;

	pT->onCreate();
	*/
	return 0;
}

void onDestroy(HWND hwnd){}

BOOL onEraseBkgnd(HWND hwnd, HDC hdc)
{
	return 0;
}

void onKillFocus(HWND hwnd, HWND hwndNewFocus){}

void onMove(HWND hwnd, int x, int y){}

void onPaint(HWND hwnd){}

void onPower(HWND hwnd, int code){}

BOOL onQueryEndSession(HWND hwnd)
{
	return 0;
}

void onQuit(HWND hwnd, int exitCode){}

void onSetFocus(HWND hwnd, HWND hwndOldFocus){}

void onShowWindow(HWND hwnd, BOOL fShow, UINT status){}

void onSize(HWND hwnd, UINT state, int cx, int cy){}

void onSysCommand(HWND hwnd, UINT cmd, int x, int y){}

void onTimer(HWND hwnd, UINT id){}

void onWindowPosChanged(HWND hwnd, const LPWINDOWPOS lpwpos){}

BOOL onWindowPosChanging(HWND hwnd, LPWINDOWPOS lpwpos)
{
	return 0;
}


WNDCLASSEX native_window_impl::window_class::create()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = win32_window_class<window_class, native_window_impl>::WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(native_window_impl*);
	wcex.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("native_window_impl_class");
	wcex.hIconSm = NULL;

	return wcex;
}

native_window_impl::window_class native_window_impl::g_window_class;











//native_container_window_impl::window_class native_container_window_impl::g_window_class;









/*
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


*/


#endif // _WIN32
