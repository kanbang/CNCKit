/**
* @file    wordring/gui/detail/win32/win32_container_window.h
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

#ifndef WORDRING_WIN32_CONTAINER_WINDOW_H
#define WORDRING_WIN32_CONTAINER_WINDOW_H

#include <wordring/debug.h>

#include <wordring/gui/detail/win32/win32_window.h>

namespace wordring
{
namespace gui
{
namespace detail
{

class dummy;

// native_container_window_impl -----------------------------------------------

class native_container_window_impl : public native_window_impl
{
public:
	native_container_window_impl();
	virtual ~native_container_window_impl();

	/// ウィンドウを作成します
	virtual void create(window* parent);

public:
	/// ウィンドウ・プロシージャの雛型です
	LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/// native_container_window_impl用にカスタマイズされたwin32ウィンドウ・クラスです
	struct window_class
		: public win32_window_class<window_class, native_container_window_impl>
	{
		static WNDCLASSEX create();
	};
	static window_class g_window_class;
};










} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_CONTAINER_WINDOW_H
