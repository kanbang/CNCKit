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

#include <wordring/wordring.h>

#include <wordring/gui/shape_int.h>
#include <wordring/gui/control.h>
#include <wordring/gui/container.h>
#include <wordring/gui/root_window.h>
#include <wordring/gui/window.h>

#include <cassert>
#include <utility>
#include <string>
#include <atomic>
#include <cstdlib>
#include <algorithm>

using namespace wordring::gui;

// control::ancestor_iterator -------------------------------------------------

control::ancestor_iterator::ancestor_iterator() : m_current(nullptr)
{
}

control::ancestor_iterator::ancestor_iterator(control *c) : m_current(c)
{
}

control* control::ancestor_iterator::operator *()
{
	assert(m_current);
	return m_current;
}

control const* control::ancestor_iterator::operator *() const
{
	assert(m_current);
	return m_current;
}

control* control::ancestor_iterator::operator ->()
{
	assert(m_current);
	return m_current;
}

control const* control::ancestor_iterator::operator ->() const
{
	assert(m_current);
	return m_current;
}

control::ancestor_iterator control::ancestor_iterator::operator ++(int)
{
	ancestor_iterator result(*this);
	++(*this);
	return result;
}

control::ancestor_iterator& control::ancestor_iterator::operator ++()
{
	assert(m_current);
	m_current = m_current->get_parent();
	return *this;
}

bool control::ancestor_iterator::operator !=(
	ancestor_iterator const& rhs) const
{
	return m_current != rhs.m_current;
}

bool control::ancestor_iterator::operator ==(
	ancestor_iterator const& rhs) const
{
	return m_current == rhs.m_current;
}

// control::descendant_reverse_iterator ---------------------------------------

control::descendant_reverse_iterator::descendant_reverse_iterator()
	: m_current(nullptr)
{
}

control::descendant_reverse_iterator::descendant_reverse_iterator(
	control *first, selector_function fn) : m_current(first), m_selector(fn)
{
}

control* control::descendant_reverse_iterator::operator *()
{
	assert(m_current);
	return m_current;
}

control const* control::descendant_reverse_iterator::operator *() const
{
	assert(m_current);
	return m_current;
}

control* control::descendant_reverse_iterator::operator ->()
{
	assert(m_current);
	return m_current;
}

control const* control::descendant_reverse_iterator::operator ->() const
{
	assert(m_current);
	return m_current;
}

control::descendant_reverse_iterator
control::descendant_reverse_iterator::operator ++(int)
{
	descendant_reverse_iterator result(*this);
	++(*this);
	return result;
}

control::descendant_reverse_iterator&
control::descendant_reverse_iterator::operator ++()
{
	assert(m_current);
	control::reverse_iterator
		it1 = m_current->rbegin(), it2 = m_current->rend();
	while (it1 != it2)
	{
		if (m_selector(it1->get()))
		{
			m_current = it1->get();
			break;
		}
		++it1;
	}
	if (it1 == it2) m_current = nullptr;

	return *this;
}

bool control::descendant_reverse_iterator::operator !=(
	descendant_reverse_iterator const& rhs) const
{
	return m_current != rhs.m_current;
}

bool control::descendant_reverse_iterator::operator ==(
	descendant_reverse_iterator const& rhs) const
{
	return m_current == rhs.m_current;
}

// control --------------------------------------------------------------------

// 構築・破棄 -----------------------------------------------------------------

control::control(rect_int rc)
	: m_parent(nullptr)
	, m_rc(rc)
{
}

control::control(rect_int rc, layout::store l)
	: m_parent(nullptr)
	, m_rc(rc)
	, m_layout(std::move(l))
{
}

control::~control()
{
}

control::store control::create(rect_int rc)
{
	return store(new control(rc));
}

// 親子関係 -------------------------------------------------------------------

bool control::is_ancestor_of(control const *c) const
{
	assert(c != nullptr);

	control const *c0 = c->get_parent();

	while (c0 != nullptr)
	{
		if (c0 == this) { return true; }
		c0 = c0->get_parent();
	}

	return false;
}

control* control::find_descendant_from_point(point_int pt)
{
	// まず自身に含まれるかテストする
	if (!hit_test(pt)) return nullptr;

	control *result = nullptr;
	// 子のテストを開始
	descendant_reverse_iterator
		it1(this, [&](control *c)->bool { return c->hit_test(pt); }), it2;

	std::for_each(it1, it2, [&](control *c){
		pt -= c->get_position();
		result = c;
	});

	return result;
}

void control::attach_parent_internal(control *parent)
{
	assert(parent);

	m_parent = parent;

	// 自身はまだウィンドウが無い。
	// 親ウィンドウが作成済みであれば、自身のウィンドウを作成できる。
	// 親ウィンドウが未作成であれば、親ウィンドウが作成される時に
	// 子コントロールが再帰的にウィンドウ作成される。

	window *w = parent->find_window();
	if (w->get_native()->is_created()) { attach_window_internal(w); }
}

void control::detach_parent_internal()
{
	assert(m_parent);

	find_service()->erase_message(this);

	window *w = find_window();
	if (w->get_native()->is_created()) { detach_window_internal(); }

	m_parent = nullptr;
}

void control::attach_window_internal(window *pw)
{
	// もし自身がウィンドウを継承していれば、ウィンドウを作成する
	window *self = to_window();
	if (self)
	{
		// 位置の正規化
		point_int pt = get_position();

		control const *pc = get_parent();
		if (pc)
		{
			// 自身がウィンドウを持つため、ウィンドウ作成前の検索は必ず失敗する
			// そのため、親コンテナに検索させて差分を計算する
			pt += pc->query_offset_from_window();
		}
		self->get_native()->create_window(pw, rect_int(pt, get_size()));

		pw = self; // 子のために自身を親とする
	}

	// 子のウィンドウを処理する
	iterator it1 = begin(), it2 = end();
	while (it1 != it2)
	{
		(*it1)->attach_window_internal(pw);
		++it1;
	}
}

void control::detach_window_internal()
{
	// 子のウィンドウを処理する
	iterator it1 = begin(), it2 = end();
	while (it1 != it2)
	{
		(*it1)->detach_window_internal();
		++it1;
	}

	// もし自身がウィンドウを継承していれば、ウィンドウを破棄する
	window *w = to_window();
	if (w)
	{
		w->get_native()->destroy_window();
	}
}

control* control::get_parent()
{
	return m_parent;
}

control const* control::get_parent() const
{
	return m_parent;
}

control* control::push_back(control::store s)
{
	control *c = s.get();
	m_storage.push_back(std::move(s));

	c->attach_parent_internal(this);
	return c;
}

control::iterator control::begin()
{
	return m_storage.begin();
}

control::iterator control::end()
{
	return m_storage.end();
}

control::const_iterator control::begin() const
{
	return m_storage.begin();
}

control::const_iterator control::end() const
{
	return m_storage.end();
}

control::reverse_iterator control::rbegin()
{
	return m_storage.rbegin();
}

control::reverse_iterator control::rend()
{
	return m_storage.rend();
}

control::const_reverse_iterator control::rbegin() const
{
	return m_storage.rbegin();
}

control::const_reverse_iterator control::rend() const
{
	return m_storage.rend();
}

// 情報 -----------------------------------------------------------------------

wchar_t const* control::get_control_name() const
{
	return L"control";
}

bool control::is_window() const
{
	return false;
}

bool control::is_container() const
{
	return false;
}

window* control::find_window()
{
	control *c = this;
	while (!c->to_window())
	{
		c = c->get_parent();
	}
	return c->to_window();
}

window* control::to_window()
{
	return nullptr;
}

window_service* control::find_service()
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
	window *w = to_window();
	if (w)
	{
		w->get_native()->show_window();
	}
}

void control::hide()
{
	window *w = to_window();
	if (w)
	{
		w->get_native()->show_window();
	}
}

// 描画 -----------------------------------------------------------------------

void control::repaint()
{
	repaint(get_rect());
}

void control::repaint(rect_int rc)
{
	window *w = find_window();

	point_int pt = query_offset_from_window();
	rc.pt = pt;

	w->get_native()->repaint_window(rc);
}

// 大きさ・位置 ---------------------------------------------------------------

void control::set_position(point_int pt)
{
	set_rect(rect_int(pt, m_rc.size));
}

point_int control::get_position() const
{
	return m_rc.pt;
}

void control::set_rect(rect_int rc)
{
	set_rect_internal(rc, true, true);
}

void control::set_rect_internal(rect_int rc, bool notify, bool paint)
{
	rect_int old = m_rc;
	m_rc = rc;

	window *w = to_window();
	if (w) // 自身がウィンドウである場合
	{
		rc.pt = query_offset_from_window();
		w->get_native()->set_window_rect(rc);
	}

	layout *l = get_layout();
	if (l) // レイアウトを持つ場合（コンテナである）
	{
		l->perform_layout(this);
	}

	if (notify)
	{
		assert(get_parent() != nullptr);
		get_parent()->get_layout()->do_child_rect(this, old);
	}

	if (paint) { repaint(); }
}

rect_int control::get_rect() const
{
	return m_rc;
}

void control::set_size(size_int size)
{
	set_rect(rect_int(m_rc.pt, size));
}

size_int control::get_size() const
{
	return m_rc.size;
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

point_int control::query_offset_from_window() const
{
	point_int result(0, 0);
	control const *c = this;
	while (c)
	{
		if (c->is_window()) break;
		result += c->get_position();
		c = c->get_parent();
	}
	return result;
}

rect_int control::query_rect_from_window() const
{
	// ウィンドウからの相対でコントロールの長方形と親の長方形の重なる部分を
	// 求める
	point_int
		pt1 = get_parent()->query_offset_from_window(),
		pt2 = query_offset_from_window();

	rect_int
		rc1 = rect_int(pt1, get_parent()->get_size()),
		rc2 = rect_int(pt2, get_size());

	return rc1 & rc2; // 重なる長方形
}

point_int control::query_offset_from(control *c) const
{
	assert(c->is_ancestor_of(this));

	point_int result = get_position();
	control const *c0 = get_parent();

	do
	{
		if (c0 == c) { return result; }
		result += c0->get_position();
	}
	while ((c0 = c0->get_parent()) != nullptr);

	return point_int();
}

bool control::hit_test(point_int pt) const
{
	return get_rect().including(pt);
}

// スタイル -------------------------------------------------------------------

control_style* control::get_style()
{
	return static_cast<control_style*>(m_style.get());
}

void control::set_style(style::store s)
{
	m_style = s;
}

// レイアウト -----------------------------------------------------------------

void control::set_layout(layout::store l)
{
	m_layout = std::move(l);
}

layout* control::get_layout()
{
	return m_layout.get();
}

// タイマー -------------------------------------------------------------------

void control::set_timer(int32_t ms)
{
	find_service();
}

// ライブラリ定義のメッセージ・ハンドラ ---------------------------------------

void control::do_message_internal(message &m)
{
}

// マウス・メッセージ ---------------------------------------------------------
/*
bool control::do_click(mouse &m)
{
	return false;
}
*/
bool control::do_mouse_down(mouse &m)
{
	return false;
}

bool control::do_mouse_down_internal(mouse &m)
{
	return do_mouse_down(m);
}

void control::do_mouse_move(mouse &m)
{
}

void control::do_mouse_move_internal(mouse &m)
{
}

void control::do_mouse_over(mouse &m)
{
}

void control::do_mouse_out()
{
}

bool control::do_mouse_up(mouse &m)
{
	return false;
}

bool control::do_mouse_up_internal(mouse &m)
{
	// 最前面からテストするために逆イテレータを使う
	// 子コントロールの処理を開始
	storage_type::reverse_iterator it1 = rbegin(), it2 = rend();

	if (it1 != it2)
	{
		m.pt -= get_position(); // 子コントロール用に位置を変更

		while (it1 != it2)
		{
			if ((*it1)->get_rect().including(m.pt)) // ヒット・テスト
			{
				// 子が処理したら終わり
				if ((*it1)->do_mouse_up_internal(m)) { return true; }
			}
			++it1;
		}
		// 子コントロールの処理終わり

		m.pt += get_position(); // this用に位置を変更
	}

	return do_mouse_up(m);
}

// キーボード・メッセージ -------------------------------------------------

// 一般メッセージ -------------------------------------------------------------

void control::do_create()
{
}

void control::do_create_internal()
{
	do_create();
}

void control::do_destroy()
{
}

void control::do_paint(canvas &cv)
{
#ifdef WORDRING_DEBUG_CONTROL

	size_int size = get_size();
	rect_int rc(point_int(0, 0), size_int(size.cx - 1, size.cy - 1));

	color_rgb rgb(0x3F, 0x3F, 0x3F);

	int32_t w = 3;

	cv.fill_rect(rc, color_rgb(0xA0, 0xA0, 0xA0));

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
	do_paint(cv); // まず自分を描画する

	iterator it1 = begin(), it2 = end();
	if (it1 != it2)
	{
		rect_int rc0 = cv->get_viewport(); // コンテナ自身のビューポート
		point_int origin = cv->get_origin();

		// 子コントロールを下（ストアのbegin()側）から描画していく
		while (it1 != it2)
		{
			// continueに備えて、ここでイテレータをインクリメントしておく
			control *c = (*it1++).get();
			// windowはシステムからメッセージが来るのでここでは描画しない
			if (c->to_window()) { continue; }
			// 重なりを調べる
			rect_int rc1 = c->query_rect_from_window() & rc0;
			// 重なりが無い場合、描画しないでよい
			if (!rc1.size) { continue; }
			cv->set_viewport(rc1);
			cv->set_origin(origin + c->get_position());
			c->do_paint_internal(cv);
		}
	}
}

void control::do_size(size_int size)
{
}

// test_control ---------------------------------------------------------------

test_control::test_control(rect_int rc, int32_t id)
	: m_id(id)
	, m_fg_color(0xFF, 0xFF, 0xFF)
	, control(rc)
{

}

test_control::~test_control()
{

}

control::store test_control::create(rect_int rc, int32_t id)
{
	return control::store(new test_control(rc, id));
}

bool test_control::do_mouse_down(mouse &m)
{
	//std::cout << "down " << m.pt.x << ", " << m.pt.y << std::endl;

	std::swap(m_fg_color, m_bg_color);
	repaint();
	return true;
}

void test_control::do_mouse_over(mouse &m)
{
	//std::cout << m.pt.x << ", " << m.pt.y << std::endl;
	//m_fg_color = color_rgb(0x33, 0, 0);
	std::swap(m_fg_color, m_bg_color);
	repaint();
}

void test_control::do_mouse_out()
{
	//std::cout << m.pt.x << ", " << m.pt.y << std::endl;
	std::swap(m_fg_color, m_bg_color);
	repaint();
}

bool test_control::do_mouse_up(mouse &m)
{
	//std::cout << "up " << m.pt.x << ", " << m.pt.y << std::endl;
	std::swap(m_fg_color, m_bg_color);
	repaint();
	return true;
}

void test_control::do_paint(canvas &cv)
{
	rect_int rc = rect_int(point_int(0, 0), get_size());

	point_int
		pt1(rc.left(), rc.top()), pt2(rc.right(), rc.top()),
		pt3(rc.left(), rc.bottom()), pt4(rc.right(), rc.bottom());

	color_rgb fg(0x3F, 0x3F, 0x3F);
	//color_rgb bg(0xA0, 0x0, 0xA0);

	int32_t w = 1;

	cv->fill_rect(rc, m_bg_color);

	cv->draw_rect(rc, w, m_fg_color);

	cv->draw_line(pt1, pt4, w, m_fg_color);
	cv->draw_line(pt2, pt3, w, m_fg_color);

	cv->draw_string(
		get_control_name(), point_int(0, 0), fg, nullptr);

	std::string src("(");
	src += std::to_string(get_rect().left());
	src += ", ";
	src += std::to_string(get_rect().top());
	src += ") - (";
	src += std::to_string(get_rect().right());
	src += ", ";
	src += std::to_string(get_rect().bottom());
	src += ") ";
	src += std::to_string(m_id);
	cv->draw_string(src, point_int(0, 16), fg, nullptr);

	//std::cout << "paint: " << m_id << std::endl;
}
