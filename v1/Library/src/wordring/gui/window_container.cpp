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

void window_container::attach_window_internal()
{
	// 自身がウィンドウを持つため、ウィンドウ作成前の検索は必ず失敗する
	// そのため、親コンテナに検索させて差分を計算する

	// 位置の正規化
	rect_int rc = get_rect();
	rc.pt += get_parent()->query_offset_from_window();

	// 自分のウィンドウを作成する
	get_native()->create_window(get_parent()->find_window(), rc);

	// 子のウィンドウを処理する
	for (control::store &s : m_children)
	{
		s->attach_window_internal();
	}
}

void window_container::detach_window_internal()
{
	// 子のウィンドウを処理する
	for (control::store &s : m_children)
	{
		s->detach_window_internal();
	}

	// 自分のウィンドウを破棄する
	get_native()->destroy_window();
}

// 情報 -----------------------------------------------------------------------

wchar_t const* window_container::get_control_name() const
{
	return L"window_container";
}

bool window_container::is_window() const
{
	return true;
}

window* window_container::find_window()
{
	return this;
}

// 表示 -----------------------------------------------------------------------

void window_container::show()
{
	get_native()->show_window();
}

void window_container::hide()
{
	get_native()->hide_window();
}

// 大きさ・位置 ---------------------------------------------------------------

void window_container::set_rect_internal(rect_int rc, bool notify, bool paint)
{
	rect_int old = rc;
	std::swap(old, m_rc);

	rc.pt = query_offset_from_window();

	get_native()->set_window_rect(rc);

	if (notify)
	{
		assert(get_parent() != nullptr);
		get_parent()->get_layout()->do_child_rect(this, old);
	}

	if (paint) { repaint(); }
}

point_int window_container::query_offset_from_window() const
{
	return point_int(0, 0);
}

rect_int window_container::query_rect_from_window() const
{
	rect_int result;
	return m_rc;
}

// マウス・メッセージ ---------------------------------------------------------

void window_container::do_mouse_down_window(mouse &m)
{
	do_mouse_down_internal(m);
}

void window_container::do_mouse_up_window(mouse &m)
{
	do_mouse_up_internal(m);
}

void window_container::do_mouse_enter_window(mouse &m)
{
}

void window_container::do_mouse_leave_window()
{
	window_service *ws = find_service();
	ws->get_mouse_service().process_mouse_leave(this);
}

void window_container::do_mouse_move_window(mouse &m)
{
	do_mouse_move_internal(m);
}

void window_container::do_mouse_wheel_window(mouse &m)
{
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

