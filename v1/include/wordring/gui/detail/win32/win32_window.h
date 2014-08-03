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

#include <wordring/debug.h>

#include <wordring/gui/detail/native_window.h>
#include <wordring/geometry/shape.h>
#include <wordring/gui/detail/win32/win32_window_class.h>

#include <Windows.h>

#include <utility>
#include <map>
#include <string>
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
	/// ウィンドウを作成します
	virtual void create_window(window* parent, rect_int rc);
	/// ウィンドウを最小化します
	virtual void close_window();
	/// ウィンドウを破棄します
	virtual void destroy_window();

	/// ウィンドウを表示します
	virtual void show_window();
	/// ウィンドウを非表示にします
	virtual void hide_window();

	virtual void set_native_parent_window(native_window* parent);
	virtual native_window* get_native_parent_window();

	/// ウィンドウ全体を再描画します
	void repaint_window();
	/// 指定の範囲を再描画します
	void repaint_window(rect_int rc);

	virtual void set_window_size(size_int size);
	virtual size_int get_window_size() const;

	virtual void set_window_position(point_int point);
	virtual point_int get_window_position() const;
	/// ウィンドウの位置と大きさを設定します
	void set_window_rect(rect_int rc);
	/// ウィンドウの位置と大きさを取得します
	rect_int get_window_rect() const;

	virtual void set_window_text(std::string text);
	virtual void set_window_text(std::wstring text);

protected:
	virtual void do_command_nw(int id, UINT codeNotify); // 親ウィンドウからのコールバック

	virtual void do_create_nw();
	virtual void do_destroy_nw();

	virtual void do_paint_nw(HDC hdc);

	virtual void do_size_nw(size_int size);







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
	 */
	LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/// native_window_impl用にカスタマイズされたwin32ウィンドウ・クラスです
	struct window_class
		: public win32_window_class<window_class, native_window_impl>
	{
		static WNDCLASSEX create();
	};
	static window_class g_window_class;
};


} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_H
