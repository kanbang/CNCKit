/**
 * @file    wordring/gui/root_window.cpp
 *
 * @brief   ルート・ウィンドウの実装ファイル
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

#include <wordring/gui/root_window.h>

#include <algorithm>

#ifdef WORDRING_WS_WIN // Windows ---------------------------------------------

#include <wordring/gui/detail/win32/win32_child_window.h>

#else
#ifdef WORDRING_WS_X11 // X11 -------------------------------------------------

#include <wordring/gui/detail/x11/x11_child_window.h>

#endif // WORDRING_WS_X11
#endif // WORDRING_WS_WIN -----------------------------------------------------

using namespace wordring::gui;

// root_container -------------------------------------------------------------

// 構築・破棄 -----------------------------------------------------------------

root_container::root_container()
	: window_container(
		  rect_int() // ルート・ウィンドウによって決まるので空
		, full_layout::create()
		, std::move(detail::native_control_window_impl::create()))
{

}

root_container::~root_container()
{
	
}

root_container::store root_container::create()
{
	return store(new root_container());
}

void root_container::attach_root_window_internal(root_window *parent)
{
	m_root_window = parent;
	attach_window_internal(parent);
}

void root_container::detach_root_window_internal()
{
	find_service()->erase_message(this);
	detach_window_internal();
}
/*
void root_container::attach_window_internal()
{
	// 自分のウィンドウを作成する
	find_window()->get_native()->create_window(m_root_window, get_rect());

	// 子のウィンドウを処理する
	for (control::store &s : m_storage)
	{
		s->attach_window_internal();
	}
}
*/
// 情報 -----------------------------------------------------------------------

wchar_t const* root_container::get_control_name() const
{
	return L"wordring::gui::root_container";
}

root_window* root_container::find_root_window()
{
	return m_root_window;
}


window_service* root_container::find_service()
{
	return m_root_window->get_service();
}

// 大きさ・位置 ---------------------------------------------------------------

void root_container::set_rect(rect_int rc)
{
	set_rect_internal(rc, false, true);
}

// root_window ----------------------------------------------------------------

// 構築・破棄 -----------------------------------------------------------------

root_window::root_window(rect_int rc)
	: m_client(root_container::create())
	, m_pt(rc.pt)
	, m_size(rc.size)
{
}

root_window::root_window(rect_int rc, detail::native_window::store nw)
	: window(std::move(nw))
	, m_client(root_container::create())
	, m_pt(rc.pt)
	, m_size(rc.size)
{
}

root_window::~root_window()
{
	m_client.reset();
}

root_window::store root_window::create(rect_int rc)
{
	return store(new root_window(rc));
}

void root_window::attach_service_internal(window_service *ws)
{
	assert(ws);

	m_service = ws;
	get_native()->create_window(nullptr, get_rect());

	m_client->attach_root_window_internal(this);
}

void root_window::detach_service_internal()
{
	assert(m_service);

	m_client->detach_root_window_internal();
	m_service = nullptr;
}

void root_window::set_client(root_container::store s)
{
	m_client->detach_root_window_internal();
	m_client = std::move(s);
	m_client->attach_root_window_internal(this);
}

container* root_window::get_client()
{
	return m_client.get();
}

control* root_window::assign(control::store s)
{
	control *c = static_cast<control*>(s.get());
	m_client->push_back(std::move(s));

	return c;
}

// 情報 -----------------------------------------------------------------------

window_service* root_window::get_service()
{
	return m_service;
}

// 表示 -----------------------------------------------------------------------

void root_window::show()
{
	get_native()->show_window();
}

void root_window::hide()
{
	get_native()->hide_window();
}

// 大きさ・位置 ---------------------------------------------------------------

void root_window::set_rect(rect_int rc)
{
	get_native()->set_window_rect(rc);
}

rect_int root_window::get_rect() const
{
	return rect_int(m_pt, m_size);
}

// その他 ---------------------------------------------------------------------

std::wstring root_window::get_title_text() const
{
	return get_native()->get_window_text();
}

void root_window::set_title_text(std::wstring s)
{
	get_native()->set_window_text(s);
}

// 一般メッセージ -------------------------------------------------------------

void root_window::do_destroy_window()
{
	m_service->quit();
}

void root_window::do_paint_window(canvas &cv)
{
	// 基底実装のwindow::do_paint_window()呼び出しを抑制するためにオーバーライド
}

void root_window::do_size_window(size_int size)
{
	if (m_client)
	{
		m_client->set_rect(rect_int(point_int(0, 0), size));
	}
}


