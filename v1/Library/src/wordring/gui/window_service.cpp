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
#include <wordring/gui/detail/win32/win32_window_service.h>
#endif // _WIN32

#ifdef __linux__
// linux
#endif // __linux__




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

void layout_service::remove(container *c)
{
	m_queue.erase(
		std::remove_if(m_queue.begin(), m_queue.end(),
			[=](container *c0)->bool{ return c->is_ancestor_of(c0); }),
		m_queue.end());
}

container* layout_service::pop()
{
	container *c = m_queue.front();
	m_queue.pop_front();

	return c;
}

// timer_service --------------------------------------------------------------

timer_service::timer_service(window_service &ws)
	: m_window_service(ws)
{
	timer_service *self = this;
	m_thread = std::thread([=]{ self->run(); });
}

timer_service::~timer_service()
{

}

timer_service::store timer_service::create(window_service &ws)
{
	return store(new timer_service(ws));
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

// window_service -------------------------------------------------------------

window_service::window_service()
	: m_native(new wordring::gui::detail::native_window_service_impl)
{
	m_native->set_public(this);
	m_debug_control_cnt = 0;

	m_timer = timer_service::create(*this);
}

window_service::~window_service()
{
	m_root_windows.clear();
}

detail::native_window_service* window_service::get_native()
{
	return m_native.get();
}

root_window* window_service::push_back(root_store s)
{
	assert(s);

	root_window *rw = s.get();
	m_root_windows.push_back(std::move(s));

	rw->attach_parent(this);

	return rw;
}

void window_service::push(message::store m)
{
	std::lock_guard<mutex_type> g(m_mutex);

	m_queue.push_back(std::move(m));
	post_tick_message();
}

message::store window_service::pop()
{
	std::lock_guard<mutex_type> g(m_mutex);

	message::store m = std::move(m_queue.front());
	m_queue.pop_front();

	return std::move(m);
}

void window_service::remove(control *c)
{
	std::lock_guard<mutex_type> g(m_mutex);

	for (message::store &s : m_queue)
	{
		if (s->m_control == c) { m_queue; }
	}
}

void window_service::run()
{
	get_native()->run();
}

void window_service::quit()
{
	m_timer->quit();

	std::lock_guard<mutex_type> g(m_mutex);
	get_native()->quit();
}

void window_service::set_timer(control *c, int32_t ms)
{

}

void window_service::post_tick_message()
{
	get_native()->post_tick_message();
	std::cout << "push" << std::endl;
}

void window_service::do_tick_message()
{
	while (m_queue.size())
	{
		message::store m(pop());
		m->m_control->do_message_internal(*m);
	}
}