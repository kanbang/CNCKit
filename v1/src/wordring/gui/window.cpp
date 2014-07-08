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

#include <wordring/gui/window.h>

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
	m_native_window->set_window(this);
}

window::window(detail::native_window* p) : m_native_window(p)
{
	m_native_window->set_window(this);
}

window::~window(){}

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
	return m_native_window->destroy();
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
bool window::onCreate()
{
	return true;
}

/// メッセージ・ハンドラ
bool window::on_click()
{
	return true;
}

// control_window -------------------------------------------------------------

control_window_::control_window_()
	: window(new detail::native_control_window_impl)
{
}

// container_window -----------------------------------------------------------

container_window_::container_window_()
	: window(new detail::native_container_window_impl)
{

}

button_window::button_window()
	: window(new wordring::gui::detail::native_button_window_impl)
{

}