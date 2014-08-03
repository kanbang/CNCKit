/**
 * @file    wordring/gui/window.cpp
 *
 * @brief   ウィンドウの実装ファイル
 *
 * @details
 *          pimplの環境固有ヘッダをこのファイルにのみインクルードしてください。
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

#include <wordring/debug.h>

#include <wordring/gui/window.h>
#include <wordring/gui/canvas.h>

#ifdef _WIN32
#include <wordring/gui/detail/win32/win32_window.h>
#endif // _WIN32

#ifdef __linux__
// linux
#endif // __linux__

using namespace wordring::gui;

// window ---------------------------------------------------------------------

window::window() : m_native(new detail::native_window_impl)
{
	m_native->set_window(this);
}

window::window(detail::native_window* p) : m_native(p)
{
	m_native->set_window(this);
}

window::~window()
{

}

detail::native_window* window::get_native()
{
	return m_native.get();
}

detail::native_window const* window::get_native() const
{
	return m_native.get();
}

/*
// ----------------------------------------------------------------------------

void window::create_window(window * parent, rect_int rc)
{
	m_native_window->create_window(parent, rc);
}

void window::close_window()
{
	m_native_window->close_window();
}

void window::destroy_window()
{
	m_native_window->destroy_window();
}

// ----------------------------------------------------------------------------

void window::show_window()
{
	m_native_window->show_window();
}

void window::hide_window()
{
	m_native_window->hide_window();
}

// ----------------------------------------------------------------------------

detail::native_window* window::get_native_window()
{
	return m_native_window.get();
}

window* window::get_parent_window()
{
	return m_native_window->get_native_parent_window()->get_window();
}

window const* window::get_parent_window() const
{
	return m_native_window->get_native_parent_window()->get_window();
}

void window::set_parent_window(window* parent)
{
	m_native_window->set_native_parent_window(parent->get_native_window());
}


// ----------------------------------------------------------------------------

void window::repaint_window()
{
	m_native_window->repaint_window();
}

void window::repaint_window(rect_int rc)
{
	m_native_window->repaint_window(rc);
}

// ----------------------------------------------------------------------------

void window::set_window_size(size_int size)
{
	m_native_window->set_window_size(size);
}

size_int window::get_window_size() const
{
	return m_native_window->get_window_size();
}

void window::set_window_position(point_int point)
{
	m_native_window->set_window_position(point);
}

point_int window::get_window_position() const
{
	return m_native_window->get_window_position();
}

/// ウィンドウの位置と大きさを設定します
void window::set_window_rect(rect_int rc)
{
	m_native_window->set_window_rect(rc);
}

/// ウィンドウの位置と大きさを取得します
rect_int window::get_window_rect() const
{
	return m_native_window->get_window_rect();
}

// ----------------------------------------------------------------------------
*/
/// メッセージ・ハンドラ
void window::do_create_w()
{
}

void window::do_destroy_w()
{
}

void window::do_paint_w(canvas& cv)
{
	cv.draw_string(
		"window::do_paint()", point_int(0, 0), rgb_color(), nullptr);
}

void window::do_size_w(size_int size)
{
}





