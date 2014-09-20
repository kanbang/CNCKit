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

#include <wordring/exception.h>

#include <wordring/gui/window.h>
#include <wordring/gui/shape_int.h>
#include <wordring/gui/mouse.h>
#include <wordring/gui/canvas.h>

#include <wordring/gui/detail/win32/win32_message_service.h>
#include <wordring/gui/detail/win32/win32_window.h>
#include <wordring/gui/detail/win32/win32_canvas.h>

#include <Windows.h>
#include <windowsx.h>

#include <string>
#include <cassert>

using namespace wordring::gui;
using namespace wordring::gui::detail;

// 構築・破棄 -----------------------------------------------------------------

native_window_impl::native_window_impl()
	: m_hwnd(NULL)
	, m_mouse_enter(false)
	, m_msg_handled(false)
{
}

native_window_impl::~native_window_impl()
{
	if(m_hwnd) { destroy_window(); }
}

native_window::store native_window_impl::create()
{
	return native_window::store(new native_window_impl);
}

void native_window_impl::create_window(window* parent, rect_int rc)
{
	HWND hparent = nullptr;
	if (parent)
	{
		hparent = static_cast<
			native_window_impl*>(parent->get_native())->m_hwnd;
	}

	m_hwnd = ::CreateWindowEx(
		WS_EX_TRANSPARENT,
		(LPCTSTR)(DWORD)native_window_impl::g_window_class.m_atom,
		L"native_window_impl",
		WS_OVERLAPPEDWINDOW,
		rc.pt.x,
		rc.pt.y,
		rc.size.cx,
		rc.size.cy,
		hparent,
		NULL,
		(HINSTANCE)0,
		this);
	wordring::check_assertion(m_hwnd != NULL);
}

void native_window_impl::destroy_window()
{
	assert(m_hwnd);
	BOOL result = ::DestroyWindow(m_hwnd);
	assert(result != 0);
	m_hwnd = nullptr;
}

// 情報 -----------------------------------------------------------------------

bool native_window_impl::is_created() const
{
	return m_hwnd != nullptr;
}

// 表示 -----------------------------------------------------------------------

void native_window_impl::close_window()
{
	assert(m_hwnd);
	BOOL result = ::CloseWindow(m_hwnd);
	assert(result != 0);
}

void native_window_impl::show_window()
{
	assert(m_hwnd);
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
}

void native_window_impl::hide_window()
{
	assert(m_hwnd);
	::ShowWindow(m_hwnd, SW_HIDE);
}

// 描画 -----------------------------------------------------------------------

void native_window_impl::repaint_window()
{
		wordring::check_assertion(
	::InvalidateRect(m_hwnd, NULL, FALSE) != 0);
}

void native_window_impl::repaint_window(rect_int rc_)
{
	RECT rc; rc.top = rc_.pt.y; rc.left = rc_.pt.x;
	rc.bottom = rc_.pt.y + rc_.size.cy; rc.right = rc_.pt.x + rc_.size.cx;

		wordring::check_assertion(
	::InvalidateRect(m_hwnd, &rc, FALSE) != 0);
}

// 大きさ・位置 ---------------------------------------------------------------

void native_window_impl::set_window_size(size_int size)
{
	assert(m_hwnd);
	UINT flags = SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	BOOL result = ::SetWindowPos(m_hwnd, 0, 0, 0, size.cx, size.cy, flags);
	assert(result != 0);
}

size_int native_window_impl::get_window_size() const
{
	assert(m_hwnd);
	RECT rect;
	BOOL result = ::GetClientRect(m_hwnd, &rect);
	assert(result != 0);
	return size_int(rect.right, rect.bottom);
}

void native_window_impl::set_window_position(point_int point)
{
	assert(m_hwnd);
	UINT flags = SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	BOOL result = ::SetWindowPos(m_hwnd, 0, point.x, point.y, 0, 0, flags);
	assert(result != 0);
}

point_int native_window_impl::get_window_position() const
{
	assert(m_hwnd);
	RECT rect;
	BOOL result = ::GetWindowRect(m_hwnd, &rect);
	assert(result != 0);
	return point_int(rect.left, rect.top);
}

void native_window_impl::set_window_rect(rect_int rc)
{
	assert(m_hwnd);

	UINT flags = SWP_NOOWNERZORDER | SWP_NOZORDER;
	BOOL result = ::SetWindowPos(
		m_hwnd,
		0,
		rc.pt.x,
		rc.pt.y,
		rc.size.cx,
		rc.size.cy,
		flags);
	assert(result != 0);
}

rect_int native_window_impl::get_window_rect() const
{
	assert(m_hwnd);
	RECT rc;
	BOOL result = ::GetClientRect(m_hwnd, &rc);
	assert(result != 0);
	return rect_int(
		  point_int(rc.left, rc.top)
		, size_int(rc.right - rc.left, rc.bottom - rc.top));
}

// その他 ---------------------------------------------------------------------

void native_window_impl::set_window_text(std::string text)
{
	assert(false);
	BOOL result = ::SetWindowTextA(m_hwnd, text.c_str());
	assert(result != 0);
}

void native_window_impl::set_window_text(std::wstring text)
{
	BOOL result = ::SetWindowTextW(m_hwnd, text.c_str());
	assert(result != 0);
}

mouse::state_ native_window_impl::make_mouse_state(UINT f)
{
	mouse::state_ result;

	result.shift = (f & MK_SHIFT) ? true : false;
	result.ctrl = (f & MK_CONTROL) ? true : false;
	result.left = (f & MK_LBUTTON) ? true : false;
	result.middle = (f & MK_MBUTTON) ? true : false;
	result.right = (f & MK_RBUTTON) ? true : false;

	return result;
}

// メッセージ -----------------------------------------------------------------

inline void native_window_impl::set_message_handled(bool handled)
{
	m_msg_handled = handled;
}

inline bool native_window_impl::get_message_handled() const
{
	return m_msg_handled;
}

// 親ウィンドウからコールバックされる（プロシージャから直接のハンドラではない）
void native_window_impl::do_command(int id, UINT codeNotify)
{

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
	case WM_MOUSELEAVE: onMouseLeave(); break;

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
		HANDLE_MSG(hwnd, WM_CREATE, onCreate);
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
	set_message_handled(true);

	mouse m(x, y);
	m.state = make_mouse_state(keyFlags);
	m.button.left = true;

	get_public()->do_mouse_down_window(m);
}

void native_window_impl::onLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
	set_message_handled(true);

	mouse m(x, y);
	m.state = make_mouse_state(keyFlags);
	m.button.left = true;

	get_public()->do_mouse_up_window(m);
}

int native_window_impl::onMouseActivate(
	HWND hwnd, HWND hwndTopLevel, UINT codeHitTest, UINT msg)
{
	return MA_ACTIVATE;
}

// このメンバはメッセージ・ハンドラではない
// onMouseMove()から呼び出されます
void native_window_impl::onMouseEnter(HWND hwnd, int x, int y, UINT keyFlags)
{
	m_mouse_enter = true;

	mouse m(x, y);
	m.state = make_mouse_state(keyFlags);

	get_public()->do_mouse_enter_window(m);
}

void native_window_impl::onMouseLeave()
{
	m_mouse_enter = false;
	get_public()->do_mouse_leave_window();
}

void native_window_impl::onMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
	set_message_handled(true);

	mouse m(x, y);
	m.state = make_mouse_state(keyFlags);

	if (!m_mouse_enter)
	{
		onMouseEnter(hwnd, x, y, keyFlags);

		TRACKMOUSEEVENT ev;
		ev.cbSize = sizeof(ev);
		ev.dwFlags = TME_LEAVE;
		ev.hwndTrack = m_hwnd;

		BOOL result = ::TrackMouseEvent(&ev);
		assert(result != 0);
	}

	get_public()->do_mouse_move_window(m);
}

void native_window_impl::onMouseWheel(
	HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{
	set_message_handled(true);

	mouse m(xPos, yPos);
	m.state = make_mouse_state(fwKeys);

	get_public()->do_mouse_wheel_window(m);

}

void native_window_impl::onRButtonDblClk(
	HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
}

void native_window_impl::onRButtonDown(
	HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	set_message_handled(true);

	mouse m(x, y);
	m.state = make_mouse_state(keyFlags);
	m.button.right = true;

	get_public()->do_mouse_down_window(m);
}

void native_window_impl::onRButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
	set_message_handled(true);

	mouse m(x, y);
	m.state = make_mouse_state(keyFlags);
	m.button.right = true;

	get_public()->do_mouse_up_window(m);
}

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
	// nwはコントロール自身、thisはコントロールを載せているウィンドウ
	native_window_impl* nw = static_cast<native_window_impl*>(
		win32_message_service_impl::find(hwndCtl));
	assert(nw);
	// nwは自分自身の機能を知っているので適切に処理できる
	nw->do_command(id, codeNotify);

	set_message_handled(true);
}

void native_window_impl::onCompacting(HWND hwnd, UINT compactRatio){}

BOOL native_window_impl::onCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds)
{
	return 0;
}

BOOL native_window_impl::onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	get_public()->do_create_window();
	return 0; // 0を返すとウィンドウを生成する。生成しない場合、-1を返す。
}

void native_window_impl::onDestroy(HWND hwnd)
{
	get_public()->do_destroy_window();
}

BOOL native_window_impl::onEraseBkgnd(HWND hwnd, HDC hdc)
{
	set_message_handled(true);
	return TRUE;
}

void native_window_impl::onKillFocus(HWND hwnd, HWND hwndNewFocus){}

void native_window_impl::onMove(HWND hwnd, int x, int y){}

void native_window_impl::onPaint(HWND hwnd)
{
	PAINTSTRUCT ps;

	HDC hdc = ::BeginPaint(hwnd, &ps);
	assert(hdc);

	RECT rc0 = ps.rcPaint;
	rect_int rc1(
		rc0.left, rc0.top, rc0.right - rc0.left, rc0.bottom - rc0.top);
	std::unique_ptr<detail::native_canvas> ncv(new native_canvas_impl(hdc));
	canvas cv(std::move(ncv));

	cv->set_viewport(rc1);

	get_public()->do_paint_window(cv);

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

void native_window_impl::onSize(HWND hwnd, UINT state, int cx, int cy)
{
	get_public()->do_size_window(size_int(cx, cy));
}

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
	wcex.style = 0;
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





#endif // _WIN32
