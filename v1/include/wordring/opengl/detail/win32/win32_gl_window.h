/**
 * @file    wordring/opengl/detail/win32/win32_gl_window.h
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

#ifndef WORDRING_WIN32_GL_WINDOW_H
#define WORDRING_WIN32_GL_WINDOW_H

#include <wordring/debug.h>

#include <wordring/gui/window.h>
#include <wordring/opengl/detail/native_gl_window.h>

#include <wordring/gui/detail/win32/win32_window_class.h>
//#include <gl/GL.h>

namespace wordring
{
namespace opengl
{
namespace detail
{

class dummy;

class native_gl_window_impl : public native_gl_window
{
protected:
	//HGLRC m_hglrc;
	/*
public:
	/// ウィンドウを作成します
	virtual void create(wordring::gui::window* parent);

public:
	/// ウィンドウ・プロシージャの雛型です
	LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/// native_container_window_impl用にカスタマイズされたwin32ウィンドウ・クラスです
	struct window_class
		: public wordring::gui::detail::win32_window_class<
			window_class, native_gl_window_impl>
	{
		static WNDCLASSEX create();
	};
	static window_class g_window_class;
*/
};


} // namespace detail
} // namespace opengl
} // namespace wordring

#endif // WORDRING_WIN32_GL_WINDOW_H
