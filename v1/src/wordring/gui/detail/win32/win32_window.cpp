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

#include <wordring/debug.h>

#include <wordring/gui/detail/win32/win32_window_service.h>

#include <wordring/gui/detail/win32/win32_window.h>
#include <wordring/gui/window.h>

#include <wordring/geometry/shape.h>

#include <wordring/gui/detail/win32/win32_canvas.h>
#include <wordring/gui/canvas.h>

#include <Windows.h>
#include <windowsx.h>

#include <string>
#include <cassert>

using namespace wordring::gui;
using namespace wordring::gui::detail;

// 基本ウィンドウ -------------------------------------------------------------

native_window_impl::native_window_impl() : m_hwnd(NULL), m_msg_handled(false)
{
}

native_window_impl::~native_window_impl()
{
	if(m_hwnd) { destroy(); }
}

inline void native_window_impl::set_message_handled(bool handled)
{
	m_msg_handled = handled;
}

inline bool native_window_impl::get_message_handled() const
{
	return m_msg_handled;
}

void native_window_impl::create(window* parent)
{
	HWND hparent = nullptr;
	if (parent)
	{
		hparent = static_cast<native_window_impl*>(parent->get_native_window())->m_hwnd;
	}

	m_hwnd = ::CreateWindow(
		(LPCTSTR)(DWORD)native_window_impl::g_window_class.m_atom,
		L"Test", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hparent,
		NULL,
		(HINSTANCE)0,
		this);
	assert(m_hwnd);
}

/// ウィンドウを最小化（アイコン化）する
void native_window_impl::close()
{
	assert(m_hwnd);
	BOOL result = ::CloseWindow(m_hwnd);
	assert(result != 0);
}

void native_window_impl::destroy()
{
	assert(m_hwnd);
	BOOL result = ::DestroyWindow(m_hwnd);
	assert(result != 0);
	m_hwnd = nullptr;
}

void native_window_impl::show()
{
	assert(m_hwnd);
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
}

void native_window_impl::hide()
{
	assert(m_hwnd);
	::ShowWindow(m_hwnd, SW_HIDE);
}

native_window* native_window_impl::get_parent()
{
	assert(m_hwnd);
	HWND hwnd = ::GetAncestor(m_hwnd, GA_PARENT);
	native_window* result = win32_window_service_impl::find(hwnd);
	assert(result != nullptr);

	return result;
}

void native_window_impl::set_parent(native_window* parent)
{
	assert(m_hwnd);
	assert(parent);
	native_window_impl* p = static_cast<native_window_impl*>(parent);
	::SetParent(m_hwnd, p->m_hwnd);
}

void native_window_impl::set_size(size_int size)
{
	assert(m_hwnd);
	UINT flags = SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	BOOL result = ::SetWindowPos(m_hwnd, 0, 0, 0, size.w, size.h, flags);
	assert(result != 0);
}

size_int native_window_impl::get_size() const
{
	assert(m_hwnd);
	RECT rect;
	BOOL result = ::GetClientRect(m_hwnd, &rect);
	assert(result != 0);
	return size_int(rect.right, rect.bottom);
}

void native_window_impl::set_position(point_int point)
{
	assert(m_hwnd);
	UINT flags = SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	BOOL result = ::SetWindowPos(m_hwnd, 0, point.x, point.y, 0, 0, flags);
	assert(result != 0);
}

point_int native_window_impl::get_position() const
{
	assert(m_hwnd);
	RECT rect;
	BOOL result = ::GetWindowRect(m_hwnd, &rect);
	assert(result != 0);
	//return size_int(rect.right, rect.bottom);
	return point_int(rect.left, rect.top);
}

void native_window_impl::set_text(std::string text)
{
	assert(false);
	BOOL result = ::SetWindowTextA(m_hwnd, text.c_str());
	assert(result != 0);
}

void native_window_impl::set_text(std::wstring text)
{
	BOOL result = ::SetWindowTextW(m_hwnd, text.c_str());
	assert(result != 0);
}

// メッセージ・ハンドラ -------------------------------------------------------

/// 親ウィンドウからコールバックされる（プロシージャから直接のハンドラではない）
void native_window_impl::do_command(int id, UINT codeNotify)
{

}

void native_window_impl::do_create()
{
	get_window()->do_create();
}

void native_window_impl::do_destroy()
{
	get_window()->do_destroy();
}

void native_window_impl::do_paint(HDC hdc)
{
	std::unique_ptr<detail::native_canvas> cv(new native_canvas_impl(hdc));
	get_window()->do_paint(canvas(std::move(cv)));
}



/// ウィンドウ・プロシージャの雛型です
LRESULT native_window_impl::WindowProc(
	HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	assert(m_hwnd == hwnd || m_hwnd == nullptr);

	m_msg_handled = false;

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
		HANDLE_MSG(hwnd, WM_PAINT, onPaint);
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
	return 0;
}

// マウス・メッセージ -----------------------------------------------------

/// ファイルがドラッグ・アンド・ドロップされた
void native_window_impl::onDropFiles(HWND hwnd, HDROP hdrop)
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

void native_window_impl::onLButtonDblClk(
	HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	int i = 0;
}

void native_window_impl::onLButtonDown(
	HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
}

void native_window_impl::onLButtonUp(HWND hwnd, int x, int y, UINT keyFlags){}

int native_window_impl::onMouseActivate(
	HWND hwnd, HWND hwndTopLevel, UINT codeHitTest, UINT msg)
{
	return MA_ACTIVATE;
}

void native_window_impl::onMouseMove(HWND hwnd, int x, int y, UINT keyFlags){}

void native_window_impl::onMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{

}

void native_window_impl::onRButtonDblClk(
	HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags){}

void native_window_impl::onRButtonDown(
	HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags){}

void native_window_impl::onRButtonUp(HWND hwnd, int x, int y, UINT flags){}

// キーボード・メッセージ -------------------------------------------------

void native_window_impl::onChar(HWND hwnd, TCHAR ch, int cRepeat){}

void native_window_impl::onDeadChar(HWND hwnd, TCHAR ch, int cRepeat){}

void native_window_impl::onHotKey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk){}

void native_window_impl::onKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

void native_window_impl::onKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

void native_window_impl::onSysChar(HWND hwnd, TCHAR ch, int cRepeat){}

void native_window_impl::onSysDeadChar(HWND hwnd, TCHAR ch, int cRepeat){}

void native_window_impl::onSysKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

void native_window_impl::onSysKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

// 一般メッセージ -------------------------------------------------------------

void native_window_impl::onActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized){}

void native_window_impl::onActivateApp(HWND hwnd, BOOL fActivate, DWORD dwThreadId){}

void native_window_impl::onClose(HWND hwnd){}

// 子ウィンドウで発生したコマンド(ボタンが押された等)は、親ウィンドウに送られ
// ます。
// このライブラリでは、子ウィンドウ自身にコマンドを処理させるので、子ウィンドウ
// のラッパーを呼び出します。
void native_window_impl::onCommand(
	HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	// wはコントロール自身、thisはコントロールを載せているウィンドウ
	native_window_impl* w = win32_window_service_impl::find(hwndCtl);
	assert(w);
	// wは自分自身の機能を知っているので適切に処理できる
	w->do_command(id, codeNotify);

	set_message_handled(true);
}

void native_window_impl::onCompacting(HWND hwnd, UINT compactRatio){}

BOOL native_window_impl::onCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds)
{
	return 0;
}

BOOL native_window_impl::onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	do_create();
	return 0; // 0を返すとウィンドウを生成する。生成しない場合、-1を返す。
}

void native_window_impl::onDestroy(HWND hwnd)
{
	do_destroy();
}

BOOL native_window_impl::onEraseBkgnd(HWND hwnd, HDC hdc)
{
	return 0;
}

void native_window_impl::onKillFocus(HWND hwnd, HWND hwndNewFocus){}

void native_window_impl::onMove(HWND hwnd, int x, int y){}

void native_window_impl::onPaint(HWND hwnd)
{
	PAINTSTRUCT ps;

	HDC hdc = ::BeginPaint(hwnd, &ps);
	assert(hdc);
	do_paint(hdc);
	::EndPaint(hwnd, &ps);
}

void native_window_impl::onPower(HWND hwnd, int code){}

BOOL native_window_impl::onQueryEndSession(HWND hwnd)
{
	return 0;
}

void native_window_impl::onQuit(HWND hwnd, int exitCode){}

void native_window_impl::onSetFocus(HWND hwnd, HWND hwndOldFocus){}

void native_window_impl::onShowWindow(HWND hwnd, BOOL fShow, UINT status){}

void native_window_impl::onSize(HWND hwnd, UINT state, int cx, int cy){}

void native_window_impl::onSysCommand(HWND hwnd, UINT cmd, int x, int y){}

void native_window_impl::onTimer(HWND hwnd, UINT id){}

void native_window_impl::onWindowPosChanged(HWND hwnd, const LPWINDOWPOS lpwpos){}

BOOL native_window_impl::onWindowPosChanging(HWND hwnd, LPWINDOWPOS lpwpos)
{
	return 0;
}


WNDCLASSEX native_window_impl::window_class::create()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc =
		win32_window_class<window_class, native_window_impl>::WindowProc;
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
