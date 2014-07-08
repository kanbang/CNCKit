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

	/// WM_COMMANDを処理します
	//LRESULT on_command(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//LRESULT on_create(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
	//LRESULT on_click();

	/// オブジェクト用にカスタマイズされたウィンドウ・プロシージャです
	LRESULT window_proc(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/// win32_window_impl用にカスタマイズされたwin32ウィンドウ・クラスです
	struct window_class
		: public win32_window_class<window_class, window, win32_window_impl>
	{
		window_class() { }
		virtual ~window_class() { }
		static WNDCLASSEX create();
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
