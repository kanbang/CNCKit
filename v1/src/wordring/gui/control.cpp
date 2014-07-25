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

#include <wordring/gui/control.h>
#include <wordring/gui/container.h>

#include <cassert>

using namespace wordring::gui;


control::control() : m_parent(nullptr) { }

// ----------------------------------------------------------------------------

bool control::is_window() const { return false; }

window* control::find_window() { return get_parent()->find_window(); }

form* control::find_form()
{
	container* parent = get_parent();
	assert(parent);
	return parent->find_form();
}


// ----------------------------------------------------------------------------

void control::set_parent(container* parent) { m_parent = parent; }

container* control::get_parent() { return m_parent; }

container const* control::get_parent() const { return m_parent; }

// ----------------------------------------------------------------------------

void control::repaint()
{
	window* w = find_window();
	assert(w);

	point_int pt = query_position_from_window();

	w->repaint_window(pt, get_size());
}

void control::repaint(point_int pt, size_int size) { assert(false); }

// ----------------------------------------------------------------------------

void control::set_size(size_int size) { set_rect(rect_int(m_position, size)); }

size_int control::get_size() const { return m_size; }

void control::set_position(point_int point) { m_position = point; }

point_int control::get_position() const { return m_position; }

void control::set_rect(rect_int rc)
{
	m_size = size_int(0, 0);
	//repaint
}

rect_int control::get_rect() const { return rect_int(m_position, m_size); }

// ----------------------------------------------------------------------------

point_int control::query_position_from_form() const
{
	point_int self = get_position();
	point_int result = get_parent()->query_position_from_form();
	result.x += self.x;
	result.y += self.y;

	return result;
}

point_int control::query_position_from_window() const
{
	point_int self = get_position();
	point_int result = get_parent()->query_position_from_window();
	result.x += self.x;
	result.y += self.y;

	return result;
}

// ----------------------------------------------------------------------------

// マウス・メッセージ ---------------------------------------------------------

void control::do_mouse_move(point_int pt)
{
	if (on_mouse_move) { on_mouse_move(pt); }
}

void control::do_mouse_over(point_int pt)
{
	if (on_mouse_over) { on_mouse_over(pt); }
}

void control::do_mouse_out(point_int pt)
{
	if (on_mouse_out) { on_mouse_out(pt); }
}


// 一般メッセージ -------------------------------------------------------------

void control::do_create() { if (on_create) { on_create(); } }
void control::do_destroy() { if (on_destroy) { on_destroy(); } }
void control::do_paint(canvas& cv) { }
void control::do_size(size_int size) { if (on_size) { return on_size(size); } }
