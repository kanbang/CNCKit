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

wchar_t const* container::get_control_name() const
{
	return L"wordring::gui::container";
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

bool container::is_ancestor_of(control const *c) const
{
	assert(c != nullptr);

	container const *c0 = c->get_parent();

	while (c0 != nullptr)
	{
		if (c0 == this) { return true; }
		c0 = c0->get_parent();
	}

	return false;
}

container::storage_type& container::get_children()
{
	return m_children;
}

void container::attach_window()
{
	//if ()
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

	repaint(get_rect());
}

// マウス・メッセージ ---------------------------------------------------------

bool container::do_mouse_down_internal(mouse &m)
{
	// 子コントロールの処理を開始

	m.pt -= get_position(); // 子コントロール用に位置を変更

	// 最前面からテストするために逆イテレータを使う
	storage_type::reverse_iterator current =
		get_children().rbegin(), last = get_children().rend();

	while (current != last)
	{
		if ((*current)->get_rect().including(m.pt)) // ヒット・テスト
		{
			// 子が処理したら終わり
			if ((*current)->do_mouse_down_internal(m)) { return true; }
		}
		++current;
	}
	// 子コントロールの処理終わり

	m.pt += get_position(); // this用に位置を変更

	return do_mouse_down(m);
}

void container::do_mouse_move_internal(mouse &m)
{
	window_service *service = find_service();

	service->get_mouse_service().process_bubble_up(this, m);

	// 子コントロールの処理を開始

	m.pt -= get_position(); // 子コントロール用に位置を変更

	// 最前面からテストするために逆イテレータを使う
	storage_type::reverse_iterator
		current = get_children().rbegin(),
		last = get_children().rend();

	while (current != last)
	{
		if ((*current)->get_rect().including(m.pt)) // ヒット・テスト
		{
			(*current)->do_mouse_move_internal(m);
			break;
		}
		++current;
	}
	// 子コントロールの処理終わり

	m.pt += get_position(); // this用に位置を変更

	// バブルを上昇させられない場合、このコンテナがトップ
	if (current == last)
	{
		service->get_mouse_service().process_bubble_top(this, m);
	}

	// ハンドラ呼び出し
	do_mouse_move(m);
}

bool container::do_mouse_up_internal(mouse &m)
{
	// 子コントロールの処理を開始

	m.pt -= get_position(); // 子コントロール用に位置を変更

	// 最前面からテストするために逆イテレータを使う
	storage_type::reverse_iterator current =
		get_children().rbegin(), last = get_children().rend();

	while (current != last)
	{
		if ((*current)->get_rect().including(m.pt)) // ヒット・テスト
		{
			// 子が処理したら終わり
			if ((*current)->do_mouse_up_internal(m)) { return true; }
		}
		++current;
	}
	// 子コントロールの処理終わり

	m.pt += get_position(); // this用に位置を変更

	return do_mouse_up(m);
}

// キーボード・メッセージ -----------------------------------------------------

// 一般メッセージ -------------------------------------------------------------

void container::do_paint_internal(canvas& cv)
{
	rect_int rc0 = cv->get_viewport(); // コンテナ自身のビューポート
	//cv->set_viewport(rc0);

	do_paint(cv); // まず自分を描画する

	// 子コントロールを下（ストアのbegin()側）から描画していく
	for (store& s : m_children)
	{
		if (s->is_window()) { continue; }
		rect_int rc1 = s->query_rect_from_window() & rc0;
		if (!rc1.size) { continue; }
		cv->set_viewport(rc1);
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
	rect_int rc(point_int(0, 0), get_size());
	cv->fill_rect(rc, rgb_color(0xA0, 0xF0, 0xF0));
}
