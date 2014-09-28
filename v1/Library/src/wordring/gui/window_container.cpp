/**
 * @file    wordring/gui/window_container.cpp
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

#include <wordring/gui/window_container.h>

#include <utility>
#include <algorithm>

using namespace wordring::gui;

// 構築・破棄 -----------------------------------------------------------------

window_container::window_container(
	rect_int rc, layout::store l, detail::native_window::store nw)
		: container(rc, std::move(l))
		, window(std::move(nw))
{
}

window_container::~window_container()
{
}

// 親子関係 -------------------------------------------------------------------

// 情報 -----------------------------------------------------------------------

wchar_t const* window_container::get_control_name() const
{
	return L"window_container";
}

bool window_container::is_window() const
{
	return true;
}

window* window_container::to_window()
{
	return this;
}

// 表示 -----------------------------------------------------------------------

// 大きさ・位置 ---------------------------------------------------------------

// マウス・メッセージ ---------------------------------------------------------

void window_container::do_mouse_down_window(mouse &m)
{
	assert(find_service());
	find_service()->get_mouse_service().process_mouse_down(this, m);
}

void window_container::do_mouse_up_window(mouse &m)
{
	assert(find_service());
	find_service()->get_mouse_service().process_mouse_up(this, m);
}

void window_container::do_mouse_enter_window(mouse &m)
{
	assert(find_service());
	find_service()->get_mouse_service().process_mouse_enter(this, m);
}

void window_container::do_mouse_leave_window()
{
	assert(find_service());
	find_service()->get_mouse_service().process_mouse_leave(this);
}

void window_container::do_mouse_move_window(mouse &m)
{
	assert(find_service());
	find_service()->get_mouse_service().process_mouse_move(this, m);
}

void window_container::do_mouse_wheel_window(mouse &m)
{
	assert(find_service());
	find_service()->get_mouse_service().process_mouse_wheel(this, m);
}

// キーボード・メッセージ -----------------------------------------------------

// 一般メッセージ -------------------------------------------------------------

void window_container::do_paint_window(canvas &cv)
{
	do_paint_internal(cv);
}

void window_container::do_size_window(size_int size)
{
	m_rc.size = size;
	get_layout()->perform_layout(this);
}

