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

#ifdef _WIN32
#include <wordring/gui/detail/win32/win32_window_service.h>
#endif // _WIN32

#ifdef __linux__
// linux
#endif // __linux__




#include <iostream>

using namespace wordring::gui;

window_service::window_service()
	: m_native(new wordring::gui::detail::native_window_service_impl)
{
	m_native->set_public(this);
	m_debug_control_cnt = 0;
}

window_service::~window_service()
{

}

detail::native_window_service& window_service::get_native()
{
	return *(m_native.get());
}

int window_service::get_debug_message_count()
{
	return m_debug_control_cnt++;
}

void window_service::push(message::store m)
{
	m_queue.push_back(std::move(m));
	post_tick_message();
}

message::store window_service::pop()
{
	message::store m = std::move(m_queue.front());
	m_queue.pop_front();

	return std::move(m);
}

void window_service::run()
{
	get_native().run();
}

void window_service::quit()
{
	get_native().quit();
}

void window_service::post_tick_message()
{
	get_native().post_tick_message();
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