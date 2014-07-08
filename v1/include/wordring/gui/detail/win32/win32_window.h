/**
 * @file    wordring/gui/detail/win32/win32_window.h
 *
 * @brief   ウィンドウの環境固有ヘッダー・ファイル
 *
 * @details
 *        - このファイルは、win32専用です。
 *          ライブラリ利用者は、このヘッダー・ファイルを使用しません。\n
 *          pimplによって実装を隠ぺいします。
 *        - このファイルはwindow.cpp内に読み込まれます。 
 *        - メッセージ・マップはスレッドに対応していません。
 *          VC++がthread_localに対応した時点で対応を考えます。
 *          gcc4.8以降はthread_localに対応しているそうです。
 *          
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#ifndef WORDRING_WIN32_WINDOW_H
#define WORDRING_WIN32_WINDOW_H

#include <wordring/gui/detail/native_window.h>
#include <wordring/geometry/shape.h>

#include <Windows.h>

#include <utility>
#include <map>
#include <cassert>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

namespace wordring
{
namespace gui
{
namespace detail
{

// メッセージ・マップ ---------------------------------------------------------

/**
 * @brief メッセージ・マップ
 * @details
 *    ウィンドウズのウィンドウ・ハンドルとC++のウィンドウ・オブジェクトを
 *    結びつけるマップです。
 *    現在はスレッドに対応していないため、プロセス内に一つしか有りません。
 */
class win32_message_map
{
	/// プログラム内唯一のマップ
	static std::map<HWND, native_window*> g_map;

public:
	/// ハンドルとウィンドウ・オブジェクトのセットをマップに追加します
	static void add_window(HWND hwnd, native_window* pwin)
	{
		g_map[hwnd] = pwin;
	}

	/// ハンドルからウィンドウ・オブジェクトを検索します
	static native_window* find_window(HWND hwnd)
	{
		std::map<HWND, native_window*>::iterator it = g_map.find(hwnd);
		return (it == g_map.end()) ? nullptr : it->second;
	}

	/// ハンドルを指定してウィンドウ・オブジェクトとのセットを削除します
	static void remove_window(HWND hwnd)
	{
		size_t n = g_map.erase(hwnd);
		assert(n == 1);
	}
};

// ウィンドウ・クラス ---------------------------------------------------------

/**
 * @brief 基底ウィンドウ・クラス
 * @details
 *    win32のウィンドウ・クラスを作る目的の基底クラスです。
 *    C++のクラスとwin32のウィンドウ・クラスを結びつけます。
 *    仮想関数を使わずにメッセージを配送する能力があります。
 */
template <typename T>
struct win32_window_class
{
	HINSTANCE m_hinstance;
	ATOM m_atom;

	/// win32のウィンドウ・クラスを登録します
	win32_window_class() : m_hinstance((HINSTANCE)NULL), m_atom((ATOM)NULL)
	{
		T::window_class* self = static_cast<T::window_class*>(this);
		WNDCLASSEX wcex = self->T::window_class::create();
		m_atom = ::RegisterClassEx(&wcex);
		m_hinstance = wcex.hInstance;
	}

	/// win32のウィンドウ・オブジェクトを解除します
	virtual ~win32_window_class()
	{
		assert(m_atom != (ATOM)NULL);
		::UnregisterClass((LPCTSTR)(DWORD)m_atom, m_hinstance);
	}

	/// ウィンドウ・プロシージャの雛型です
	static LRESULT CALLBACK WindowProc(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		T* self = static_cast<T*>(win32_message_map::find_window(hWnd));

		if (uMsg == WM_CREATE || uMsg == WM_NCCREATE)
		{
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			self = (T*)pcs->lpCreateParams;

			win32_message_map::add_window(
				hWnd, static_cast<native_window*>(self));

			self->m_hwnd = hWnd;
		}

		if (self)
		{
			return self->window_proc(hWnd, uMsg, wParam, lParam);
		}
		else
		{
			switch (uMsg)
			{
			case WM_GETMINMAXINFO:
				break;
			case WM_NCDESTROY:
				break;
			default:
#ifdef _DEBUG
				std::cout <<
					"ハンドルされないメッセージ: 0x";
				std::cout << std::hex << uMsg << std::endl;
#endif // _DEBUG
				break;
			}

		}

		return  ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
};

// 基本ウィンドウ -------------------------------------------------------------

/**
 * @brief 基本ウィンドウ
 * 
 * - native_window_implにtypedefされます。
 */
class win32_window_impl : public native_window
{
public:
	HWND m_hwnd;

public:
	win32_window_impl();

	virtual void create(window* parent);
	/// ウィンドウを最小化します
	virtual void close();

	virtual void destroy();

	virtual native_window* get_parent();
	virtual void set_parent(native_window* parent);

	virtual void set_size(size_int size);
	virtual size_int get_size() const;
	virtual void set_position(point_int point);
	virtual point_int get_position() const;

	// マウス・メッセージ -----------------------------------------------------

	void onDropFiles(HWND hwnd, HDROP hdrop);
	
	void onLButtonDblClk(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	
	void onLButtonDown(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	
	void onLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
	
	void onMButtonDblclk(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	
	void onMButtonDown(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	
	void onMButtonUp(HWND hwnd, int x, int y, UINT flags);
	
	int onMouseActivate(
		HWND hwnd, HWND hwndTopLevel, UINT codeHitTest, UINT msg);
	
	void onMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
	
	void onMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys);
	
	void onParentNotify(HWND hwnd, UINT msg, HWND hwndChild, int idChild);
	
	void onRButtonDblClk(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	
	void onRButtonDown(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	
	void onRButtonUp(HWND hwnd, int x, int y, UINT flags);
	
	// キーボード・メッセージ -------------------------------------------------

	void onChar(HWND hwnd, TCHAR ch, int cRepeat);

	int onCharToItem(HWND hwnd, UINT ch, HWND hwndListbox, int iCaret);

	void onDeadChar(HWND hwnd, TCHAR ch, int cRepeat);

	void onHotKey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk);

	void onKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

	void onKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

	void onSysChar(HWND hwnd, TCHAR ch, int cRepeat);

	void onSysDeadChar(HWND hwnd, TCHAR ch, int cRepeat);

	void onSysKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

	void onSysKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

	int onVkeyToItem(HWND hwnd, UINT vk, HWND hwndListbox, int iCaret);

	// 一般メッセージ ---------------------------------------------------------

	void onActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized);

	void onActivateApp(HWND hwnd, BOOL fActivate, DWORD dwThreadId);

	void onCancelMode(HWND hwnd);

	void onChildActivate(HWND hwnd);

	void onClose(HWND hwnd);

	void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

	void onCommNotify(HWND hwnd, int cid, UINT flags);

	void onCompacting(HWND hwnd, UINT compactRatio);

	int onCompareItem(HWND hwnd, const COMPAREITEMSTRUCT * lpCompareItem);

	void onContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos);

	BOOL onCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds);

	BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);

	HBRUSH onCtlColorBtn(HWND hwnd, HDC hdc, HWND hwndChild, int type);

	HBRUSH onCtlColorDlg(HWND hwnd, HDC hdc, HWND hwndChild, int type);

	HBRUSH onCtlColorEdit(HWND hwnd, HDC hdc, HWND hwndChild, int type);

	HBRUSH onCtlColorListbox(HWND hwnd, HDC hdc, HWND hwndChild, int type);

	HBRUSH onCtlColorMsgbox(HWND hwnd, HDC hdc, HWND hwndChild, int type);

	HBRUSH onCtlColorScrollbar(HWND hwnd, HDC hdc, HWND hwndChild, int type);

	HBRUSH onCtlColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, int type);

	void onDeleteItem(HWND hwnd, const DELETEITEMSTRUCT * lpDeleteItem);

	void onDestroy(HWND hwnd);

	BOOL onDeviceChange(HWND hwnd, UINT uEvent, DWORD dwEventData);

	void onDevModeChange(HWND hwnd, LPCTSTR lpszDeviceName);

	void onDisplayChange(HWND hwnd, UINT bitsPerPixel, UINT cxScreen, UINT cyScreen);

	void onDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem);

	void onEnable(HWND hwnd, BOOL fEnable);

	void onEndSession(HWND hwnd, BOOL fEnding);

	void onEnterIdle(HWND hwnd, UINT source, HWND hwndSource);

	BOOL onEraseBkgnd(HWND hwnd, HDC hdc);

	void onFontChange(HWND hwnd);

	UINT onGetDlgCode(HWND hwnd, LPMSG lpmsg);

	HFONT onGetFont(HWND hwnd);

	void onGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);

	INT onGetText(HWND hwnd, int cchTextMax, LPTSTR lpszText);

	INT onGetTextLength(HWND hwnd);

	void onHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);

	BOOL onIconEraseBkgnd(HWND hwnd, HDC hdc);

	BOOL onInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);

	void onInitMenu(HWND hwnd, HMENU hMenu);

	void onInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu);

	void onKillFocus(HWND hwnd, HWND hwndNewFocus);

	void onMeasureItem(HWND hwnd, MEASUREITEMSTRUCT * lpMeasureItem);

	DWORD onMenuChar(HWND hwnd, UINT ch, UINT flags, HMENU hmenu);

	void onMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags);

	void onMove(HWND hwnd, int x, int y);

	HWND onNextDlgCtl(HWND hwnd, HWND hwndSetFocus, BOOL fNext);

	void onPaint(HWND hwnd);

	void onPaletteChanged(HWND hwnd, HWND hwndPaletteChange);

	void onPaletteIsChanging(HWND hwnd, HWND hwndPaletteChange);

	void onPower(HWND hwnd, int code);

	HICON onQueryDragIcon(HWND hwnd);

	BOOL onQueryEndSession(HWND hwnd);

	BOOL onQueryNewPalette(HWND hwnd);

	BOOL onQueryOpen(HWND hwnd);

	void onQueueSync(HWND hwnd);

	void onQuit(HWND hwnd, int exitCode);

	void onSetFocus(HWND hwnd, HWND hwndOldFocus);

	void onSetFont(HWND hwndCtl, HFONT hfont, BOOL fRedraw);

	void onSetRedraw(HWND hwnd, BOOL fRedraw);

	void onSetText(HWND hwnd, LPCTSTR lpszText);

	void onShowWindow(HWND hwnd, BOOL fShow, UINT status);

	void onSize(HWND hwnd, UINT state, int cx, int cy);

	void onSpoolerStatus(HWND hwnd, UINT status, int cJobInQueue);

	void onSysColorChange(HWND hwnd);

	void onSysCommand(HWND hwnd, UINT cmd, int x, int y);

	void onSystemError(HWND hwnd, int errCode);

	void onTimeChange(HWND hwnd);

	void onTimer(HWND hwnd, UINT id);

	void onVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);

	void onWindowPosChanged(HWND hwnd, const LPWINDOWPOS lpwpos);

	BOOL onWindowPosChanging(HWND hwnd, LPWINDOWPOS lpwpos);

	void onWinIniChange(HWND hwnd, LPCTSTR lpszSectionName);





	/// WM_COMMANDを処理します
	LRESULT on_command(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT on_create(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
	LRESULT on_click();

	/// オブジェクト用にカスタマイズされたウィンドウ・プロシージャです
	LRESULT window_proc(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/// win32_window_impl用にカスタマイズされたwin32ウィンドウ・クラスです
	struct window_class : public win32_window_class<win32_window_impl>
	{
		window_class() { }
		virtual ~window_class() { }
		WNDCLASSEX create() const;
	};

	/// win32_window_impl用のwindow_class
	static window_class g_window_class;
};

/**
 * @brief コントロール・ウィンドウ
 * @details
 *    - native_control_window_implにtypedefされます。
 */
class win32_control_window_impl : public win32_window_impl
{
public:
	win32_control_window_impl();

	void create(window* parent);

	/// win32_control_window_impl用にカスタマイズされたwin32ウィンドウ・クラスです
	struct window_class : public win32_window_class<win32_control_window_impl>
	{
		window_class() { }
		virtual ~window_class() { }
		WNDCLASSEX create() const;
	};

	/// win32_control_window_impl用のwindow_class
	static window_class g_window_class;
};

/**
 * @brief コンテナ・ウィンドウ
 * @details
 *    - win32_container_window_implにtypedefされます。
 */
class win32_container_window_impl : public win32_control_window_impl
{
public:
	void create(window* parent);

	/// win32_container_window_impl用にカスタマイズされたwin32ウィンドウ・クラスです
	struct window_class : public win32_window_class<win32_container_window_impl>
	{
		window_class() { }
		virtual ~window_class() { }
		virtual WNDCLASSEX create() const;
	};

	/// win32_container_window_impl用のwindow_class
	static window_class g_window_class;
};

class win32_button_window_impl : public win32_window_impl
{
public:
	virtual void create(window* parent);
};

/** window.cppで使用 */
typedef win32_window_impl native_window_impl;
/** window.cppで使用 */
typedef win32_control_window_impl native_control_window_impl;
/** window.cppで使用 */
typedef win32_container_window_impl native_container_window_impl;
/** window.cppで使用 */
typedef win32_button_window_impl native_button_window_impl;




} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_H
