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

#include <wordring/debug.h>

#include <wordring/gui/message.h>
#include <wordring/gui/window_service.h>

#include <wordring/gui/control.h>
#include <wordring/gui/container.h>
#include <wordring/gui/root_window.h>

#ifdef _WIN32
#include <wordring/gui/detail/win32/win32_message_service.h>
#endif // _WIN32

#ifdef __linux__
// linux
#endif // __linux__


#include <memory>
#include <deque>
#include <algorithm>

#include <iostream>

using namespace wordring::gui;

// layout_service -------------------------------------------------------------

void layout_service::push(container *c)
{
	// キューが空の場合、開始位置にいる
	bool start = (m_queue.empty()) ? true : false;

	iterator it = m_queue.begin();
	while (it != m_queue.end())
	{
		// 祖先のコンテナがレイアウト予約されている場合、子孫は予約する必要がない
		if ((*it)->is_ancestor_of(c)) { return; }
		// 子孫が予約されている場合、祖先と挿げ替える
		if (c->is_ancestor_of(*it))
		{
			m_queue.erase(it);
			m_queue.push_back(c);
			return; // 既に何かが有るのなら、開始位置ではないのでreturn出来る
		}
	}

	// レイアウトを予約
	m_queue.push_back(c);

	if (start) // 開始位置にいる場合、レイアウトを開始する
	{
		while (!m_queue.empty())
		{
			container *c0 = pop();
			c0->perform_layout();
		}
	}
}

void layout_service::erase(container *c)
{
	m_queue.erase(
		std::remove_if(
			m_queue.begin(),
			m_queue.end(),
			[=](container *c0)->bool{ return c->is_ancestor_of(c0); }),
		m_queue.end());
}

container* layout_service::pop()
{
	container *c = m_queue.front();
	m_queue.pop_front();

	return c;
}

// mouse_service --------------------------------------------------------------

mouse_service::mouse_service()
{

}

void mouse_service::process_bubble_up(control *c, mouse &m)
{
	if (std::find(m_queue.begin(), m_queue.end(), c) == m_queue.end())
	{
		m_queue.push_back(c);
		c->do_mouse_over(m);
	}
}

void mouse_service::process_bubble_top(control *c, mouse &m)
{
	iterator it = std::remove_if(
		m_queue.begin(),
		m_queue.end(),
		[&](control *c0)->bool{ return process_mouse_out(c0, c, m); });

	m_queue.erase(it, m_queue.end());
}

bool mouse_service::process_mouse_out(control *c0, control *c, mouse &m)
{
	bool result = true; // マウス・カーソルがc0から出ていればtrue

	// 同じコントロール内でptが変わっただけの場合、カーソルは出ていない
	if (c0 == c) { result = false; }

	// カーソルのあるコントロールを載せているコンテナは、カーソルが出ていない
	if (c0->is_container() && static_cast<container*>(c0)->is_ancestor_of(c))
	{
		result = false;
	}

	if (result)
	{
		c0->do_mouse_out(m);
	}

	return result;
}

void mouse_service::process_mouse_leave(control *c)
{
	// コントロールの場合、cだけ処理する
	if (!c->is_container())
	{
		m_queue.erase(
			std::find(m_queue.begin(), m_queue.end(), c),
			m_queue.end());
		return;
	}

	// コンテナの場合、cの子もすべて処理する
	iterator it = std::remove_if(
		m_queue.begin(),
		m_queue.end(),
		[&](control *c0)->bool{
			assert(c->is_container());
			if (static_cast<container*>(c)->is_ancestor_of(c0))
			{
				mouse m(0, 0);
				c0->do_mouse_out(m);
				return true;
			}
			return false;
		});

	m_queue.erase(it, m_queue.end());
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

	rw->attach_service(this);

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
	if (c->is_container())
	{
		m_layout_service.erase(static_cast<container*>(c));
	}

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
		s->detach_service();
	}
	m_windows.clear();
	m_message_service.quit();
}

void window_service::set_timer(control *c, std::chrono::milliseconds ms)
{

}

layout_service& window_service::get_layout_service()
{
	return m_layout_service;
}

mouse_service& window_service::get_mouse_service()
{
	return m_mouse_service;
}

style_service& window_service::get_style_service()
{
	return m_style_service;
}