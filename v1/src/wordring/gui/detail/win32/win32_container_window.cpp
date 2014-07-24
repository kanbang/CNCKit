/**
 * @file    wordring/gui/detail/win32/win32_container_window.cpp
 *
 * @brief   
 *
 * @details
 *          
 *          
 *          
 *          
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

#include <wordring/gui/window.h>

#include <wordring/gui/detail/win32/win32_container_window.h>
#include <wordring/gui/detail/win32/win32_window_class.h>

#include <Windows.h>
#include <windowsx.h>

using namespace wordring::gui::detail;

native_container_window_impl::native_container_window_impl()
{

}

native_container_window_impl::~native_container_window_impl()
{

}

void native_container_window_impl::create(wordring::gui::window* parent)
{
	HWND hparent = nullptr;
	if (parent)
	{
		hparent = static_cast<native_window_impl*>(
			parent->get_native_window())->m_hwnd;
	}

	m_hwnd = ::CreateWindow(
		(LPCTSTR)(DWORD)native_window_impl::g_window_class.m_atom,
		L"native_container_window_impl", WS_OVERLAPPEDWINDOW,
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

/// ウィンドウ・プロシージャの雛型です
LRESULT native_container_window_impl::WindowProc(
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

WNDCLASSEX native_container_window_impl::window_class::create()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = window_class::WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(native_container_window_impl*);
	wcex.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("native_container_window_impl");
	wcex.hIconSm = NULL;

	return wcex;
}
/// native_container_window_impl用のwindow_class
native_container_window_impl::window_class g_window_class;

#endif // _WIN32
