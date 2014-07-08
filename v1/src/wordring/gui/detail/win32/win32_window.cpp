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

std::map<HWND, native_window*> win32_message_map::g_map;

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
		hparent = dynamic_cast<win32_window_impl*>(parent->get_native())->m_hwnd;
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
	native_window* result = win32_message_map::find_window(hwnd);
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
	switch (uMsg)
	{
		//HANDLE_MSG(hwnd, WM_CREATE, on_create);
		//HANDLE_MSG(hwnd, WM_COMMAND, on_command);

		// マウス・メッセージ -------------------------------------------------

		HANDLE_MSG(hwnd, WM_DROPFILES, onDropFiles);
		HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, onLButtonDblClk);
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, onLButtonDown);
		HANDLE_MSG(hwnd, WM_LBUTTONUP, onLButtonUp);
		HANDLE_MSG(hwnd, WM_MBUTTONDBLCLK, onMButtonDblclk);
		HANDLE_MSG(hwnd, WM_MBUTTONDOWN, onMButtonDown);
		HANDLE_MSG(hwnd, WM_MBUTTONUP, onMButtonUp);
		HANDLE_MSG(hwnd, WM_MOUSEACTIVATE, onMouseActivate);
		HANDLE_MSG(hwnd, WM_MOUSEMOVE, onMouseMove);
		HANDLE_MSG(hwnd, WM_MOUSEWHEEL, onMouseWheel);
		HANDLE_MSG(hwnd, WM_PARENTNOTIFY, onParentNotify);
		HANDLE_MSG(hwnd, WM_RBUTTONDBLCLK, onRButtonDblClk);
		HANDLE_MSG(hwnd, WM_RBUTTONDOWN, onRButtonDown);
		HANDLE_MSG(hwnd, WM_RBUTTONUP, onRButtonUp);

		// キーボード ---------------------------------------------------------

		HANDLE_MSG(hwnd, WM_CHAR, onChar);
		HANDLE_MSG(hwnd, WM_CHARTOITEM, onCharToItem);
		HANDLE_MSG(hwnd, WM_DEADCHAR, onDeadChar);
		HANDLE_MSG(hwnd, WM_HOTKEY, onHotKey);
		HANDLE_MSG(hwnd, WM_KEYDOWN, onKeyDown);
		HANDLE_MSG(hwnd, WM_KEYUP, onKeyUp);
		HANDLE_MSG(hwnd, WM_SYSCHAR, onSysChar);
		HANDLE_MSG(hwnd, WM_SYSDEADCHAR, onSysDeadChar);
		HANDLE_MSG(hwnd, WM_SYSKEYDOWN, onSysKeyDown);
		HANDLE_MSG(hwnd, WM_SYSKEYUP, onSysKeyUp);
		HANDLE_MSG(hwnd, WM_VKEYTOITEM, onVkeyToItem);

		// 一般メッセージ ---
		HANDLE_MSG(hwnd, WM_ACTIVATE, onActivate);
		HANDLE_MSG(hwnd, WM_ACTIVATEAPP, onActivateApp);
		HANDLE_MSG(hwnd, WM_CANCELMODE, onCancelMode);
		HANDLE_MSG(hwnd, WM_CHILDACTIVATE, onChildActivate);
		HANDLE_MSG(hwnd, WM_CLOSE, onClose);
		HANDLE_MSG(hwnd, WM_COMMAND, onCommand);
		HANDLE_MSG(hwnd, WM_COMMNOTIFY, onCommNotify);
		HANDLE_MSG(hwnd, WM_COMPACTING, onCompacting);
		HANDLE_MSG(hwnd, WM_COMPAREITEM, onCompareItem);
		HANDLE_MSG(hwnd, WM_CONTEXTMENU, onContextMenu);
		HANDLE_MSG(hwnd, WM_COPYDATA, onCopyData);
		HANDLE_MSG(hwnd, WM_CREATE, onCreate);
		HANDLE_MSG(hwnd, WM_CTLCOLORBTN, onCtlColorBtn);
		HANDLE_MSG(hwnd, WM_CTLCOLORDLG, onCtlColorDlg);
		HANDLE_MSG(hwnd, WM_CTLCOLOREDIT, onCtlColorEdit);
		HANDLE_MSG(hwnd, WM_CTLCOLORLISTBOX, onCtlColorListbox);
		HANDLE_MSG(hwnd, WM_CTLCOLORMSGBOX, onCtlColorMsgbox);
		HANDLE_MSG(hwnd, WM_CTLCOLORSCROLLBAR, onCtlColorScrollbar);
		HANDLE_MSG(hwnd, WM_CTLCOLORSTATIC, onCtlColorStatic);
		HANDLE_MSG(hwnd, WM_DELETEITEM, onDeleteItem);
		HANDLE_MSG(hwnd, WM_DESTROY, onDestroy);
		HANDLE_MSG(hwnd, WM_DEVICECHANGE, onDeviceChange);
		HANDLE_MSG(hwnd, WM_DEVMODECHANGE, onDevModeChange);
		HANDLE_MSG(hwnd, WM_DISPLAYCHANGE, onDisplayChange);
		HANDLE_MSG(hwnd, WM_DRAWITEM, onDrawItem);
		HANDLE_MSG(hwnd, WM_ENABLE, onEnable);
		HANDLE_MSG(hwnd, WM_ENDSESSION, onEndSession);
		HANDLE_MSG(hwnd, WM_ENTERIDLE, onEnterIdle);
		HANDLE_MSG(hwnd, WM_ERASEBKGND, onEraseBkgnd);
		HANDLE_MSG(hwnd, WM_FONTCHANGE, onFontChange);
		HANDLE_MSG(hwnd, WM_GETDLGCODE, onGetDlgCode);
		HANDLE_MSG(hwnd, WM_GETFONT, onGetFont);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, onGetMinMaxInfo);
		HANDLE_MSG(hwnd, WM_GETTEXT, onGetText);
		HANDLE_MSG(hwnd, WM_GETTEXTLENGTH, onGetTextLength);
		HANDLE_MSG(hwnd, WM_HSCROLL, onHScroll);
		HANDLE_MSG(hwnd, WM_ICONERASEBKGND, onIconEraseBkgnd);
		HANDLE_MSG(hwnd, WM_INITDIALOG, onInitDialog);
		HANDLE_MSG(hwnd, WM_INITMENU, onInitMenu);
		HANDLE_MSG(hwnd, WM_INITMENUPOPUP, onInitMenuPopup);
		HANDLE_MSG(hwnd, WM_KILLFOCUS, onKillFocus);
		HANDLE_MSG(hwnd, WM_MEASUREITEM, onMeasureItem);
		HANDLE_MSG(hwnd, WM_MENUCHAR, onMenuChar);
		HANDLE_MSG(hwnd, WM_MENUSELECT, onMenuSelect);
		HANDLE_MSG(hwnd, WM_MOVE, onMove);
		HANDLE_MSG(hwnd, WM_NEXTDLGCTL, onNextDlgCtl);
		HANDLE_MSG(hwnd, WM_PAINT, onPaint);
		HANDLE_MSG(hwnd, WM_PALETTECHANGED, onPaletteChanged);
		HANDLE_MSG(hwnd, WM_PALETTEISCHANGING, onPaletteIsChanging);
		HANDLE_MSG(hwnd, WM_POWER, onPower);
		HANDLE_MSG(hwnd, WM_QUERYDRAGICON, onQueryDragIcon);
		HANDLE_MSG(hwnd, WM_QUERYENDSESSION, onQueryEndSession);
		HANDLE_MSG(hwnd, WM_QUERYNEWPALETTE, onQueryNewPalette);
		HANDLE_MSG(hwnd, WM_QUERYOPEN, onQueryOpen);
		HANDLE_MSG(hwnd, WM_QUEUESYNC, onQueueSync);
		HANDLE_MSG(hwnd, WM_QUIT, onQuit);
		HANDLE_MSG(hwnd, WM_SETFOCUS, onSetFocus);
		HANDLE_MSG(hwnd, WM_SETFONT, onSetFont);
		HANDLE_MSG(hwnd, WM_SETREDRAW, onSetRedraw);
		HANDLE_MSG(hwnd, WM_SETTEXT, onSetText);
		HANDLE_MSG(hwnd, WM_SHOWWINDOW, onShowWindow);
		HANDLE_MSG(hwnd, WM_SIZE, onSize);
		HANDLE_MSG(hwnd, WM_SPOOLERSTATUS, onSpoolerStatus);
		HANDLE_MSG(hwnd, WM_SYSCOLORCHANGE, onSysColorChange);
		HANDLE_MSG(hwnd, WM_SYSCOMMAND, onSysCommand);
		//HANDLE_MSG(hwnd, WM_SYSTEMERROR, onSystemError);
		HANDLE_MSG(hwnd, WM_TIMECHANGE, onTimeChange);
		HANDLE_MSG(hwnd, WM_TIMER, onTimer);
		HANDLE_MSG(hwnd, WM_VSCROLL, onVScroll);
		HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGED, onWindowPosChanged);
		HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGING, onWindowPosChanging);
		HANDLE_MSG(hwnd, WM_WININICHANGE, onWinIniChange);


		/*
	case WM_MOUSEMOVE:

	case WM_COMMAND:
		return on_command(hWnd, uMsg, wParam, lParam);

	case WM_CREATE:
		return on_create();

	case WM_DESTROY:
		win32_message_map::remove_window(m_hwnd);
		::PostQuitMessage(0);
		return 0;
		*/
	}

	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);

}

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

void win32_window_impl::onMButtonDblclk(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}

void win32_window_impl::onMButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}

void win32_window_impl::onMButtonUp(HWND hwnd, int x, int y, UINT flags)
{

}

int win32_window_impl::onMouseActivate(HWND hwnd, HWND hwndTopLevel, UINT codeHitTest, UINT msg)
{

}

void win32_window_impl::onMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{

}

void win32_window_impl::onMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{

}

void win32_window_impl::onParentNotify(HWND hwnd, UINT msg, HWND hwndChild, int idChild)
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

int win32_window_impl::onCharToItem(HWND hwnd, UINT ch, HWND hwndListbox, int iCaret)
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

int win32_window_impl::onVkeyToItem(HWND hwnd, UINT vk, HWND hwndListbox, int iCaret)
{

}

// 一般メッセージ -------------------------------------------------------------


void win32_window_impl::onActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{

}

void win32_window_impl::onActivateApp(HWND hwnd, BOOL fActivate, DWORD dwThreadId)
{

}

void win32_window_impl::onCancelMode(HWND hwnd)
{

}

void win32_window_impl::onChildActivate(HWND hwnd)
{

}

void win32_window_impl::onClose(HWND hwnd)
{

}

void win32_window_impl::onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}

void win32_window_impl::onCommNotify(HWND hwnd, int cid, UINT flags)
{

}

void win32_window_impl::onCompacting(HWND hwnd, UINT compactRatio)
{

}

int win32_window_impl::onCompareItem(HWND hwnd, const COMPAREITEMSTRUCT * lpCompareItem)
{

}

void win32_window_impl::onContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos)
{

}

BOOL win32_window_impl::onCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds)
{

}

BOOL win32_window_impl::onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{

}

HBRUSH win32_window_impl::onCtlColorBtn(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{

}

HBRUSH win32_window_impl::onCtlColorDlg(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{

}

HBRUSH win32_window_impl::onCtlColorEdit(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{

}

HBRUSH win32_window_impl::onCtlColorListbox(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{

}

HBRUSH win32_window_impl::onCtlColorMsgbox(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{

}

HBRUSH win32_window_impl::onCtlColorScrollbar(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{

}

HBRUSH win32_window_impl::onCtlColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{

}

void win32_window_impl::onDeleteItem(HWND hwnd, const DELETEITEMSTRUCT * lpDeleteItem)
{

}

void win32_window_impl::onDestroy(HWND hwnd)
{

}

BOOL win32_window_impl::onDeviceChange(HWND hwnd, UINT uEvent, DWORD dwEventData)
{

}

void win32_window_impl::onDevModeChange(HWND hwnd, LPCTSTR lpszDeviceName)
{

}

void win32_window_impl::onDisplayChange(HWND hwnd, UINT bitsPerPixel, UINT cxScreen, UINT cyScreen)
{

}

void win32_window_impl::onDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem)
{

}

void win32_window_impl::onEnable(HWND hwnd, BOOL fEnable)
{

}

void win32_window_impl::onEndSession(HWND hwnd, BOOL fEnding)
{

}

void win32_window_impl::onEnterIdle(HWND hwnd, UINT source, HWND hwndSource)
{

}

BOOL win32_window_impl::onEraseBkgnd(HWND hwnd, HDC hdc)
{

}

void win32_window_impl::onFontChange(HWND hwnd)
{

}

UINT win32_window_impl::onGetDlgCode(HWND hwnd, LPMSG lpmsg)
{

}

HFONT win32_window_impl::onGetFont(HWND hwnd)
{

}

void win32_window_impl::onGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{

}

INT win32_window_impl::onGetText(HWND hwnd, int cchTextMax, LPTSTR lpszText)
{

}

INT win32_window_impl::onGetTextLength(HWND hwnd)
{

}

void win32_window_impl::onHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{

}

BOOL win32_window_impl::onIconEraseBkgnd(HWND hwnd, HDC hdc)
{

}

BOOL win32_window_impl::onInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{

}

void win32_window_impl::onInitMenu(HWND hwnd, HMENU hMenu)
{

}

void win32_window_impl::onInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu)
{

}

void win32_window_impl::onKillFocus(HWND hwnd, HWND hwndNewFocus)
{

}

void win32_window_impl::onMeasureItem(HWND hwnd, MEASUREITEMSTRUCT * lpMeasureItem)
{

}

DWORD win32_window_impl::onMenuChar(HWND hwnd, UINT ch, UINT flags, HMENU hmenu)
{

}

void win32_window_impl::onMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{

}

void win32_window_impl::onMove(HWND hwnd, int x, int y)
{

}

HWND win32_window_impl::onNextDlgCtl(HWND hwnd, HWND hwndSetFocus, BOOL fNext)
{

}

void win32_window_impl::onPaint(HWND hwnd)
{

}

void win32_window_impl::onPaletteChanged(HWND hwnd, HWND hwndPaletteChange)
{

}

void win32_window_impl::onPaletteIsChanging(HWND hwnd, HWND hwndPaletteChange)
{

}

void win32_window_impl::onPower(HWND hwnd, int code)
{

}

HICON win32_window_impl::onQueryDragIcon(HWND hwnd)
{

}

BOOL win32_window_impl::onQueryEndSession(HWND hwnd)
{

}

BOOL win32_window_impl::onQueryNewPalette(HWND hwnd)
{

}

BOOL win32_window_impl::onQueryOpen(HWND hwnd)
{

}

void win32_window_impl::onQueueSync(HWND hwnd)
{

}

void win32_window_impl::onQuit(HWND hwnd, int exitCode)
{

}

void win32_window_impl::onSetFocus(HWND hwnd, HWND hwndOldFocus)
{

}

void win32_window_impl::onSetFont(HWND hwndCtl, HFONT hfont, BOOL fRedraw)
{

}

void win32_window_impl::onSetRedraw(HWND hwnd, BOOL fRedraw)
{

}

void win32_window_impl::onSetText(HWND hwnd, LPCTSTR lpszText)
{

}

void win32_window_impl::onShowWindow(HWND hwnd, BOOL fShow, UINT status)
{

}

void win32_window_impl::onSize(HWND hwnd, UINT state, int cx, int cy)
{

}

void win32_window_impl::onSpoolerStatus(HWND hwnd, UINT status, int cJobInQueue)
{

}

void win32_window_impl::onSysColorChange(HWND hwnd)
{

}

void win32_window_impl::onSysCommand(HWND hwnd, UINT cmd, int x, int y)
{

}

void win32_window_impl::onSystemError(HWND hwnd, int errCode)
{

}

void win32_window_impl::onTimeChange(HWND hwnd)
{

}

void win32_window_impl::onTimer(HWND hwnd, UINT id)
{

}

void win32_window_impl::onVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{

}

void win32_window_impl::onWindowPosChanged(HWND hwnd, const LPWINDOWPOS lpwpos)
{

}

BOOL win32_window_impl::onWindowPosChanging(HWND hwnd, LPWINDOWPOS lpwpos)
{

}

void win32_window_impl::onWinIniChange(HWND hwnd, LPCTSTR lpszSectionName)
{

}






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

WNDCLASSEX win32_window_impl::window_class::create() const
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = win32_window_class<win32_window_impl>::WindowProc;
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
		hparent = dynamic_cast<win32_control_window_impl*>(parent->get_native())->m_hwnd;
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

WNDCLASSEX win32_control_window_impl::window_class::create() const
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = win32_window_class<win32_control_window_impl>::WindowProc;
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
		hparent = dynamic_cast<win32_window_impl*>(parent->get_native())->m_hwnd;
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

WNDCLASSEX win32_container_window_impl::window_class::create() const
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = win32_window_class<win32_container_window_impl>::WindowProc;
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
		hparent = dynamic_cast<win32_window_impl*>(parent->get_native())->m_hwnd;
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

	win32_message_map::add_window(m_hwnd, this);
	//::ShowWindow(m_hwnd, SW_SHOW);
	//::UpdateWindow(m_hwnd);

}





#endif // _WIN32
