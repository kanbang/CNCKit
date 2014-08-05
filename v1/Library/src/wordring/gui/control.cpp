/**
* @file    wordring/gui/control.cpp
*
* @brief   GUIコントロールの実装ファイル
*
* @details
*          ボタン、スクロールバーなどのGUIコントロールです。
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

#include <wordring/debug.h>

#include <wordring/geometry/shape.h>

#include <wordring/gui/control.h>
#include <wordring/gui/container.h>

#include <wordring/gui/root_window.h>

#include <wordring/gui/window.h>

//#include <wordring/gui/message.h>

#include <cassert>
#include <string>

using namespace wordring::gui;


// 構築・破棄 -----------------------------------------------------------------

control::control() : m_parent(nullptr)
{
}

control::~control()
{
}

control::store control::create()
{
	return store(new control);
}

void control::create(container* parent, rect_int rc)
{
	set_parent(parent);

	do_create();
}

// 情報 -----------------------------------------------------------------------

char const* control::get_control_name() const
{
	return "control";
}

bool control::is_window() const
{
	return false;
}

bool control::is_container() const
{
	return false;
}

window& control::find_window()
{
	assert(get_parent());
	return get_parent()->find_window();
}

container* control::find_container()
{
	return get_parent();
}

root_container* control::find_root_container()
{
	return get_parent()->find_root_container();
}

void control::set_root_window(root_window &rw)
{
	assert(false); // ルート・ウィンドウ直下にこのコントロールを配置できません
}

root_window& control::find_root_window()
{
	assert(get_parent());
	return get_parent()->find_root_window();
}

window_service& control::find_service()
{
	assert(get_parent());
	return get_parent()->find_service();
}

// 表示 -----------------------------------------------------------------------

bool control::is_visible() const
{
	return true;
}

void control::show()
{
}

void control::hide()
{

}

// 親子関係 -------------------------------------------------------------------

void control::set_parent(container *parent)
{
	//assert(parent != nullptr);
	m_parent = parent;
}

container* control::get_parent()
{
	return m_parent;
}

container const* control::get_parent() const
{
	return m_parent;
}
/*
bool control::is_ancestor(container const *c) const
{
	container const *c0 = nullptr;

	while ((c0 = get_parent()) != nullptr)
	{
		if (c0 == c) { return true; }
	}
	return false;
}
*/
// 描画 -----------------------------------------------------------------------

void control::repaint()
{
	window &w = find_window();

	point_int pt = query_position_from_window();

	w.get_native()->repaint_window(rect_int(pt, get_size()));
}

void control::repaint(rect_int rc)
{
	window &w = find_window();

	point_int pt = query_position_from_window();

	w.get_native()->repaint_window(rect_int(pt, rc.size));
}

void control::request_repaint(rect_int rc)
{
	/// TODO: メッセージ圧縮を実装する
	repaint(rc);
}

// 大きさ・位置 ---------------------------------------------------------------

void control::set_size(size_int size)
{
	set_rect(rect_int(m_pt, size));
}

size_int control::get_size() const
{
	return is_visible() ? m_size : size_int();
}

void control::set_position(point_int pt)
{
	set_rect(rect_int(pt, m_size));
}

point_int control::get_position() const
{
	return m_pt;
}

void control::set_rect(rect_int rc)
{
	// ルート・コンテナはオーバーライドする必要があります
	assert(get_parent() != nullptr);

	if (rc == rect_int(m_pt, m_size)) { return; } // ループ・ガード

	m_pt = rc.pt;
	m_size = rc.size;

	do_size(rc.size);

	get_parent()->request_layout();
}

rect_int control::get_rect() const
{
	return rect_int(m_pt, m_size);
}

size_int control::get_preferred_size() const
{
	return get_size();
}

size_int control::get_min_size() const
{
	return get_size();
}

size_int control::get_max_size() const
{
	return get_size();
}

point_int control::query_position_from_form() const
{
	return get_parent()->query_position_from_form() + get_position();
}

point_int control::query_position_from_window() const
{
	return get_parent()->query_position_from_window() + get_position();
}

rect_int control::query_rect_from_window() const
{
	// コントロールの長方形と親の長方形の重なる部分を求める
	point_int
		pt1 = get_parent()->query_position_from_window(),
		pt2 = query_position_from_window();

	rect_int
		rc1 = rect_int(pt1, get_parent()->get_size()),
		rc2 = rect_int(pt2, get_size());

	return rc1 & rc2; // 重なる長方形
}

// ライブラリ定義のメッセージ・ハンドラ ---------------------------------------

void control::do_message_internal(message &m)
{
}

// マウス・メッセージ ---------------------------------------------------------

bool control::do_mouse_move(point_int pt)
{
	return true;
}

bool control::do_mouse_move_internal(point_int pt)
{
	return do_mouse_move(pt);
}

void control::do_mouse_over(point_int pt)
{
}

void control::do_mouse_out(point_int pt)
{
}

// キーボード・メッセージ -------------------------------------------------

// 一般メッセージ -------------------------------------------------------------

void control::do_create()
{
}

void control::do_destroy()
{
}

void control::do_paint(canvas &cv)
{
#ifdef WORDRING_DEBUG_CONTROL

	size_int size = get_size();
	rect_int rc(point_int(0, 0), size_int(size.cx - 1, size.cy - 1));

	rgb_color rgb(0x3F, 0x3F, 0x3F);

	int32_t w = 3;

	cv.fill_rect(rc, rgb_color(0xA0, 0xA0, 0xA0));

	cv.draw_line(rc.top_left(), rc.top_right(), w, rgb);
	cv.draw_line(rc.bottom_left(), rc.bottom_right(), w, rgb);
	cv.draw_line(rc.top_left(), rc.bottom_left(), w, rgb);
	cv.draw_line(rc.top_right(), rc.bottom_right(), w, rgb);
	cv.draw_line(rc.top_left(), rc.bottom_right(), w, rgb);
	cv.draw_line(rc.top_right(), rc.bottom_left(), w, rgb);
	
	cv.draw_string(
		get_control_name(), point_int(0, 0), rgb, nullptr);

#endif // WORDRING_DEBUG_CONTROL
}

void control::do_paint_internal(canvas& cv)
{
	cv.set_viewport(query_rect_from_window());
	do_paint(cv);
}

void control::do_size(size_int size)
{
}


