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
#include <wordring/gui/detail/win32/win32_window_class.h>

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

// 基本ウィンドウ -------------------------------------------------------------

/**
 * @brief 基本ウィンドウ
 * 
 * 
 */
class native_window_impl : public native_window
{
public:
	HWND m_hwnd;
	bool m_msg_handled; /// true: メッセージの処理を完了した

public:
	native_window_impl();
	virtual ~native_window_impl();

protected:
	void set_message_handled(bool handled);
public:
	bool get_message_handled() const;

public:
	virtual void create(window* parent);
	/// ウィンドウを最小化します
	virtual void close();
	virtual void destroy();

	virtual void set_parent(native_window* parent);
	virtual native_window* get_parent();

	virtual void set_size(size_int size);
	virtual size_int get_size() const;

	virtual void set_position(point_int point);
	virtual point_int get_position() const;

protected:
	virtual void on_command(int id, UINT codeNotify); // 親ウィンドウからのコールバック

	virtual bool on_create();

	virtual void on_paint(HDC hdc);







protected:
	// マウス・メッセージ -----------------------------------------------------

	/// ファイルがドラッグ・アンド・ドロップされた
	void onDropFiles(HWND hwnd, HDROP hdrop);

	void onLButtonDblClk(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void onLButtonDown(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);

	void onLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);

	int onMouseActivate(
		HWND hwnd, HWND hwndTopLevel, UINT codeHitTest, UINT msg);

	void onMouseMove(HWND hwnd, int x, int y, UINT keyFlags);

	void onMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys);

	void onRButtonDblClk(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);

	void onRButtonDown(
		HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);

	void onRButtonUp(HWND hwnd, int x, int y, UINT flags);

	// キーボード・メッセージ -------------------------------------------------

	void onChar(HWND hwnd, TCHAR ch, int cRepeat);

	void onDeadChar(HWND hwnd, TCHAR ch, int cRepeat);

	void onHotKey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk);

	void onKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

	void onKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

	void onSysChar(HWND hwnd, TCHAR ch, int cRepeat);

	void onSysDeadChar(HWND hwnd, TCHAR ch, int cRepeat);

	void onSysKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

	void onSysKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

	// 一般メッセージ ---------------------------------------------------------

	void onActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized);

	void onActivateApp(HWND hwnd, BOOL fActivate, DWORD dwThreadId);

	void onClose(HWND hwnd);

	/// 子ウィンドウでコマンドが発生した
	void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

	void onCompacting(HWND hwnd, UINT compactRatio);

	BOOL onCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds);

	BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);

	void onDestroy(HWND hwnd);

	BOOL onEraseBkgnd(HWND hwnd, HDC hdc);

	void onKillFocus(HWND hwnd, HWND hwndNewFocus);

	void onMove(HWND hwnd, int x, int y);

	void onPaint(HWND hwnd);

	void onPower(HWND hwnd, int code);

	BOOL onQueryEndSession(HWND hwnd);

	void onQuit(HWND hwnd, int exitCode);

	void onSetFocus(HWND hwnd, HWND hwndOldFocus);

	void onShowWindow(HWND hwnd, BOOL fShow, UINT status);

	void onSize(HWND hwnd, UINT state, int cx, int cy);

	void onSysCommand(HWND hwnd, UINT cmd, int x, int y);

	void onTimer(HWND hwnd, UINT id);

	void onWindowPosChanged(HWND hwnd, const LPWINDOWPOS lpwpos);

	BOOL onWindowPosChanging(HWND hwnd, LPWINDOWPOS lpwpos);

public:
	/** ウィンドウ・プロシージャの雛型です
	 * @param handled true: それ以上処理を行わない。false: デフォルトの処理を行う。 
	 *    
	 */
	LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/// win32_window_impl用にカスタマイズされたwin32ウィンドウ・クラスです
	struct window_class
		: public win32_window_class<window_class, native_window_impl>
	{
		static WNDCLASSEX create();
	};
	static window_class g_window_class;
};
/*
template <typename T>
class native_container_window_impl : public native_window_impl
{
public:

	LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

	}

	struct window_class
		: public win32_window_class<
			window_class, native_container_window_impl>
	{
		static WNDCLASSEX create()
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
	};

	/// win32_window_impl用のwindow_class
	static window_class g_window_class;
};
*/
/*
template <typename T>
class native_window_impl_impl : public win32_window_impl
{

};
*/
/**
 * @brief コントロール・ウィンドウ
 * @details
 *    - native_control_window_implにtypedefされます。
 */
/*
class win32_control_window_impl : public win32_window_impl
{
public:
	win32_control_window_impl();

	void create(window* parent);

	/// win32_control_window_impl用にカスタマイズされたwin32ウィンドウ・クラスです
	struct window_class
		: public win32_window_class<window_class, window, win32_control_window_impl>
	{
		window_class() { }
		virtual ~window_class() { }
		static WNDCLASSEX create();
	};

	/// win32_control_window_impl用のwindow_class
	static window_class g_window_class;

public:
	typedef window_class class_type;
	typedef window window_type;
};
*/
/**
 * @brief コンテナ・ウィンドウ
 * @details
 *    - win32_container_window_implにtypedefされます。
 */
/*
class win32_container_window_impl : public win32_control_window_impl
{
public:
	void create(window* parent);

	/// win32_container_window_impl用にカスタマイズされたwin32ウィンドウ・クラスです
	struct window_class
		: public win32_window_class<window_class, window, win32_container_window_impl>
	{
		window_class() { }
		virtual ~window_class() { }
		static WNDCLASSEX create();
	};

	/// win32_container_window_impl用のwindow_class
	static window_class g_window_class;

public:
	typedef window_class class_type;
	typedef window window_type;
};

class win32_button_window_impl : public win32_window_impl
{
public:
	virtual void create(window* parent);
};
*/
/** window.cppで使用 */
//typedef win32_window_impl native_window_impl;

//typedef win32_window_impl_impl native_window_impl_impl;
/** window.cppで使用 */
//typedef win32_control_window_impl native_control_window_impl;
/** window.cppで使用 */
//typedef win32_container_window_impl native_container_window_impl;
/** window.cppで使用 */
//typedef win32_button_window_impl native_button_window_impl;




} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_H
