/**
 * @file    wordring/gui/detail/win32/win32_child_window.h
 *
 * @brief   コントロール用のウィンドウ
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

#ifndef WORDRING_WIN32_CONTROL_WINDOW_H
#define WORDRING_WIN32_CONTROL_WINDOW_H

#include <wordring/debug.h>

#include <wordring/geometry/shape.h>

#include <wordring/gui/detail/win32/win32_window.h>
#include <wordring/gui/detail/win32/win32_window_class.h>

#include <Windows.h>

namespace wordring
{
namespace gui
{

class window;

namespace detail
{

class dummy;

/// コントロール用のウィンドウ
class native_control_window_impl : public native_window_impl
{
	// 構築・破棄 -------------------------------------------------------------
protected:
	native_control_window_impl();

public:
	virtual ~native_control_window_impl();

	/**
	 * @brief   ウィンドウを作成します
	 *
	 * @details
	 *          メモリー管理を行うため、オブジェクトの生成は必ずこの関数を使い
	 *          ます。
	 */
	static native_window::store create();

	/// ウィンドウを作成します
	virtual void create_window(window* parent, rect_int rc);

public:
	/// ウィンドウ・プロシージャの雛型です
	LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/// native_control_window_impl用にカスタマイズされたwin32ウィンドウ・クラスです
	struct window_class
		: public win32_window_class<window_class, native_control_window_impl>
	{
		static WNDCLASSEX create();
	};
	static window_class g_window_class;
};


} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_CONTROL_WINDOW_H
