/**
 * @file    wordring/gui/window_service.cpp
 *
 * @brief   ウィンドウ・サービスの実装ファイル
 *
 * @details
 *          一般的なGUIライブラリにおけるメッセージ・ポンプ相当です。
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

#include <wordring/gui/message.h>
#include <wordring/gui/window_service.h>

#include <wordring/gui/control.h>
#include <wordring/gui/container.h>
#include <wordring/gui/root_window.h>

#ifdef _WIN32
#endif // _WIN32

#ifdef WORDRING_WS_WIN // Windows ---------------------------------------------

#include <wordring/gui/detail/win32/win32_message_service.h>

#else
#ifdef WORDRING_WS_X11 // X11 -------------------------------------------------

#include <wordring/gui/detail/x11/x11_message_service.h>

#endif // WORDRING_WS_X11
#endif // WORDRING_WS_WIN -----------------------------------------------------

#include <memory>
#include <deque>
#include <algorithm>

using namespace wordring::gui;

// mouse_service --------------------------------------------------------------

mouse_service::mouse_service() : m_hover(nullptr), m_capture(nullptr)
{

}

control* mouse_service::get_hover()
{
	return m_hover;
}

control* mouse_service::set_hover(control *c, mouse &m)
{
	if (c != m_hover)
	{
		if(m_hover) m_hover->do_mouse_out();
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

// timer_service --------------------------------------------------------------

timer_service::timer_service()
{
	timer_service *self = this;
	m_thread = std::thread([=]{ self->run(); });
}

timer_service::~timer_service()
{

}

void timer_service::push(message::store m)
{
	std::lock_guard<mutex_type> g(m_mutex);

	m_queue.push_back(std::move(m));
}

timer_service::iterator timer_service::begin()
{
	return m_queue.begin();
}

timer_service::iterator timer_service::end()
{
	return m_queue.end();
}

void timer_service::run()
{
	m_run_flag = true;
	while (true)
	{
		if (!m_run_flag) { break; }

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void timer_service::quit()
{
	m_run_flag = false;
	m_thread.join();
}

// message_service ------------------------------------------------------------

message_service::message_service()
	: m_native(std::make_unique<detail::native_message_service_impl>())
	, m_window_service(nullptr)
{
	m_native->set_public(this);
}

message_service::~message_service()
{

}

detail::native_message_service* message_service::get_native()
{
	return m_native.get();
}

void message_service::set_window_service(window_service *ws)
{
	m_window_service = ws;
}

void message_service::push(message::store s)
{
	std::lock_guard<mutex_type> g(m_mutex);

	m_queue.push_back(std::move(s));
	get_native()->tick();
}

message::store message_service::pop()
{
	std::lock_guard<mutex_type> g(m_mutex);

	message::store s = std::move(m_queue.front());
	m_queue.pop_front();

	return std::move(s);
}

void message_service::erase(control *c)
{
	std::lock_guard<mutex_type> g(m_mutex);

	iterator
		first = m_queue.begin(),
		last = m_queue.end();

	if (c->is_container()) // コンテナは子孫のコントロールも削除する
	{
		container *c0 = static_cast<container*>(c);
		m_queue.erase(std::remove_if(
			first,
			last,
			[=](message::store &s)->bool
				{ return c0->is_ancestor_of(s->m_control); }), last);
	}
	else // コントロールはコントロールのみ削除する
	{
		m_queue.erase(std::remove_if(
			first,
			last,
			[=](message::store &s)->bool{ return c == s->m_control; }), last);
	}
}

void message_service::run()
{
	get_native()->run();
}

void message_service::quit()
{
	std::lock_guard<mutex_type> g(m_mutex);
	get_native()->quit();
}

void message_service::do_tack()
{
}

// window_service -------------------------------------------------------------

window_service::window_service()
{
	m_message_service.set_window_service(this);
	m_font_service.set_window_service(this);
}

window_service::~window_service()
{
	m_windows.clear();
}

root_window* window_service::push_back(root_store s)
{
	assert(s);

	root_window *rw = s.get();
	m_windows.push_back(std::move(s));

	rw->attach_service_internal(this);

	return rw;
}

root_window::store window_service::remove(root_window *rw)
{

	return root_window::store();
}

void window_service::post_message(message::store m)
{
	m_message_service.push(std::move(m));
}

void window_service::erase_message(control *c)
{

	m_message_service.erase(c);
}

void window_service::run()
{
	m_message_service.run();
}

void window_service::quit()
{
	for (root_window::store &s : m_windows)
	{
		s->detach_service_internal();
	}
	m_windows.clear();
	m_message_service.quit();
}

void window_service::set_timer(control *c, std::chrono::milliseconds ms)
{

}

mouse_service& window_service::get_mouse_service()
{
	return m_mouse_service;
}
/*
control_data_service& window_service::get_data_service()
{
	return m_data_service;
}
*/

font_service& window_service::get_font_service()
{
	return m_font_service;
}