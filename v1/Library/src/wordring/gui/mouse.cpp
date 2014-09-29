/**
 * @file    wordring/gui/mouse.cpp
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

#include <wordring/wordring.h>

#include <wordring/gui/mouse.h>
#include <wordring/gui/control.h>

using namespace wordring::gui;


// mouse_service --------------------------------------------------------------

mouse_service::mouse_service() : m_hover(nullptr), m_capture(nullptr)
{

}

window_service* mouse_service::get_window_service()
{
	return m_window_service;
}

void mouse_service::set_window_service(window_service *ws)
{
	m_window_service = ws;
}

control* mouse_service::get_hover()
{
	return m_hover;
}

control* mouse_service::set_hover(control *c, mouse &m)
{
	if (c != m_hover)
	{
		if (m_hover) m_hover->do_mouse_out();
		c->do_mouse_over(m);
		std::swap(m_hover, c);
	}
	return c; // 元のm_hover
}

control* mouse_service::release_hover()
{
	control *result = m_hover;
	if (result) result->do_mouse_out();
	m_hover = nullptr;
	return result;
}

control* mouse_service::get_capture()
{
	return m_capture;
}

control* mouse_service::set_capture(control *c)
{
	std::swap(m_capture, c);
	return c;
}

void mouse_service::process_mouse_down(control *w, mouse &m)
{
	control *c = w->find_descendant_from_point(m.pt);
	m.pt -= c->query_offset_from(w);
	control::ancestor_iterator it1(c), it2(w->get_parent());
	while (it1 != it2)
	{
		if (it1->do_mouse_down_internal(m)) break;
		m.pt += it1->get_position();
		++it1;
	}
}

void mouse_service::process_mouse_up(control *w, mouse &m)
{
	control *c = w->find_descendant_from_point(m.pt);
	m.pt -= c->query_offset_from(w);
	control::ancestor_iterator it1(c), it2(w->get_parent());
	while (it1 != it2)
	{
		if (it1->do_mouse_up_internal(m)) break;
		m.pt += it1->get_position();
		++it1;
	}
}

void mouse_service::process_mouse_enter(control *w, mouse &m)
{

}

void mouse_service::process_mouse_leave(control *w)
{
	release_hover();
}

void mouse_service::process_mouse_move(control *w, mouse &m)
{
	control *c = nullptr;

	c = get_capture();
	if (c) // マウスがキャプチャされている場合
	{
		set_hover(c, m);
		c->do_mouse_move_internal(m);
	}
	else // マウスがキャプチャされていない場合
	{
		control::descendant_reverse_iterator
			it1(w, [&](control *c)->bool { return c->hit_test(m.pt); }), it2;
		std::for_each(it1, it2, [&](control *c1){
			c = c1;
			m.pt -= c->get_position(); // 位置正規化
			c->do_mouse_move_internal(m);
		});
		set_hover(c, m);
	}
}

void mouse_service::process_mouse_wheel(control *w, mouse &m)
{

}






