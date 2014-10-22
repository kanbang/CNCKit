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
	m_mouse_service.set_window_service(this);
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


text_service& window_service::get_text_service()
{
	return m_text_service;
}




