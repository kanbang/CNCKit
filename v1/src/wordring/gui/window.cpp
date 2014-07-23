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

window::window() : m_native_window(new detail::native_window_impl)
{
	//detail::native_window_impl* p = new detail::native_window_impl;
	//m_native_window.reset(p);

	m_native_window->set_window(this);
}

window::window(detail::native_window* p) : m_native_window(p)
{
	m_native_window->set_window(this);
}

window::~window()
{

}

void window::create(window * parent)
{
	m_native_window->create(parent);
}

detail::native_window* window::get_native_window()
{
	return m_native_window.get();
}

void window::close()
{
	m_native_window->close();
}

void window::destroy()
{
	m_native_window->destroy();
}

void window::show()
{
	m_native_window->show();
}

void window::hide()
{
	m_native_window->hide();
}

window* window::get_parent()
{
	return m_native_window->get_parent()->get_window();
}

void window::set_parent(window* parent)
{
	m_native_window->set_parent(parent->get_native_window());
}

void window::set_size(size_int size)
{
	m_native_window->set_size(size);
}

size_int window::get_size() const
{
	return m_native_window->get_size();
}
void window::set_position(point_int point)
{
	m_native_window->set_position(point);
}

point_int window::get_position() const
{
	return m_native_window->get_position();
}

/// メッセージ・ハンドラ
void window::do_create()
{
}

void window::do_destroy()
{
}

void window::do_paint(canvas& cv)
{
}

/*
button_window::button_window()
	: window(new wordring::gui::detail::native_button_window_impl)
{

}*/
