/**
 * @file    wordring/gui/container.cpp
 *
 * @brief   GUIコンテナの実装ファイル
 *
 * @details
 *          フォームなどのGUIコンテナです。
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

#include <wordring/gui/window_service.h>

#include <wordring/gui/control.h>
#include <wordring/gui/container.h>
#include <wordring/gui/root_window.h>

#include <cassert>
#include <memory>


using namespace wordring::gui;

// 構築・破棄 -----------------------------------------------------------------

container::container(rect_int rc)
	: m_layout(flow_layout::create())
	, control(rc)
{
}

container::container(rect_int rc, layout::store l)
	: m_layout(std::move(l))
	, control(rc)
{
}

container::~container()
{
	m_children.clear();
}

control::store container::create(rect_int rc)
{
	return control::store(new container(rc));
}

control::store container::create(rect_int rc, layout::store l)
{
	return control::store(new container(rc, std::move(l)));
}

control* container::push_back(control::store s)
{
	control *c = s.get();
	m_children.push_back(std::move(s));

	c->attach_parent(this);
	return c;
}

void container::set_layout(layout::store l)
{
	m_layout = std::move(l);
}

// 情報 -----------------------------------------------------------------------

char const* container::get_control_name() const
{
	return "container";
}

bool container::is_container() const
{
	return true;
}

container* container::find_container()
{
	return this;
}

// 親子関係 -------------------------------------------------------------------

bool container::is_ancestor_of(control *c) const
{
	container const *c0 = c->get_parent();

	do
	{
		if (c0 == this) { return true; }
	}
	while ((c0 = c0->get_parent()) != nullptr);

	return false;
}

container::storage_type& container::get_children()
{
	return m_children;
}

void container::attach_window()
{
	for (control::store &s : m_children)
	{
		s->attach_window();
	}
}

void container::detach_window()
{
	for (control::store &s : m_children)
	{
		s->detach_window();
	}
}

// 描画 -----------------------------------------------------------------------

// 大きさ・位置 ---------------------------------------------------------------

void container::perform_layout()
{
	m_layout->perform_layout(this);

	for (store &s : m_children)
	{
		if (s->is_container())
		{
			container *c = static_cast<container*>(s.get());
			c->perform_layout();
		}
	}

	request_repaint(get_rect());
}

void container::request_layout()
{
	find_service()->request_layout(this);
}

// マウス・メッセージ ---------------------------------------------------------

bool container::do_mouse_move_internal(point_int pt)
{
	// 最前面からテストするために逆イテレータを使う
	storage_type::reverse_iterator current = get_children().rbegin();
	while (current != get_children().rend())
	{
		if ((*current)->get_rect().including(pt)) // ヒット・テスト
		{
			// 子コントロールが処理した場合、処理を終える
			if ((*current)->do_mouse_move_internal(pt)) { return true; }
			break;
		}
		++current;
	}

	// 子コントロールがヒットしない、あるいは処理しなかった場合、自身で処理を
	// 試し、結果を返す
	return do_mouse_move(pt);
}

// キーボード・メッセージ -----------------------------------------------------

// 一般メッセージ -------------------------------------------------------------

void container::do_paint_internal(canvas& cv)
{

	cv.set_viewport(query_rect_from_window());

	do_paint(cv); // まず自分を描画する

	// 子コントロールを下（ストアのbegin()側）から描画していく
	for (store& s : m_children)
	{
		if (s->is_window()) { continue; }
		s->do_paint_internal(cv);
	}
}

// test_container -------------------------------------------------------------

test_container::test_container(rect_int rc) : container(rc)
{

}

test_container::~test_container()
{

}

control::store test_container::create(rect_int rc)
{
	return control::store(new test_container(rc));
}

void test_container::do_paint(canvas &cv)
{
	cv.fill_rect(get_rect(), rgb_color(0xA0, 0xF0, 0xF0));
}
