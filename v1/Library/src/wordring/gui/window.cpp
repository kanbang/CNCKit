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


window::window() : m_native(detail::native_window_impl::create())
{
	m_native->set_public(this);
}

window::window(detail::native_window::store nw) : m_native(std::move(nw))
{
	m_native->set_public(this);
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

// マウス・メッセージ ---------------------------------------------------------

void window::do_mouse_move_window(point_int pt)
{

}

/// メッセージ・ハンドラ
void window::do_create_window()
{
}

void window::do_destroy_window()
{
}

void window::do_paint_window(canvas& cv)
{
	cv.draw_string(
		"window::do_paint()", point_int(0, 0), rgb_color(), nullptr);
}

void window::do_size_window(size_int size)
{
}





